// Copyright (c) 2025 Michael Alvanos Services LTD
// All rights reserved.
//
// This software is provided "as is" without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.


#pragma once

#include <vector>

class NetworkProvider;

class NetworkObserver
{   
public:
    virtual ~NetworkObserver() = default;
    virtual void onNetworkStop() = 0;
    virtual void onNewNumber(int number) = 0;
    virtual void onAverageSquare(int number, std::shared_ptr<NetworkProvider> whoAsked) = 0;
};
