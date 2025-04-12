#pragma once
#include <chrono>
#include <functional>

class Timer {
public:
    virtual ~Timer() = default;
    virtual void start(std::chrono::seconds periodSeconds, std::function<void(void)>&& callback) = 0;
    virtual void cancel() = 0;
};