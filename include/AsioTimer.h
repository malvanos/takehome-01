// Copyright (c) 2025 Michail Alvanos
// All rights reserved.
//
// This software is provided "as is" without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// This software is the property of Michael Alvanos.
// Redistribution or reproduction, in whole or in part, is not permitted
// without explicit prior written permission from the owner.

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
