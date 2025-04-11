// Copyright (c) 2025 Michael Alvanos Services LTD
// All rights reserved.
//
// This software is provided "as is" without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// This software is the property of Michael Alvanos Services LTD.
// Redistribution or reproduction, in whole or in part, is not permitted
// without explicit prior written permission from the owner.

#pragma once

#include <vector>

class NumbersClient;

class NetworkObserver
{   
public:
    virtual ~NetworkObserver() = default;
    virtual void onNewNumber(uint64_t number) = 0;
    virtual void onAverageSquare(uint64_t number, std::shared_ptr<NumbersClient> whoAsked) = 0;
};
