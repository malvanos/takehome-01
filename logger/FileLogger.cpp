// Copyright (c) 2025 Michael Alvanos Services LTD
// All rights reserved.
//
// This software is provided "as is" without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.


#include "pch.h"
#include "FileLogger.h"
#include <filesystem>
#include <fstream>
#include <chrono>
#include <sstream>

#include <windows.h>
#include <process.h>

static unsigned long getProcessID() {
    return GetCurrentProcessId();
}

static std::string getCurrentTimestampForLoggerChrono() {
    auto now = std::chrono::system_clock::now();
    auto timepoint_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    long long milliseconds = timepoint_ms.time_since_epoch().count() % 1000;

    auto dp = std::chrono::floor<std::chrono::seconds>(now);
    std::time_t t = std::chrono::system_clock::to_time_t(dp);
    std::tm tm_val;

    localtime_s(&tm_val, &t);

    std::stringstream ss;
    ss << std::put_time(&tm_val, "%H:%M:%S") << "." << std::setfill('0') << std::setw(3) << milliseconds;
    return ss.str();
}


FileLogger::FileLogger()
{
    unsigned long processId = getProcessID();
    std::string logFilePathName = std::to_string(processId) + "_log.txt";
    auto logFilePath = std::filesystem::current_path() / logFilePathName;
    logFile = std::ofstream(logFilePath, std::ios::app);

    if (!logFile.is_open()) {
        throw std::runtime_error("Failed to open log file: " + logFilePath.string());
    }
}

void FileLogger::log(LogLevel level, const std::string& message) {
        std::lock_guard<std::mutex> lock(mutex);
        std::string timestamp = getCurrentTimestampForLoggerChrono();
        if (logFile.is_open()) {
            switch (level) {
            case LogLevel::INFO:
                logFile << timestamp << " [INFO]: " << message << std::endl;
                break;
            case LogLevel::WARNING:
                logFile << timestamp << " [WARNING]: " << message << std::endl;
                break;
            case LogLevel::LOGERROR:
                logFile << timestamp << " [ERROR]: " << message << std::endl;
                break;
            }
        }
}

FileLogger::~FileLogger() {
    if (logFile.is_open()) {
        logFile.flush();
        logFile.close();
    }
}
