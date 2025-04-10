// Copyright (c) 2025 Michael Alvanos Services LTD
// All rights reserved.
//
// This software is provided "as is" without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.

#pragma once
#include <cstdint>

#pragma pack(push, 1) // 1-byte alignment

enum class PacketType : uint32_t { 
    INVALID=0, 
    DATA_ADD=1, 
    SUM_OF_SQUARES_REQUEST, 
    SUM_OF_SQUARES_ANSWER
};


struct NetworkPacket
{
    PacketType packetType;
    uint64_t data;
    uint32_t hashCheck;
};

static uint32_t packetHashCalculator(const NetworkPacket& packet)
{
    // Random hash function found on the net. TODO: Need a proper CRC method.
    uint32_t hash = 0;
    hash ^= static_cast<uint32_t>(packet.packetType);
    hash = (hash * 16777619U);// 16777619U is a large prime
    hash ^= packet.data;
    hash = (hash * 16777619U);

    return hash;
}

static bool validatePacket(const NetworkPacket& packet)
{
    return packet.hashCheck == packetHashCalculator(packet);
}

static NetworkPacket createPacket(PacketType packetType, uint64_t data) {
    NetworkPacket packet = {
        .packetType = packetType,
        .data = data
    };
    packet.hashCheck = packetHashCalculator(packet);
    return packet;
}

static void fillPacket(NetworkPacket& packet, PacketType packetType, uint64_t data) {
    packet.packetType = packetType;
    packet.data = data;
    packet.hashCheck = packetHashCalculator(packet);
}

#pragma pack(pop)
