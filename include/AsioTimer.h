#pragma once
#include <chrono>
#include <functional>
#include <memory>
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/io_context.hpp>
#include "Timer.h"


class AsioTimer : public Timer, public std::enable_shared_from_this<AsioTimer> {
public:
    AsioTimer(boost::asio::io_context& io_context)
        : timer(io_context) {
    }
    void start(std::chrono::seconds periodSeconds, std::function<void(void)>&& callback) override {
        timer.expires_after(periodSeconds);
        timer.async_wait([callback = std::move(callback)](const boost::system::error_code& ec) {
            if (!ec) {
                callback();
            }
            });
    }
    void cancel() override {
        timer.cancel();
    }
private:
    boost::asio::steady_timer timer;
};
