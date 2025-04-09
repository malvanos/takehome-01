// Copyright (c) 2025 Michael Alvanos Services LTD
// All rights reserved.
//
// This software is provided "as is" without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.

#include "pch.h"

#include "Session.h"
#include "../include/Logger.h"
#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <functional>
#include <utility>
#include <stdexcept>

Session::Session(
    boost::asio::ip::tcp::socket socket,
    std::shared_ptr<Logger> logger, 
    std::shared_ptr<NetworkServer> server)
    : socket(std::move(socket))
    , logger(std::move(logger))
    , serverAcceptor(std::move(server))
{
}

Session::~Session()
{
    close();
}

void Session::start()
{
    read();
}

void Session::read() {
    socket.async_receive(
        boost::asio::buffer(data,16),
        [this](boost::system::error_code ec, std::size_t length)
        {
            if (ec) {
                logger->log(Logger::LogLevel::LOGERROR, "Error receiving message: " + ec.message());
                return;
            }
            // Process the received message
            // TODO: add process here



            read();
        });
}

void Session::close() {
    try {
        // TODO: shutdown can wait for ever for tcp to close
        boost::system::error_code ec;
        socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
        if (ec)
        {
            logger->log(Logger::LogLevel::LOGERROR, "Error shutting down socket: " + ec.message());
        }
        socket.close(ec);
        if (ec)
        {
            logger->log(Logger::LogLevel::LOGERROR, "Error closing socket: " + ec.message());
        }
    } catch (const std::exception& e) {
        logger->log(Logger::LogLevel::LOGERROR, "Exception while closing socket: " + std::string(e.what()));
    }
}

void Session::send(const std::vector<char>& message, std::function<void(void)>&& callback)
{
    // TODO: add queue for sending messages
    auto self(shared_from_this());
    boost::asio::async_write(socket, boost::asio::buffer(message),
        [this, self, callback = std::move(callback)](boost::system::error_code ec, std::size_t /*length*/)
        {
            if (ec)
            {
                logger->log(Logger::LogLevel::LOGERROR, "Error sending message: " + ec.message());
                return;
            }
            callback();
        });
}
