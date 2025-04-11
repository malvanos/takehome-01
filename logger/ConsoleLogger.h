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

#include "../include/Logger.h"

#include <iostream>
#include <mutex>
#include <string>

class ConsoleLogger : public Logger
{
public:
    void log(LogLevel level, const std::string& message) override {
        std::lock_guard<std::mutex> lock(mutex);
        switch (level) {
        case LogLevel::INFO:
            std::cout << "[INFO] " << message << std::endl;
            break;
        case LogLevel::WARNING:
            std::cout << "[WARNING] " << message << std::endl;
            break;
        case LogLevel::LOGERROR:
            std::cout << "[ERROR] " << message << std::endl;
            break;
        }
    }
private:
    std::mutex mutex;
};
