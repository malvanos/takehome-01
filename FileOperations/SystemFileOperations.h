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

#include "../include/FileOperations.h"
#include <fstream>
#include <boost/asio.hpp>
#include <memory>
#include "../include/Logger.h"

class SystemFileOperations : public FileOperations {
public:
    struct Dependencies {
        boost::asio::io_context& ioContext;
        std::shared_ptr<Logger> logger;
    };

    SystemFileOperations(Dependencies&& deps);
    void writeFile(const std::unordered_set<uint64_t>& numbers) override;
    void stop() override;
private:
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> workGuard;
    std::shared_ptr<Logger> logger;
};

