// Copyright (c) 2025 Michael Alvanos Services LTD
// All rights reserved.
//
// This software is provided "as is" without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.

#include "pch.h"
#include "Logic.h"
#include <chrono>
#include <functional>
#include "boost/asio.hpp"
#include "boost/asio/error.hpp"
#include "boost/system/error_code.hpp"
#include "boost/system/detail/error_category.hpp"

Logic::Logic(Dependencies&& dependencies)
    : logger(std::move(dependencies.logger))
    , waitingPeriodForDumps(dependencies.waitingPeriodForDumps)
    , timer(dependencies.io_context)
    , ioContext(dependencies.io_context)
{
}

Logic::~Logic()
{
}

void Logic::start(std::shared_ptr<NetworkProvider> server)
{
    this->server = server;
    logger->log(Logger::LogLevel::INFO, "Logic started");
    start_snapshot_timer();
}

void Logic::onNewNumber(int number) {
    auto self(shared_from_this());
    boost::asio::post(ioContext, [this, self, number]() {
        numbersContainer.insert(number);
        }
    );
}

void Logic::onAverageSquare(int number, std::shared_ptr<NetworkProvider> whoAsked) {
    auto self(shared_from_this());
    boost::asio::post(ioContext, [this, self, number, whoAsked=std::move(whoAsked)]() {
            double sum_of_squares = std::pow(number, 2);

            for (int num : numbersContainer) {
                sum_of_squares += std::pow(num, 2);
            }

            whoAsked->send(sum_of_squares);
        }
    );
}

void Logic::onNetworkStop()
{
    logger->log(Logger::LogLevel::INFO, "Network stopped");
    // Handle stop logic here
}


void Logic::stop()
{
    logger->log(Logger::LogLevel::INFO, "Logic stopped");
    timer.cancel();
}

void Logic::start_snapshot_timer() {
    logger->log(Logger::LogLevel::INFO, "Snapshot timer started");
    take_snapshot_after(std::chrono::seconds(waitingPeriodForDumps), [this]() {
        logger->log(Logger::LogLevel::INFO, "Taking snapshot...");
        start_snapshot_timer();
    });
}

void Logic::take_snapshot_after(std::chrono::seconds periodSeconds, std::function<void(void)>&& callback) {
    if (periodSeconds <= std::chrono::seconds(0)) {
        callback();
        return;
    }

    timer.expires_after(periodSeconds);
    timer.async_wait(
        [this, callback](const boost::system::error_code& ec) {
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
        });

}