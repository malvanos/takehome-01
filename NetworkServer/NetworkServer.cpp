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

#include "pch.h"
#include "NetworkServer.h"
#include "Session.h"

NetworkServer::NetworkServer(Dependencies&& deps)
    : acceptor(deps.ioContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), PORT_NUMBER))
    , logger(deps.logger)
    , io_context(deps.ioContext)
{
}

void NetworkServer::start(std::shared_ptr<NetworkObserver> observer)
{
    auto self(shared_from_this());
    boost::asio::post(io_context, [this,self, observer=std::move(observer)]() {
        this->observer = observer;
        startAccepting();
        logger->log(Logger::LogLevel::INFO, "NetworkServer started");
    });
}

void NetworkServer::startAccepting() {
    auto self(shared_from_this());
    acceptor.async_accept(
        [this,self](boost::system::error_code ec, boost::asio::ip::tcp::socket socket)
        {
            if (forceShutdown)
            {
                return;
            }
            if (ec)
            {
                logger->log(Logger::LogLevel::LOGERROR, "Error accepting connection: " + ec.message());
                return;
            }

            logger->log(Logger::LogLevel::INFO, "New network connection");
            socket.set_option(forceKeepAliveOption);
            Session::Depedencies deps = {
                .socket = std::move(socket),
                .logger = logger, 
                .server = self,
                .observer = observer,
                .io_context = io_context
            };
            auto session = std::make_shared<Session>(std::move(deps));
            register_session(session);
            session->start();
            startAccepting();
        });
}

void NetworkServer::stop()
{
    auto self(shared_from_this());
    boost::asio::post(io_context, [this, self]() {
        forceShutdown = true;
        stopAccepting();
        closeSessions();
    });
}

void NetworkServer::closeSessions() {
    auto copyOfSessions = std::move(sessions);
    for (const auto& session : copyOfSessions) {
        session->close();
    }
}

void NetworkServer::stopAccepting() {
    // Note: this is blocking, for simplicity
    boost::system::error_code ec;
    acceptor.close(ec);
    if (ec) {
        logger->log(Logger::LogLevel::LOGERROR, "Error closing acceptor: " + ec.message());
    }
}

void NetworkServer::register_session(std::shared_ptr<Session> session) {
    sessions.insert(session);
}

void NetworkServer::unregister_session(std::shared_ptr<Session> session) {
    sessions.erase(session);
}


