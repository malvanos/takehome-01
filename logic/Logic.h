// Copyright (c) 2025 Michael Alvanos Services LTD
// All rights reserved.
//
// This software is provided "as is" without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.

#pragma once

#include "../include/Logger.h"
#include <memory>
#include <unordered_set>
#include "boost/asio/steady_timer.hpp"
#include "boost/asio/io_context.hpp"
#include "../include/NetworkProvider.h"
#include "../include/NetworkObserver.h"

class Logic : public NetworkObserver, std::enable_shared_from_this<Logic> {
    public:
        struct Dependencies {
            std::shared_ptr<Logger> logger;
            int waitingPeriodForDumps;
            boost::asio::io_context& io_context;
        };

        Logic(Dependencies&& dependencies);
        ~Logic();
        void start(std::shared_ptr<NetworkProvider> server);
        void onNewNumber(int number) override;
        void onAverageSquare(int number, std::shared_ptr<NetworkProvider> whoAsked) override;        
        void onNetworkStop() override;
        void stop();
private:
    void take_snapshot_after(std::chrono::seconds periodSeconds, std::function<void(void)>&& callback);
    void start_snapshot_timer();

    std::shared_ptr<Logger> logger;
    std::shared_ptr<NetworkProvider> server;
    int waitingPeriodForDumps;
    boost::asio::steady_timer timer;

    std::unordered_set<unsigned short> numbersContainer;
    boost::asio::io_context& ioContext;
};