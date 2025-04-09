#pragma once
#include <cstdint>


#pragma pack(1)

#pragma pack(push, 1) // 1-byte alignment

struct NetworkPacket
{
    uint8_t version;
    uint8_t packetType;
    uint16_t data;
    uint32_t hashCheck;
};

static uint32_t packetHashCalculator(const NetworkPacket& packet)
{
    // Random hash function found on the net. TODO: Need a proper CRC method.
    uint32_t hash = 0;
    hash ^= packet.version;
    hash = (hash * 16777619U); // 16777619U is a large prime
    hash ^= packet.packetType;
    hash = (hash * 16777619U);
    hash ^= packet.data;
    hash = (hash * 16777619U);

    return hash;
}

static bool validatePacket(const NetworkPacket& packet)
{
    return packet.hashCheck == packetHashCalculator(packet);
}

#pragma pack(pop)
