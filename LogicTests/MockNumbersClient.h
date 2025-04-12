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
#include "../include/NumbersClient.h"

class MockNumbersClient : public NumbersClient, public std::enable_shared_from_this<MockNumbersClient> {
public:
    void send(uint64_t sumOfSquares) override {
        // Mock implementation
        receivedSumOfSquares = sumOfSquares;
    }
    uint64_t getReceivedSumOfSquares() const {
        return receivedSumOfSquares;
    }
private:
    uint64_t receivedSumOfSquares = 0;
};
