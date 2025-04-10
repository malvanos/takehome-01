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

#include "../include/NetworkObserver.h"

Session::Session(Depedencies deps)
    : socket(std::move(deps.socket))
    , logger(std::move(deps.logger))
    , serverAcceptor(std::move(deps.server))
    , observer(std::move(deps.observer))
    , io_context(deps.io_context)
{
}

Session::~Session()
{
    close();
}

void Session::start()
{
    auto self(shared_from_this());
    boost::asio::post(io_context, [this, self]() {
        read();
        if (shouldTransmit()) {
            auto sumOfSquares = dataToSend.front();
            dataToSend.pop_front();
            write(sumOfSquares);
        }
    });

}

void Session::read() {
    auto self(shared_from_this());
    socket.async_receive(
        boost::asio::buffer(&receivePacketData,sizeof(receivePacketData)),
        [this,self](boost::system::error_code ec, std::size_t length)
        {
            if (forceShutdown) {
                return;
            }

            if (ec) {
                logger->log(Logger::LogLevel::LOGERROR, "Error receiving message: " + ec.message());
                return;
            }
            
            if (!validatePacket(receivePacketData)) {
                logger->log(Logger::LogLevel::LOGERROR, "Invalid hash check.");
                close();
            }
            
            switch (receivePacketData.packetType) {
            case  PacketType::DATA_ADD:
                observer->onNewNumber(receivePacketData.data);
                break;
            case  PacketType::SUM_OF_SQUARES_REQUEST:
                observer->onAverageSquare(receivePacketData.data,self);
                break;
            case  PacketType::INVALID:
            default:
                logger->log(Logger::LogLevel::LOGERROR, "Invalid packet type.");
                close();
                return;
            }

            read();
        });
}

void Session::close() {
    auto self(shared_from_this());
    boost::asio::post(io_context, [this, self]() {
        forceShutdown = true;
        closeConnection();
    });
}

void Session::closeConnection() {
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
    }
    catch (const std::exception& e) {
        logger->log(Logger::LogLevel::LOGERROR, "Exception while closing socket: " + std::string(e.what()));
    }
}

bool Session::shouldTransmit() {
    if (dataToSend.size() > 0 and !transmitting) {
        return true;
    }
    return false;
}

void Session::send(uint64_t sumOfSquares)
{
    if (shouldTransmit()) {
        write(sumOfSquares);
    } else {
        dataToSend.push_back(sumOfSquares);
    }
}

void Session::write(uint64_t sumOfSquares) {
    auto self(shared_from_this());

    fillPacket(
        transmitingPacket,
        PacketType::SUM_OF_SQUARES_ANSWER, 
        sumOfSquares
    );
    
    transmitting = true;

    boost::asio::async_write(socket, boost::asio::buffer(&transmitingPacket, sizeof(transmitingPacket)),
        [this, self](boost::system::error_code ec, std::size_t /*length*/)
        {
            transmitting = false;
            if (forceShutdown) {
                return;
            }
            if (ec)
            {
                logger->log(Logger::LogLevel::LOGERROR, "Error sending message: " + ec.message());
                return;
            }

            if (shouldTransmit()) {
                auto sumOfSquares = dataToSend.front();
                dataToSend.pop_front();
                write(sumOfSquares);
            }

        });
}
