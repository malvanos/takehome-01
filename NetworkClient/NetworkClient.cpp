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
    , networkClientProvider(std::move(deps.networkClientProvider))
{
}

NetworkClient::~NetworkClient()
{
}

void NetworkClient::start()
{
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
