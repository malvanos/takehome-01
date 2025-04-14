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
#include <memory>

class NetworkClientObserver {
public:
    virtual ~NetworkClientObserver() = default;
    virtual void onSumOfSquaresResponse(uint64_t sum) = 0;
    virtual void onDisconnect() = 0;
    virtual void onConnect() = 0;
};