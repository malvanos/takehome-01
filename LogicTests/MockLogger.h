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
#include "../include/Logger.h"

class MockLogger : public Logger, public std::enable_shared_from_this<MockLogger> {  
public:
    void log(LogLevel level, const std::string& message) override {
        // Mock implementation
    }
    void info(const std::string& message) override {
        // Mock implementation
    }
    void warning(const std::string& message) override {
        // Mock implementation
    }
    void error(const std::string& message) override {
        // Mock implementation
    }
};