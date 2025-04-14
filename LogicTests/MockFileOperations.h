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
#include <string>
#include <memory>
#include "../include/FileOperations.h"
#include <fstream>

class MockFileOperations : public FileOperations, public std::enable_shared_from_this<MockFileOperations> {
public:
    void writeFile(const std::unordered_set<uint64_t>& numbers) override {
        // Mock implementation
        savedNumbers = numbers;
    }
    void stop() override {
        // Mock implementation
    }

    std::unordered_set<uint64_t> getSavedNumbers() const {
        return savedNumbers;
    }

private:
    std::unordered_set<uint64_t> savedNumbers;
};  