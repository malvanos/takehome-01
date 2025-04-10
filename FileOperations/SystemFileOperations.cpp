// Copyright (c) 2025 Michael Alvanos Services LTD
// All rights reserved.
//
// This software is provided "as is" without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.


#include "pch.h"
#include "SystemFileOperations.h"

// TODO: add locks or queue post

SystemFileOperations::SystemFileOperations(Dependencies&& deps)
    : workGuard(deps.ioContext.get_executor())
{

}

void SystemFileOperations::stop() {
    workGuard.reset();
}

void SystemFileOperations::writeFile(const std::unordered_set<unsigned short>& numbers){
    std::ofstream outputFile("binary_data.dat", std::ios::binary);

    if (outputFile.is_open()) {
        // Header
        uint32_t count = static_cast<uint32_t>(numbers.size());
        outputFile.write(reinterpret_cast<const char*>(&count), sizeof(count));

        // The actual numbers
        for (unsigned short num : numbers) {
            outputFile.write(reinterpret_cast<const char*>(&num), sizeof(num));
        }

        outputFile.close();
    }
};