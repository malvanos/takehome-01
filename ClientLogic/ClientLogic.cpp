// Copyright (c) 2025 Michael Alvanos Services LTD
// All rights reserved.
//
// This software is provided "as is" without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.

#include "pch.h"
#include "ClientLogic.h"
#include "../include/Logger.h"

ClientLogic::ClientLogic(Dependencies&& deps)
    : logger(std::move(deps.logger))
    , networkClientProvider(std::move(deps.networkClientProvider))
    , ioContext(deps.ioContext)
    , timer(deps.ioContext)
{
    randomNumberGenerator.seed(std::random_device{}());
}

void ClientLogic::start()
{
    auto self(shared_from_this());
    networkClientProvider->start(self);
    start_random_number_timer();
    logger->info("ClientLogic started");
}

void ClientLogic::start_random_number_timer() {
    auto self(shared_from_this());
    send_random_number_after(std::chrono::seconds(waitingPeriodForSendinNumber), [this, self]() {
        start_random_number_timer();
        });
}

void ClientLogic::send_random_number_after(std::chrono::seconds periodSeconds, std::function<void(void)>&& callback) {
    if (periodSeconds <= std::chrono::seconds(0)) {
        callback();
        return;
    }

    timer.expires_after(periodSeconds);
    timer.async_wait(
        [this, callback(std::move(callback))](const boost::system::error_code& ec) {
            if (forceShutdown) {
                logger->log(Logger::LogLevel::WARNING, "ClientLogic is shutting down, aborting timer.");
                return;
            }
            if (!ec) {
                logger->log(Logger::LogLevel::WARNING, "Random number timer expired.");
                sendRandomNumber();
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

void ClientLogic::sendSumOfSquaresRequest()
{
    uint64_t data = randomNumberDistribution(randomNumberGenerator);
    logger->info("Sending sum of squares request with number " + std::to_string(data));
    networkClientProvider->sendSumOfSquaresRequest(data);
}

void ClientLogic::sendRandomNumber()
{
    uint64_t data = randomNumberDistribution(randomNumberGenerator);
    logger->info("Sending random number " + std::to_string(data));
    networkClientProvider->sendRandomNumber(data);
}

void ClientLogic::onSumOfSquaresResponse(uint64_t sum) {
    logger->log(Logger::LogLevel::INFO, "Received sum of squares: " + std::to_string(sum));
}

void ClientLogic::stop()
{
    networkClientProvider->stop();
    timer.cancel();
    logger->info("ClientLogic stopped");
    forceShutdown = true;
}

ClientLogic::~ClientLogic()
{
    stop();
}