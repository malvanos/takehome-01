// Copyright (c) 2025 Michael Alvanos Services LTD
// All rights reserved.
//
// This software is provided "as is" without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.

#pragma once
#include <memory>
#include <boost/asio.hpp>
#include "../include/Logger.h"
#include "../include/NetworkClientProvider.h"

class NetworkClientObserver;

class NetworkClient : 
    public NetworkClientProvider, 
    public std::enable_shared_from_this<NetworkClient>
{
public:
    struct Dependencies {
        boost::asio::io_context& ioContext;
        std::shared_ptr<Logger> logger;
    };
    NetworkClient(Dependencies&& deps);
    ~NetworkClient();
    void start(std::shared_ptr<NetworkClientObserver> networkClientProvider) override;
    void stop() override;
    void sendRandomNumber(uint64_t number) override;
    void sendSumOfSquaresRequest(uint64_t number) override;
private:
    std::shared_ptr<Logger> logger;
    std::shared_ptr<NetworkClientObserver> networkClientProvider;
    boost::asio::io_context& ioContext;
    boost::asio::ip::tcp::socket socket;
    boost::asio::ip::tcp::resolver::results_type endpoints;
};