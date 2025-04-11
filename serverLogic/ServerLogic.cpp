// Copyright (c) 2025 Michael Alvanos Services LTD
// All rights reserved.
//
// This software is provided "as is" without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.

#include "pch.h"
#include "ServerLogic.h"
#include <chrono>
#include <functional>
#include "boost/asio.hpp"
#include "boost/asio/error.hpp"
#include "boost/system/error_code.hpp"
#include "boost/system/detail/error_category.hpp"

ServerLogic::ServerLogic(Dependencies&& dependencies)
    : logger(std::move(dependencies.logger))
    , waitingPeriodForDumps(dependencies.waitingPeriodForDumps)
    , timer(dependencies.io_context)
    , ioContext(dependencies.io_context)
    , server(std::move(dependencies.server))
    , fileOperations(std::move(dependencies.fileOperations))
{
}

ServerLogic::~ServerLogic()
{
}

void ServerLogic::start()
{
    logger->log(Logger::LogLevel::INFO, "Logic started");
    start_snapshot_timer();
    server->start(shared_from_this());
}

void ServerLogic::onNewNumber(uint64_t number) {
    auto self(shared_from_this());
    boost::asio::post(ioContext, [this, self, number]() {
        numbersContainer.insert(number);
        }
    );
}

void ServerLogic::onAverageSquare(uint64_t number, std::shared_ptr<NumbersClient> whoAsked) {
    auto self(shared_from_this());
    boost::asio::post(ioContext, [this, self, number, whoAsked=std::move(whoAsked)]() {
            uint64_t sumOfSquares = number * number;

            for (uint64_t num : numbersContainer) {
                sumOfSquares += num * num;
            }

            whoAsked->send(sumOfSquares);
        }
    );
}

void ServerLogic::onNetworkStop()
{
    logger->log(Logger::LogLevel::INFO, "Network stopped");
    // Handle stop logic here
}


void ServerLogic::stop()
{
    auto self(shared_from_this());
    boost::asio::post(ioContext, [this, self]() {
        logger->log(Logger::LogLevel::INFO, "Logic stopped");
        timer.cancel();
        fileOperations->stop();
        server->stop();
        forceShutdown = true;
     });
}

void ServerLogic::start_snapshot_timer() {
    logger->log(Logger::LogLevel::INFO, "Snapshot timer started");
    auto self(shared_from_this());
    take_snapshot_after(std::chrono::seconds(waitingPeriodForDumps), [this, self]() {
        logger->log(Logger::LogLevel::INFO, "Taking snapshot...");
        start_snapshot_timer();
    });
}

void ServerLogic::take_snapshot_after(std::chrono::seconds periodSeconds, std::function<void(void)>&& callback) {
    if (periodSeconds <= std::chrono::seconds(0)) {
        callback();
        return;
    }

    timer.expires_after(periodSeconds);
    timer.async_wait(
        [this, callback](const boost::system::error_code& ec) {
            if (forceShutdown) {
                logger->log(Logger::LogLevel::INFO, "Snapshot timer was aborted due to shutdown.");
                return;
            }
            if (!ec) {
                logger->log(Logger::LogLevel::WARNING,"Snapshot timer expired, taking snapshot.");
                callback();
            }
            else if (ec == boost::asio::error::operation_aborted) {
                logger->log(Logger::LogLevel::WARNING, "Snapshot timer was aborted.");
            }
            else {
                logger->log(Logger::LogLevel::LOGERROR, "Snapshot timer error.");
            }
            fileOperations->writeFile(numbersContainer);
        });

}