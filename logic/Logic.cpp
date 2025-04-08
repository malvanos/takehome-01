// logic.cpp : Defines the functions for the static library.
//

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
