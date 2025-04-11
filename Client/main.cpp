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
#include "../ClientLogic/ClientLogic.h"
#include "../include/Logger.h"
#include "../NetworkClient/NetworkClient.h"
#include <string>

#include <boost/asio.hpp>
#include <thread>

#include <conio.h> // For _kbhit() and _getch() on Windows

static void runAsioContext(boost::asio::io_context& ctx) {
    ctx.run();
}

int main(int argc, char* argv[])
{
    // Logger
    auto constoleLogger = std::make_unique<ConsoleLogger>();
    auto fileLogger = std::make_unique<FileLogger>();
    auto duplicateLogger = std::make_shared<DuplicateLogger>(std::move(constoleLogger), std::move(fileLogger));

    // Network client
    boost::asio::io_context networkContext;
    NetworkClient::Dependencies NetworkClientDeps{
        .ioContext = networkContext,
        .logger = duplicateLogger
    };
    auto client = std::make_shared<NetworkClient>(std::move(NetworkClientDeps));

    // Logic
    boost::asio::io_context logicContext;
    ClientLogic::Dependencies deps{
        .ioContext = logicContext,
        .logger = duplicateLogger,
        .networkClientProvider = std::move(client)
    };
    auto logic = std::make_shared<ClientLogic>(std::move(deps));

    logic->start();

    std::thread thread1(runAsioContext, std::ref(networkContext));
    std::thread thread2(runAsioContext, std::ref(logicContext));

    duplicateLogger->info("Press ESC to close");
    duplicateLogger->info("Press '1' to request sum of squares");
    while (true) {
        if (_kbhit()) {
            int key = _getch();
            if (key == 27) { // ASCII code for Escape key
                duplicateLogger->info("Received ESC. Closing.");
                logic->stop();
                break;
            }
            else if (key == '1') {
                duplicateLogger->info("Received '1'. Requesting sum of squares.");
                logic->sendSumOfSquaresRequest();
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    thread1.join();
    thread2.join();

}

