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

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <memory>
#include <boost/asio.hpp>
#include "../include/Logger.h"
#include "../include/NetworkProvider.h"

#include <functional>
#include <set>

class Session;
class NetworkObserver;

class NetworkServer : 
    public NetworkProvider,
    public std::enable_shared_from_this<NetworkServer>
{
public:
    struct Dependencies {
        boost::asio::io_context& ioContext;
        std::shared_ptr<Logger> logger;
    };

    NetworkServer(Dependencies&& deps);
    void start(std::shared_ptr<NetworkObserver> observer) override;
    void stop() override;

    void register_session(std::shared_ptr<Session> session);
    void unregister_session(std::shared_ptr<Session> session);

private:
    //void sendDataNetwork(const std::vector<char>& message, std::function<void>& callback);
    void startAccepting();
    void stopAccepting();
    void closeSessions();

    boost::asio::ip::tcp::acceptor acceptor;
    std::shared_ptr<Logger> logger;
    boost::asio::io_context& io_context;

    std::set<std::shared_ptr<Session>> sessions;
    std::shared_ptr<NetworkObserver> observer;
    boost::asio::socket_base::keep_alive forceKeepAliveOption =
        boost::asio::socket_base::keep_alive(true);
    bool forceShutdown = false;
};