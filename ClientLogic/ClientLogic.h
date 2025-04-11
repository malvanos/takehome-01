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
#include <boost/asio.hpp>
#include "../include/Logger.h"
#include "../include/NetworkClientProvider.h"
#include "../include/NetworkClientObserver.h"

class ClientLogic : 
    public NetworkClientObserver,
    public std::enable_shared_from_this<ClientLogic>
{
public:
    struct Dependencies {
        boost::asio::io_context& ioContext;
        std::shared_ptr<Logger> logger;
        std::shared_ptr<NetworkClientProvider> networkClientProvider;
    };

    ClientLogic(Dependencies&& deps);
    ~ClientLogic();
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
    boost::asio::io_context& ioContext;

    std::mt19937 randomNumberGenerator;
    std::uniform_int_distribution<uint64_t> randomNumberDistribution{ 0, 1023 };
    boost::asio::steady_timer timer;

    const int waitingPeriodForSendinNumber = 1;
    bool forceShutdown = false;
    bool transmitNumbers = false;
};