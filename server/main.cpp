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
#include <thread>


static void runAsioContext(boost::asio::io_context& ctx) {
    ctx.run();
}

int main(int argc, char *argv[])
{

    auto constoleLogger = std::make_unique<ConsoleLogger>();
    auto fileLogger = std::make_unique<FileLogger>();
    auto duplicateLogger = std::make_shared<DuplicateLogger>(std::move(constoleLogger), std::move(fileLogger));

    boost::asio::io_context logicContext;

    Logic::Dependencies deps{
        .logger = duplicateLogger,
        .waitingPeriodForDumps = 10,
        .io_context = logicContext
    };

    auto logic = std::make_unique<Logic>(std::move(deps));
        
    boost::asio::io_context networkContext;

    NetworkServer::Dependencies networkServerDeps{
        .ioContext = networkContext,
        .logger = duplicateLogger
    };

    auto server = std::make_shared<NetworkServer>(std::move(networkServerDeps));

    logic->start(server);

    std::thread thread1(runAsioContext, std::ref(networkContext));
    std::thread thread2(runAsioContext, std::ref(logicContext));

    thread1.join();
    thread2.join();

}

