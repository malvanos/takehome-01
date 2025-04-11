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

#include <memory>

class NetworkObserver;

class NetworkProvider 
{
public:
    virtual ~NetworkProvider() = default;
    virtual void start(std::shared_ptr<NetworkObserver> observer) = 0;
    virtual void stop() = 0;
};
