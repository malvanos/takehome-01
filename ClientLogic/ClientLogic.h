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

#include <memory>
#include <random>
#include "../include/Logger.h"
#include "../include/NetworkClientProvider.h"
#include "../include/NetworkClientObserver.h"
#include "../include/Timer.h"

class ClientLogic : 
    public NetworkClientObserver,
    public std::enable_shared_from_this<ClientLogic>
{
public:
    struct Dependencies {
        std::shared_ptr<Timer> timer;
        std::shared_ptr<Logger> logger;
        std::shared_ptr<NetworkClientProvider> networkClientProvider;
        std::function<void(std::function<void()>)> taskPoster;
    };

    ClientLogic(Dependencies&& deps);
    void start();
    void sendSumOfSquaresRequest();
    void stop();
    void onSumOfSquaresResponse(uint64_t sum) override;
    void onDisconnect() override;
    void onConnect() override;

private:

    void sendRandomNumber();
    void start_random_number_timer();
    void send_random_number_after(std::chrono::seconds periodSeconds, std::function<void(void)>&& callback);

    std::shared_ptr<Logger> logger;
    std::shared_ptr<NetworkClientProvider> networkClientProvider;

    std::mt19937 randomNumberGenerator;
    std::uniform_int_distribution<uint64_t> randomNumberDistribution{ 0, 1023 };
    std::shared_ptr<Timer> timer;

    const int waitingPeriodForSendinNumber = 1;
    bool forceShutdown = false;
    bool transmitNumbers = false;
    std::function<void(std::function<void()>)> taskPoster;
};