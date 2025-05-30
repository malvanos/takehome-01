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
#include "../include/NetworkClientProvider.h"
#include "../include/NetworkClientObserver.h"


class MockNetworkClientProvider : public NetworkClientProvider, public std::enable_shared_from_this<MockNetworkClientProvider> {
public:
    void start(std::shared_ptr<NetworkClientObserver> networkClientProvider) override {
        // Mock implementation
        isStartCalled = true;
    }
    void stop() override {
        // Mock implementation
    }
    void sendRandomNumber(uint64_t number) override {
        // Mock implementation
        timesCalledSentRandomNumber++;
        sentRandomNumber = number;
    }
    void sendSumOfSquaresRequest(uint64_t number) override {
        // Mock implementation
        timesCalledSumOfSquaresRequest++;
        sentSumOfSquaresRequest = number;
    }
    bool isStarted() {
        return isStartCalled;
    }
    uint64_t getSentRandomNumber() const {
        return sentRandomNumber;
    }
    uint64_t getTimesCalledSentRandomNumber() const {
        return timesCalledSentRandomNumber;
    }
    uint64_t getSentSumOfSquaresRequest() const {
        return sentSumOfSquaresRequest;
    }
    uint64_t getTimesCalledSumOfSquaresRequest() const {
        return timesCalledSumOfSquaresRequest;
    }
private:    
    bool isStartCalled = false;
    uint64_t sentRandomNumber = 0;
    uint64_t sentSumOfSquaresRequest = 0;
    uint64_t timesCalledSentRandomNumber = 0;
    uint64_t timesCalledSumOfSquaresRequest = 0;
};