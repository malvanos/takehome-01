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
#include "../serverLogic/ServerLogic.h"
#include "../include/Logger.h"
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
    auto constoleLogger = std::make_unique<ConsoleLogger>();
    auto fileLogger = std::make_unique<FileLogger>();
    auto duplicateLogger = std::make_shared<DuplicateLogger>(std::move(constoleLogger), std::move(fileLogger));


    int waitingPeriodForDumps = 10;

    for (int i = 1; i < argc - 1; ++i) {
        std::string arg = argv[i];
        if (arg == "-N") {
            try {
                waitingPeriodForDumps = std::stoi(argv[i + 1]);
                break;
            }
            catch (const std::exception& e) {
                duplicateLogger->log(Logger::LogLevel::LOGERROR, "Invalid number for -N: " + std::string(argv[i + 1]));
                duplicateLogger->log(Logger::LogLevel::LOGERROR, "Setting default value 10.");
                waitingPeriodForDumps = 10;
            }
        }
    }

    duplicateLogger->info("N seconds to take a dump is set to: " + std::to_string(waitingPeriodForDumps));

    // File operations
    boost::asio::io_context fileSystemOperationsContext;
    SystemFileOperations::Dependencies filedeps{
        .ioContext = fileSystemOperationsContext,
        .logger = duplicateLogger
    };

    auto fileOperations = std::make_unique<SystemFileOperations>(std::move(filedeps));

    // Network server
    boost::asio::io_context networkContext;
    NetworkServer::Dependencies networkServerDeps{
        .ioContext = networkContext,
        .logger = duplicateLogger
    };
    auto server = std::make_shared<NetworkServer>(std::move(networkServerDeps));

    // Logic
    boost::asio::io_context logicContext;
    ServerLogic::Dependencies deps{
        .logger = duplicateLogger,
        .waitingPeriodForDumps = 10,
        .io_context = logicContext,
        .server = server,
        .fileOperations = std::move(fileOperations)
    };
    auto logic = std::make_shared<ServerLogic>(std::move(deps));

    logic->start();

    std::thread thread1(runAsioContext, std::ref(networkContext));
    std::thread thread2(runAsioContext, std::ref(logicContext));
    std::thread thread3(runAsioContext, std::ref(fileSystemOperationsContext));

    duplicateLogger->info("Press ESC to close");
    while (true) {
        if (_kbhit()) {
            int key = _getch();
            if (key == 27) { // ASCII code for Escape key
                duplicateLogger->info("Received ESC. Closing.");
                logic->stop();
                break;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    thread1.join();
    thread2.join();
    thread3.join();
   
}

