// Copyright (c) 2025 Michail Alvanos
// All rights reserved.
//
// This software is provided "as is" without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// This software is the property of Michael Alvanos.
// Redistribution or reproduction, in whole or in part, is not permitted
// without explicit prior written permission from the owner.

#pragma once
#include <memory>
#include <boost/asio.hpp>
#include "../include/Logger.h"
#include "../include/NetworkClientProvider.h"
#include "../include/Protocol.h"
#include <deque>

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
    void start(std::shared_ptr<NetworkClientObserver> networkClientProvider) override;
    void stop() override;
    void sendRandomNumber(uint64_t number) override;
    void sendSumOfSquaresRequest(uint64_t number) override;
private:

    void enqueueOrTransmit(uint64_t number, PacketType type);
    void resolve();
    void connect(boost::asio::ip::tcp::resolver::results_type&& result);
    void read();
    void write();
    void requestSumSquares(uint64_t number);
    void closeConnection();

    std::shared_ptr<Logger> logger;
    std::shared_ptr<NetworkClientObserver> observer;
    boost::asio::io_context& ioContext;
    boost::asio::ip::tcp::socket socket;
    boost::asio::ip::tcp::resolver resolver;
    boost::asio::ip::tcp::resolver::results_type endpoints;
    NetworkPacket receivedPacketData;
    std::deque<NetworkPacket> dataToSend;
    NetworkPacket transmitingPacket;
    bool isConnected = false;
    bool transmitting = false;
    bool forceShutdown = false;

};