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

#include "pch.h"
#include "CppUnitTest.h"
#include "../clientLogic/ClientLogic.h"
#include "MockLogger.h"
#include "MockTimer.h"
#include "MockFileOperations.h"
#include "MockNumbersClient.h"
#include "MockNetworkClientProvider.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace LogicTests
{
    TEST_CLASS(ClientLogicTests)
    {
    private:
        std::shared_ptr<ClientLogic> createLogic(
            std::shared_ptr<MockLogger> logger,
            std::shared_ptr<MockTimer> timer,
            std::shared_ptr<MockFileOperations> fileOperations,
            std::shared_ptr<MockNetworkClientProvider> networkClient)
        {
            ClientLogic::Dependencies deps{
                .timer = timer,
                .logger = logger,
                .networkClientProvider = networkClient,
                .taskPoster = [](std::function<void()>&& f) { f(); }
            };
            return std::make_shared<ClientLogic>(std::move(deps));
        }

        template<typename... Ts>
        auto createMocks()
        {
            return std::make_tuple(std::make_shared<Ts>()...);
        }

    public:

        TEST_METHOD(WhenInitialiaze_shouldNotThrow)
        {
            auto [logger, timer, fileOperations, networkClient] =
                createMocks<MockLogger, MockTimer, MockFileOperations, MockNetworkClientProvider>();

            auto logic = createLogic(logger, timer, std::move(fileOperations), networkClient);
        }

        TEST_METHOD(WhenStartCalled_startNetworkServerAndTimer) {
            auto [logger, timer, fileOperations, networkClient] =
                createMocks<MockLogger, MockTimer, MockFileOperations, MockNetworkClientProvider>();
            auto logic = createLogic(logger, timer, std::move(fileOperations), networkClient);
            logic->start();
            Assert::IsTrue(networkClient->isStarted());
            Assert::IsTrue(timer->isStarted());
        }

        TEST_METHOD(WhenTimerExpires_sendToNetworkClient) {
            auto [logger, timer, fileOperations, networkClient] =
                createMocks<MockLogger, MockTimer, MockFileOperations, MockNetworkClientProvider>();
            auto logic = createLogic(logger, timer, std::move(fileOperations), networkClient);
            logic->start();
            logic->onConnect();
            timer->triggerCallback(0);
            Assert::AreEqual(1ull, networkClient->getTimesCalledSentRandomNumber());
        }

        TEST_METHOD(WhenSendSumOfSquaresRequestCalled_sendToNetworkClient) {
            auto [logger, timer, fileOperations, networkClient] =
                createMocks<MockLogger, MockTimer, MockFileOperations, MockNetworkClientProvider>();
            auto logic = createLogic(logger, timer, std::move(fileOperations), networkClient);
            logic->start();
            logic->onConnect();
            logic->sendSumOfSquaresRequest();
            timer->triggerCallback(0);
            Assert::AreEqual(1ull, networkClient->getTimesCalledSumOfSquaresRequest());
        }
    };
}
