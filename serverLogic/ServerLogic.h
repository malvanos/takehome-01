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

#pragma once

#include "../include/Logger.h"
#include <memory>
#include <unordered_set>
#include "../include/NetworkProvider.h"
#include "../include/NetworkObserver.h"
#include "../include/NumbersClient.h"
#include "../include/FileOperations.h"
#include "../include/Timer.h"

class ServerLogic : public NetworkObserver, public std::enable_shared_from_this<ServerLogic> {
    public:
        struct Dependencies {
            std::shared_ptr<Logger> logger;
            int waitingPeriodForDumps;
            std::shared_ptr<Timer> timer;
            std::shared_ptr<NetworkProvider> server;
            std::unique_ptr<FileOperations> fileOperations;
            std::function<void(std::function<void()>)> taskPoster;
        };

        ServerLogic(Dependencies&& dependencies);
        void start();
        void onNewNumber(uint64_t number) override;
        void onAverageSquare(uint64_t number, std::shared_ptr<NumbersClient> whoAsked) override;
        void stop();
private:
    void takeSnapShot_after(std::chrono::seconds periodSeconds, std::function<void(void)>&& callback);
    void startSnapshotTimer();

    std::shared_ptr<Logger> logger;
    std::shared_ptr<NetworkProvider> server;
    int waitingPeriodForDumps;
    std::shared_ptr<Timer> timer;

    std::unordered_set<uint64_t> numbersContainer;
    std::shared_ptr<FileOperations> fileOperations;
    bool forceShutdown = false;
    std::function<void(std::function<void()>)> taskPoster;
};