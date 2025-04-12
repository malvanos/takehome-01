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