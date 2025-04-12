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