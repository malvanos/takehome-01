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

#pragma once
#include <string>
#include <memory>

#include "../include/NetworkProvider.h"
#include "../include/NetworkObserver.h"
#include "../include/NumbersClient.h"

class MockNetworkProvider : public NetworkProvider, public std::enable_shared_from_this<MockNetworkProvider> {
public:
    void start(std::shared_ptr<NetworkObserver> observer) override {
        // Mock implementation
        isStartCalled = true;
    }
    void stop() override {
        // Mock implementation
    }
    bool isStarted() {
        // Mock implementation
        return isStartCalled;
    }

private:
    bool isStartCalled = false;
};