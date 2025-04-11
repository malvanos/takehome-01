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

class Logger {
public:
    enum class LogLevel {
        INFO,
        WARNING,
        LOGERROR
    };

    virtual ~Logger() = default;

    virtual void log(LogLevel level, const std::string& message) = 0;

    virtual void info(const std::string& message) { log(LogLevel::INFO, message); }
    virtual void warning(const std::string& message) { log(LogLevel::WARNING, message); }
    virtual void error(const std::string& message) { log(LogLevel::LOGERROR, message); }

};