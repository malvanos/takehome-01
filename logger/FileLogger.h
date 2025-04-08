// Copyright (c) 2025 Michael Alvanos Services LTD
//
// This software is provided "as is" without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.

#pragma once

#include "../include/Logger.h"
#include <string>
#include <mutex>
#include <fstream>

class FileLogger : public Logger {
public:
    FileLogger();
    ~FileLogger();

    void log(LogLevel level, const std::string& message) override;
private:
    std::mutex mutex;
    std::ofstream logFile;
};