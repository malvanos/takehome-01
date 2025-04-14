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