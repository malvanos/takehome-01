// Copyright (c) 2025 Michael Alvanos Services LTD
// All rights reserved.
//
// This software is provided "as is" without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.

#pragma once
#include "../include/Logger.h"

#include <memory>
#include <string>

class DuplicateLogger : public Logger, std::enable_shared_from_this<DuplicateLogger>
{
public:
    DuplicateLogger(std::unique_ptr<Logger> logger1, std::unique_ptr<Logger> logger2)
        : m_logger1(std::move(logger1)), m_logger2(std::move(logger2)) {
    }
    void log(LogLevel level, const std::string& message) override {
        m_logger1->log(level, message);
        m_logger2->log(level, message);
    }
private:
    std::unique_ptr<Logger> m_logger1;
    std::unique_ptr<Logger> m_logger2;

};

