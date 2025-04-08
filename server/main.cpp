// Copyright (c) 2025 Michael Alvanos Services LTD
// All rights reserved.
//
// This software is provided "as is" without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.

#include <iostream>
#include <memory>
#include "../logger/ConsoleLogger.h"
#include "../logger/FileLogger.h"
#include "../logger/DuplicateLogger.h"
#include "../logic/Logic.h"
#include "../include/Logger.h"
#include <string>

int main()
{

    auto constoleLogger = std::make_unique<ConsoleLogger>();
    auto fileLogger = std::make_unique<FileLogger>();
    auto duplicateLogger = std::make_unique<DuplicateLogger>(std::move(constoleLogger), std::move(fileLogger));
    auto logic = std::make_unique<Logic>(std::move(duplicateLogger));
    logic->start(42);

}

