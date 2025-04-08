// Copyright (c) 2025 Michael Alvanos Services LTD
// All rights reserved.
//
// This software is provided "as is" without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.

#include <iostream>
#include <memory>
#include "../logger/ConsoleLogger.h"
#include "../logger/FileLogger.h"
#include "../logger/DuplicateLogger.h"
#include "../logic/Logic.h"
#include "../include/Logger.h"
#include "../NetworkServer/NetworkServer.h"
#include <string>

#include <boost/asio.hpp>

int main(int argc, char *argv[])
{

    auto constoleLogger = std::make_unique<ConsoleLogger>();
    auto fileLogger = std::make_unique<FileLogger>();
    auto duplicateLogger = std::make_shared<DuplicateLogger>(std::move(constoleLogger), std::move(fileLogger));

    Logic::Dependencies deps{
        .logger = duplicateLogger,
        .waitingPeriodForDumps = 10
    };

    auto logic = std::make_unique<Logic>(std::move(deps));
        
    boost::asio::io_context io_context;

    NetworkServer::Dependencies networkServerDeps{
        .io_context = io_context,
        .logger = duplicateLogger
    };

    NetworkServer server(std::move(networkServerDeps));
    server.start();

    io_context.run();

}

