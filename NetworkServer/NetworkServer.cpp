// Copyright (c) 2025 Michael Alvanos Services LTD
// All rights reserved.
//
// This software is provided "as is" without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.

#include "pch.h"
#include "NetworkServer.h"
#include "Session.h"

NetworkServer::NetworkServer(Dependencies&& deps)
    : acceptor(deps.ioContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 12345))
    , logger(deps.logger)
    , io_context(deps.ioContext)
{
}

NetworkServer::~NetworkServer()
{
    stop();
}

void NetworkServer::start(std::shared_ptr<NetworkObserver> observer)
{
    auto self(shared_from_this());
    boost::asio::post(io_context, [this,self, observer=std::move(observer)]() {
        this->observer = observer;
        startAccepting();
    });
}

void NetworkServer::startAccepting() {
    logger->log(Logger::LogLevel::INFO, "NetworkServer started");

    auto self(shared_from_this());
    acceptor.async_accept(
        [this,self](boost::system::error_code ec, boost::asio::ip::tcp::socket socket)
        {
            if (ec)
            {
                logger->log(Logger::LogLevel::LOGERROR, "Error accepting connection: " + ec.message());
                return;
            }

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
        stopAccepting();
        closeSessions();
    });
}

void NetworkServer::closeSessions() {
    // Close all connections here
}

void NetworkServer::stopAccepting() {
    // Note: this is blocking, for simplicity
    try {
        acceptor.close();
    } catch (const std::exception& e) {
        // Ensure that we continue the execution
        logger->log(Logger::LogLevel::LOGERROR, "Error closing acceptor: " + std::string(e.what()));
    }
}

void NetworkServer::register_session(std::shared_ptr<Session> session) {
    sessions.insert(session);
}

void NetworkServer::unregister_session(std::shared_ptr<Session> session) {
    sessions.erase(session);
}


