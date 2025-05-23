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

#include <conio.h> // For _kbhit() and _getch()
#include "../include/AsioTimer.h"

constexpr int KEY_ESCAPE = 27;

static void runAsioContext(boost::asio::io_context& ctx) {
    ctx.run();
}

int main(int argc, char* argv[])
{
    // Logger
    auto logger = std::make_shared<DuplicateLogger>(
        std::make_unique<ConsoleLogger>(),
        std::make_unique<FileLogger>()
    );

    // Network client
    boost::asio::io_context networkContext;
    NetworkClient::Dependencies NetworkClientDeps{
        .ioContext = networkContext,
        .logger = logger
    };
    auto client = std::make_shared<NetworkClient>(std::move(NetworkClientDeps));

    // Logic
    boost::asio::io_context logicContext;
    auto asioTaskPoster = [&logicContext](std::function<void()>&& task) {
        boost::asio::post(logicContext, std::move(task));
        };
    auto timer = std::make_shared<AsioTimer>(logicContext);
    ClientLogic::Dependencies deps{
        .timer = std::move(timer),
        .logger = logger,
        .networkClientProvider = std::move(client),
        .taskPoster = std::move(asioTaskPoster)
    };
    auto logic = std::make_shared<ClientLogic>(std::move(deps));

    logic->start();

    std::thread networkThread(runAsioContext, std::ref(networkContext));
    std::thread logicThread(runAsioContext, std::ref(logicContext));

    logger->info("Press ESC to close");
    logger->info("Press '1' to request sum of squares");
    while (true) {
        if (_kbhit()) {
            int key = _getch();
            if (key == KEY_ESCAPE) { // ASCII code for Escape key
                logger->info("Received ESC. Closing.");
                logic->stop();
                break;
            }
            else if (key == '1') {
                logger->info("Received '1'. Requesting sum of squares.");
                logic->sendSumOfSquaresRequest();
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    networkThread.join();
    logicThread.join();

}

