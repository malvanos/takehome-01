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

#include <boost/asio.hpp>
#include <memory>
#include <array>
#include <functional>
#include <deque>

#include "../include/Logger.h"
#include "../include/NumbersClient.h"
#include "../include/Protocol.h"

class NetworkServer;
class NetworkObserver;

class Session :
    public NumbersClient,
    public std::enable_shared_from_this<Session> {
public:

    struct Depedencies {
        boost::asio::ip::tcp::socket socket;
        std::shared_ptr<Logger> logger;
        std::shared_ptr<NetworkServer> server;
        std::shared_ptr<NetworkObserver> observer;
        boost::asio::io_context& io_context;
    };

    Session(Depedencies deps);
    void start();
    void close();
    void send(uint64_t sumOfSquares) override;
private:
    void read();
    void write(uint64_t sumOfSquares);
    void closeConnection();
    boost::asio::ip::tcp::socket socket;
    std::shared_ptr<Logger> logger;
    std::shared_ptr<NetworkServer> serverAcceptor;

    NetworkPacket receivePacketData;

    std::deque<uint64_t> dataToSend;
    bool transmitting = false;
    NetworkPacket transmitingPacket;
    std::shared_ptr<NetworkObserver> observer;
    boost::asio::io_context& io_context;
    bool forceShutdown = false;
};