// Copyright (c) 2025 Michael Alvanos Services LTD
// All rights reserved.
//
// This software is provided "as is" without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// This software is the property of Michael Alvanos Services LTD.
// Redistribution or reproduction, in whole or in part, is not permitted
// without explicit prior written permission from the owner.

#include "pch.h"
#include "ClientLogic.h"
#include "../include/Logger.h"

ClientLogic::ClientLogic(Dependencies&& deps)
    : logger(std::move(deps.logger))
    , networkClientProvider(std::move(deps.networkClientProvider))
    , timer(std::move(deps.timer))
    , taskPoster(std::move(deps.taskPoster))
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
    auto self(shared_from_this());
    timer->start(periodSeconds, [this, self, callback = std::move(callback)]() {
        if (forceShutdown) {
            logger->log(Logger::LogLevel::WARNING, "ClientLogic is shutting down, aborting timer.");
            return;
        }
        sendRandomNumber();
        callback();
    });
}

void ClientLogic::sendSumOfSquaresRequest()
{
    auto self(shared_from_this());
    taskPoster([this,self]() {
        if (!transmitNumbers) return;
        uint64_t data = randomNumberDistribution(randomNumberGenerator);
        logger->info("Sending sum of squares request with number " + std::to_string(data));
        networkClientProvider->sendSumOfSquaresRequest(data);
    });
}

void ClientLogic::sendRandomNumber()
{
    if (!transmitNumbers) return;
    uint64_t data = randomNumberDistribution(randomNumberGenerator);
    logger->info("Sending random number " + std::to_string(data));
    networkClientProvider->sendRandomNumber(data);
}

void ClientLogic::onSumOfSquaresResponse(uint64_t sum) {
    logger->log(Logger::LogLevel::INFO, "Received sum of squares: " + std::to_string(sum));
}

void ClientLogic::onDisconnect()
{
    auto self(shared_from_this());
    taskPoster([this, self]() {
        logger->log(Logger::LogLevel::WARNING, "Disconnected from server. Will not send other numbers.");
        transmitNumbers = false;
    });
}

void ClientLogic::onConnect()
{
    auto self(shared_from_this());
    taskPoster([this, self]() {
        transmitNumbers = true;
    });
}

void ClientLogic::stop()
{
    auto self(shared_from_this());
    taskPoster([this, self]() {
        networkClientProvider->stop();
        timer->cancel();
        logger->info("ClientLogic stopped");
        forceShutdown = true;
    });
}
