// Copyright (c) 2025 Michael Alvanos Services LTD
// All rights reserved.
//
// This software is provided "as is" without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.

#pragma once

#include <vector>
#include <functional>

class NetworkProvider
{
public:
    virtual ~NetworkProvider() = default;
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void send(const std::vector<char>& message, std::function<void>&& callback) = 0;
};
