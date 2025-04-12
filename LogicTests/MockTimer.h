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

#pragma once
#include <string>
#include <memory>
#include "../include/Timer.h"

#include <chrono>
#include <functional>

class MockTimer : public Timer, public std::enable_shared_from_this<MockTimer> {

public:
    void start(std::chrono::seconds periodSeconds, std::function<void(void)>&& callback) override {
        callbacks.push_back(std::move(callback));
        isStartCalled = true;
    }
    void cancel() override {
        // Mock implementation
    }

    bool isStarted() {
        // Mock implementation
        return isStartCalled;
    }

    void triggerAllCallbacks() {
        auto localCallbacks = callbacks;
        // Mock implementation to trigger the callback
        for (auto& callback : localCallbacks) {
            callback();
        }
    }
    void triggerCallback(int index) {
        callbacks[index](); 
    }

private:
    bool isStartCalled = false;
    std::vector <std::function<void(void)>> callbacks;
};