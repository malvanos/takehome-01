// Copyright (c) 2025 Michael Alvanos Services LTD
// All rights reserved.
//
// This software is provided "as is" without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.

#include "pch.h"
#include "NetworkClient.h"

NetworkClient::NetworkClient(Dependencies&& deps)
    : logger(std::move(deps.logger))
    , ioContext(deps.ioContext)
    , socket(deps.ioContext)
{
}

NetworkClient::~NetworkClient()
{
}

void NetworkClient::start(std::shared_ptr<NetworkClientObserver> networkClientProvider)
{
    auto self(shared_from_this());
    boost::asio::post(ioContext, [this, self, networkClientProvider = std::move(networkClientProvider)]() {
        this->networkClientProvider = networkClientProvider;
        logger->log(Logger::LogLevel::INFO, "NetworkClient started");
    });
}

void NetworkClient::stop()
{
}

void NetworkClient::sendRandomNumber(uint64_t number)
{
}

void NetworkClient::sendSumOfSquaresRequest(uint64_t number)
{
}
