// Copyright (c) 2025 Michael Alvanos Services LTD
// All rights reserved.
//
// This software is provided "as is" without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.

#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <array>
#include <functional>
#include <deque>

#include "../include/Logger.h"

#include "Protocol.h"

class NetworkServer;

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(
        boost::asio::ip::tcp::socket socket, 
        std::shared_ptr<Logger> logger,
        std::shared_ptr<NetworkServer> server
    );
    ~Session();
    void start();
    void close();
    void send(uint64_t sumOfSquares);
private:
    void read();
    void write(uint64_t sumOfSquares);
    bool shouldTransmit();
    boost::asio::ip::tcp::socket socket;
    std::shared_ptr<Logger> logger;
    std::shared_ptr<NetworkServer> serverAcceptor;

    char receiveData[sizeof(NetworkPacket)] = {0};

    std::deque<uint64_t> dataToSend;
    bool transmitting = false;
    NetworkPacket transmitingPacket;
};