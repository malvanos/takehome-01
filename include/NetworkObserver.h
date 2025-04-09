// Copyright (c) 2025 Michael Alvanos Services LTD
// All rights reserved.
//
// This software is provided "as is" without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.


#pragma once

#include <vector>

class NetworkObserver
{   
public:
    virtual ~NetworkObserver() = default;
    virtual void onClientConnected() = 0;
    virtual void onNetworkStop() = 0;
    virtual void onMessage(const std::vector<char>& message) = 0;
};
