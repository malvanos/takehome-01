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