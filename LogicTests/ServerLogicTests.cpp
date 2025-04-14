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

#include "pch.h"
#include "CppUnitTest.h"
#include "../serverLogic/ServerLogic.h"
#include "MockLogger.h"
#include "MockTimer.h"
#include "MockFileOperations.h"
#include "MockNetworkProvider.h"
#include "MockNumbersClient.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace LogicTests
{
	TEST_CLASS(ServerLogicTests)
	{
    private:
        std::shared_ptr<ServerLogic> createLogic(
            std::shared_ptr<MockLogger> logger,
            std::shared_ptr<MockTimer> timer,
            std::shared_ptr<MockFileOperations> fileOperations,
            std::shared_ptr<MockNetworkProvider> networkServer)
        {
            ServerLogic::Dependencies deps{
                .logger = logger,
                .waitingPeriodForDumps = 10,
                .timer = timer,
                .server = networkServer,
                .fileOperations = std::move(fileOperations),
                .taskPoster = [](std::function<void()>&& f) { f(); }
            };
            return std::make_shared<ServerLogic>(std::move(deps));
        }   

        template<typename... Ts>
        auto createMocks()
        {
            return std::make_tuple(std::make_shared<Ts>()...);
        }

	public:
		
		TEST_METHOD(WhenInitialiaze_shouldNotThrow)
		{
            auto [logger, timer, fileOperations, networkServer] =
                createMocks<MockLogger, MockTimer, MockFileOperations, MockNetworkProvider>();

            auto logic = createLogic(logger, timer, std::move(fileOperations), networkServer);
		}

        TEST_METHOD(WhenStartCalled_startNetworkServerAndTimer) {
            auto [logger, timer, fileOperations, networkServer] =
                createMocks<MockLogger, MockTimer, MockFileOperations, MockNetworkProvider>();
            auto logic = createLogic(logger, timer, std::move(fileOperations), networkServer);
            logic->start();
            Assert::IsTrue(networkServer->isStarted());
            Assert::IsTrue(timer->isStarted());
        }

        TEST_METHOD(WhenOnNewNumberCalled_insertToContainer) {
            auto [logger, timer, fileOperations, networkServer] =
                createMocks<MockLogger, MockTimer, MockFileOperations, MockNetworkProvider>();
            auto logic = createLogic(logger, timer, fileOperations, networkServer);
            logic->start();
            
            logic->onNewNumber(667);
            logic->onNewNumber(997);
            logic->onNewNumber(47);

            timer->triggerCallback(0);

            auto savedNumbers = fileOperations->getSavedNumbers();
            Assert::AreEqual(3, (int)savedNumbers.size());
            Assert::IsTrue(savedNumbers.contains(667));
            Assert::IsTrue(savedNumbers.contains(997));
            Assert::IsTrue(savedNumbers.contains(47));
        }

        TEST_METHOD(WhenOnonAverageSquareCalled_ReturnTheSumOfSquares) {
            auto [logger, timer, fileOperations, networkServer] =
                createMocks<MockLogger, MockTimer, MockFileOperations, MockNetworkProvider>();
            auto logic = createLogic(logger, timer, fileOperations, networkServer);
            logic->start();
            auto client = std::make_shared<MockNumbersClient>();

            logic->onAverageSquare(5, client);
            Assert::AreEqual(25ull, client->getReceivedSumOfSquares());

            logic->onNewNumber(10);
            logic->onAverageSquare(6, client);
            Assert::AreEqual(136ull, client->getReceivedSumOfSquares());
        }

        TEST_METHOD(WhenOnonAverageSquareCalled_DoNotStoreTheNumberOnContainer) {
            auto [logger, timer, fileOperations, networkServer] =
                createMocks<MockLogger, MockTimer, MockFileOperations, MockNetworkProvider>();
            auto logic = createLogic(logger, timer, fileOperations, networkServer);
            logic->start();
            auto client = std::make_shared<MockNumbersClient>();

            logic->onAverageSquare(5, client);
            timer->triggerCallback(0);
            Assert::AreEqual(25ull, client->getReceivedSumOfSquares());

            auto savedNumbers = fileOperations->getSavedNumbers();
            Assert::AreEqual(0llu, savedNumbers.size());

            logic->onNewNumber(10);
            logic->onAverageSquare(6, client);
            timer->triggerCallback(0);
            Assert::AreEqual(136ull, client->getReceivedSumOfSquares());

            savedNumbers = fileOperations->getSavedNumbers();
            Assert::AreEqual(1llu, savedNumbers.size());
            Assert::IsTrue(savedNumbers.contains(10));
        }
	};
}
