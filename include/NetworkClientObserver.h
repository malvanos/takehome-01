#pragma once
#include <memory>

class NetworkClientObserver {
public:
    virtual ~NetworkClientObserver() = default;
    virtual void onSumOfSquaresResponse(uint64_t sum) = 0;
    virtual void onDisconnect() = 0;
    virtual void onConnect() = 0;
};