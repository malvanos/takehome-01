// Copyright (c) 2025 Michael Alvanos Services LTD
// All rights reserved.
//
// This software is provided "as is" without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.

#include "pch.h"
#include "Logic.h"

Logic::Logic(std::unique_ptr<Logger> logger)
    : logger(std::move(logger))
{
}

Logic::~Logic()
{
}

void Logic::start(int data)
{
    logger->log(Logger::LogLevel::INFO, "Logic started with data: " + std::to_string(data));
}
