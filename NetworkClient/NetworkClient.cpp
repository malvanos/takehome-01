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
#include "NetworkClient.h"
#include "../include/NetworkClientObserver.h"

NetworkClient::NetworkClient(Dependencies&& deps)
    : logger(std::move(deps.logger))
    , ioContext(deps.ioContext)
    , socket(deps.ioContext)
    , resolver(deps.ioContext)
{
}

void NetworkClient::start(std::shared_ptr<NetworkClientObserver> networkClientProvider)
{
    auto self(shared_from_this());
    boost::asio::post(ioContext, [this, self, networkClientProvider = std::move(networkClientProvider)]() {
        this->observer = networkClientProvider;
        resolve();
        logger->log(Logger::LogLevel::INFO, "NetworkClient started");
    });
}

void NetworkClient::stop()
{
    auto self(shared_from_this());
    boost::asio::post(ioContext, [this, self]() {
        forceShutdown = true;
        closeConnection();
    });
}

void NetworkClient::resolve()
{
    auto self = shared_from_this();
    resolver.async_resolve(TARGET_HOST, PORT_NUMBER_STRING,
        [this, self](const boost::system::error_code& ec, boost::asio::ip::tcp::resolver::results_type results) {
            if (forceShutdown) {
                return;
            }
            if (ec) {
                logger->log(Logger::LogLevel::LOGERROR, "Error resolving address: " + ec.message());
                return;
            }
            logger->log(Logger::LogLevel::INFO, "Resolved address");
            connect(std::move(results));
        });
}

void NetworkClient::connect(boost::asio::ip::tcp::resolver::results_type&& result)
{
    auto self = shared_from_this();
    boost::asio::async_connect(socket, std::move(result),
        [this, self](const boost::system::error_code& ec, const boost::asio::ip::tcp::endpoint&) {
            if (forceShutdown) {
                return;
            }
            if (ec) {
                logger->log(Logger::LogLevel::LOGERROR, "Error connecting to server: " + ec.message());
                isConnected = false;
                return;
            }

            logger->log(Logger::LogLevel::INFO, "Connected to server");
            isConnected = true;
            observer->onConnect();
            read();
        });
}

void NetworkClient::read()
{
    auto self(shared_from_this());
    socket.async_receive(
        boost::asio::buffer(&receivedPacketData, sizeof(receivedPacketData)),
        [this, self](boost::system::error_code ec, std::size_t length)
        {
            if (forceShutdown) {
                return;
            }

            if (ec) {
                logger->log(Logger::LogLevel::LOGERROR, "Error receiving message: " + ec.message());
                isConnected = false;
                closeConnection();
                return;
            }

            if (!validatePacket(receivedPacketData)) {
                logger->log(Logger::LogLevel::LOGERROR, "Invalid hash check.");
                closeConnection();
            }

            switch (receivedPacketData.packetType) {
            case  PacketType::SUM_OF_SQUARES_ANSWER:
                observer->onSumOfSquaresResponse(receivedPacketData.data);
                break;
            default:
                logger->log(Logger::LogLevel::LOGERROR, "Invalid packet type.");
                closeConnection();
                return;
            }

            read();
        });
}

void NetworkClient::sendRandomNumber(uint64_t number)
{
    auto self(shared_from_this());
    boost::asio::post(ioContext, [this, self, number]() {
        enqueueOrTransmit(number, PacketType::DATA_ADD);
    });
}

void NetworkClient::sendSumOfSquaresRequest(uint64_t number)
{
    auto self(shared_from_this());
    boost::asio::post(ioContext, [this, self, number]() {
        enqueueOrTransmit(number, PacketType::SUM_OF_SQUARES_REQUEST);
    });
}

void NetworkClient::enqueueOrTransmit(uint64_t number, PacketType type) 
{
    if (forceShutdown) {
        return;
    }
    if (isConnected) {
        if (!transmitting) {
            transmitingPacket = createPacket(type, number);
            write();
        }
        else {
            dataToSend.push_back(createPacket(type,number));
        }
    }
    else {
        logger->log(Logger::LogLevel::LOGERROR, "NetworkClient not connected");
    }
}

void NetworkClient::write()
{
    auto self(shared_from_this());
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
                isConnected = false;
                return;
            }

            if (!dataToSend.empty()) {
                transmitingPacket = dataToSend.front();
                dataToSend.pop_front();
                write();
            }

        });
}

void NetworkClient::requestSumSquares(uint64_t number)
{
}

void NetworkClient::closeConnection()
{
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
    isConnected = false;
    observer->onDisconnect();
}

