#pragma once

class NetworkClientProvider {
 public: 
    virtual ~NetworkClientProvider() = default;
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void sendRandomNumber(uint64_t number) = 0;
    virtual void sendSumOfSquaresRequest(uint64_t number) = 0;
};