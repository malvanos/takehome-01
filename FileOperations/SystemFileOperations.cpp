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

#include "pch.h"
#include "SystemFileOperations.h"


SystemFileOperations::SystemFileOperations(Dependencies&& deps)
    : workGuard(deps.ioContext.get_executor())
    , logger(deps.logger)
{

}

void SystemFileOperations::stop() {
    workGuard.reset();
}

void SystemFileOperations::writeFile(const std::unordered_set<uint64_t>& numbers){
    logger->info("Writing file binary_data.dat");
    std::ofstream outputFile("binary_data.dat", std::ios::binary);

    if (outputFile.is_open()) {
        // Header
        uint32_t count = static_cast<uint32_t>(numbers.size());
        outputFile.write(reinterpret_cast<const char*>(&count), sizeof(count));

        // The actual numbers - we can use short here sice we know the values are 0<= x <= 1023
        for (uint64_t num : numbers) {
            outputFile.write(reinterpret_cast<const char*>(&num), sizeof(num));
        }

        outputFile.close();
    }
};