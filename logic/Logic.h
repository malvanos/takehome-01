// Copyright (c) 2025 Michael Alvanos Services LTD
// All rights reserved.
//
// This software is provided "as is" without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.

#pragma once

#include "../include/Logger.h"
#include <memory>

class Logic {
    public:
        Logic(std::unique_ptr<Logger> logger);
        ~Logic();
        void start(int data);
private:
    std::unique_ptr<Logger> logger;
};