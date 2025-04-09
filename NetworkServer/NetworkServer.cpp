// Copyright (c) 2025 Michael Alvanos Services LTD
// All rights reserved.
//
// This software is provided "as is" without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.

#include "pch.h"
#include "NetworkServer.h"


NetworkServer::NetworkServer(Dependencies&& deps)
    : acceptor(deps.ioContext)
    , socket(deps.ioContext)
    , logger(deps.logger)
    , io_context(deps.ioContext)
{
}

NetworkServer::~NetworkServer()
{
}

void NetworkServer::start()
{
    logger->log(Logger::LogLevel::INFO, "NetworkServer started");
    // Start the server logic here
}

void NetworkServer::stop()
{
    // Stop the server logic here
}


void NetworkServer::send(const std::vector<char>& message, std::function<void>&& callback) {
        
}

