// Copyright (c) 2025 Michael Alvanos Services LTD
// All rights reserved.
//
// This software is provided "as is" without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// This software is the property of Michael Alvanos Services LTD.
// Redistribution or reproduction, in whole or in part, is not permitted
// without explicit prior written permission from the owner.

#include <iostream>
#include <memory>
#include "../logger/ConsoleLogger.h"
#include "../logger/FileLogger.h"
#include "../logger/DuplicateLogger.h"
#include "../serverLogic/ServerLogic.h"
#include "../include/Logger.h"
#include "../include/AsioTimer.h"
#include "../NetworkServer/NetworkServer.h"
#include "../FileOperations/SystemFileOperations.h"
#include <string>

#include <boost/asio.hpp>
#include <thread>

#include <conio.h> // For _kbhit() and _getch() on Windows

static void runAsioContext(boost::asio::io_context& ctx) {
    ctx.run();
}

int main(int argc, char *argv[])
{
    // Logger
    auto logger = std::make_shared<DuplicateLogger>(
        std::make_unique<ConsoleLogger>(),
        std::make_unique<FileLogger>()
    );

    int waitingPeriodForDumps = 10;

    for (int i = 1; i < argc - 1; ++i) {
        std::string arg = argv[i];
        if (arg == "-N") {
            try {
                waitingPeriodForDumps = std::stoi(argv[i + 1]);
                break;
            }
            catch (const std::exception& e) {
                logger->log(Logger::LogLevel::LOGERROR, "Invalid number for -N: " + std::string(argv[i + 1]));
                logger->log(Logger::LogLevel::LOGERROR, "Setting default value 10.");
                waitingPeriodForDumps = 10;
            }
        }
    }

    logger->info("N seconds to take a dump is set to: " + std::to_string(waitingPeriodForDumps));

    // File operations
    boost::asio::io_context fileSystemOperationsContext;
    SystemFileOperations::Dependencies filedeps{
        .ioContext = fileSystemOperationsContext,
        .logger = logger
    };

    auto fileOperations = std::make_shared<SystemFileOperations>(std::move(filedeps));

    // Network server
    boost::asio::io_context networkContext;
    NetworkServer::Dependencies networkServerDeps{
        .ioContext = networkContext,
        .logger = logger
    };
    auto server = std::make_shared<NetworkServer>(std::move(networkServerDeps));

    // Logic
    boost::asio::io_context logicContext;
    auto asioTaskPoster = [&logicContext](std::function<void()>&& task) {
        boost::asio::post(logicContext, std::move(task));
    };
    auto timer = std::make_shared<AsioTimer>(logicContext);
    ServerLogic::Dependencies deps{
        .logger = logger,
        .waitingPeriodForDumps = 10,
        .timer = std::move(timer),
        .server = server,
        .fileOperations = std::move(fileOperations),
        .taskPoster = std::move(asioTaskPoster)
    };
    auto logic = std::make_shared<ServerLogic>(std::move(deps));

    logic->start();

    std::thread networkThread(runAsioContext, std::ref(networkContext));
    std::thread logicThread(runAsioContext, std::ref(logicContext));
    std::thread filesystemThread(runAsioContext, std::ref(fileSystemOperationsContext));

    logger->info("Press ESC to close");
    while (true) {
        if (_kbhit()) {
            int key = _getch();
            if (key == 27) { // ASCII code for Escape key
                logger->info("Received ESC. Closing.");
                logic->stop();
                break;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    networkThread.join();
    logicThread.join();
    filesystemThread.join();
   
}

