// NetworkServer.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "NetworkServer.h"



NetworkServer::NetworkServer(Dependencies&& deps)
    : acceptor(deps.io_context)
    , socket(deps.io_context)
    , logger(deps.logger)
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
