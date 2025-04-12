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
#include "ServerLogic.h"
#include <chrono>
#include <functional>

ServerLogic::ServerLogic(Dependencies&& dependencies)
    : logger(std::move(dependencies.logger))
    , waitingPeriodForDumps(dependencies.waitingPeriodForDumps)
    , timer(std::move(dependencies.timer))
    , server(std::move(dependencies.server))
    , fileOperations(std::move(dependencies.fileOperations))
    , taskPoster(std::move(dependencies.taskPoster))
{
}

void ServerLogic::start()
{
    // We need to start the server
    server->start(shared_from_this());
    auto self(shared_from_this());
    taskPoster([this, self]() {
        logger->log(Logger::LogLevel::INFO, "Logic started");
        startSnapshotTimer();
    });
}

void ServerLogic::onNewNumber(uint64_t number) {
    auto self(shared_from_this());
    taskPoster([this, self, number]() {
        logger->info("New number received: " + std::to_string(number));
        numbersContainer.insert(number);
        }
    );
}

void ServerLogic::onAverageSquare(uint64_t number, std::shared_ptr<NumbersClient> whoAsked) {
    auto self(shared_from_this());
    taskPoster([this, self, number, whoAsked=std::move(whoAsked)]() {
            logger->info("Sum of squares requested: " + std::to_string(number));
            uint64_t sumOfSquares = number * number;

            for (uint64_t num : numbersContainer) {
                sumOfSquares += num * num;
            }
            logger->info("Sum of squares calculated: " + std::to_string(sumOfSquares));
            whoAsked->send(sumOfSquares);
        }
    );
}

void ServerLogic::stop()
{
    auto self(shared_from_this());
    taskPoster([this, self]() {
        logger->log(Logger::LogLevel::INFO, "Logic stopped");
        timer->cancel();
        fileOperations->stop();
        server->stop();
        forceShutdown = true;
     });
}

void ServerLogic::startSnapshotTimer() {
    logger->log(Logger::LogLevel::INFO, "Snapshot timer started");
    auto self(shared_from_this());
    takeSnapShot_after(std::chrono::seconds(waitingPeriodForDumps), [this, self]() {
        logger->log(Logger::LogLevel::INFO, "Taking snapshot...");
        startSnapshotTimer();
    });
}

void ServerLogic::takeSnapShot_after(std::chrono::seconds periodSeconds, std::function<void(void)>&& callback) {
    if (periodSeconds <= std::chrono::seconds(0)) {
        callback();
        return;
    }

    auto self(shared_from_this());
    timer->start(periodSeconds, [this, self, callback]() {
        if (forceShutdown) {
            logger->log(Logger::LogLevel::INFO, "Snapshot timer was aborted due to shutdown.");
            return;
        }
        fileOperations->writeFile(numbersContainer);
        callback();
    });
}