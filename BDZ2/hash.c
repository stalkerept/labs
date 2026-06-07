#include <stdint.h>
#include <string.h>

static inline uint64_t XXH_rotl64(uint64_t x, int r) {
    return (x << r) | (x >> (64 - r));
}

static inline uint64_t XXH64_read64(const void* ptr) {
    uint64_t val;
    memcpy(&val, ptr, sizeof(val));
    return val;
}

static inline uint64_t XXH64_read32(const void* ptr) {
    uint32_t val;
    memcpy(&val, ptr, sizeof(val));
    return (uint64_t)val;
}


uint64_t hash_string(const char* data) {
    size_t len = (size_t)strlen(data);
    const uint64_t PRIME64_1 = 0x9E3779B185EBCA87ULL;
    const uint64_t PRIME64_2 = 0xC2B2AE3D27D4EB4FULL;
    const uint64_t PRIME64_3 = 0x165667B19E3779F9ULL;
    const uint64_t PRIME64_4 = 0x85EBCA77C2B2AE63ULL;
    const uint64_t PRIME64_5 = 0x27D4EB2F165667C5ULL;
    const unsigned char* p = (const unsigned char*)data;
    const unsigned char* const end = p + len;
    uint64_t h64;
    if (len >= 32) {
        const unsigned char* const limit = end - 32;
        uint64_t v1 = PRIME64_1 + PRIME64_2;
        uint64_t v2 = PRIME64_2;
        uint64_t v3 = 0;
        uint64_t v4 = 0 - PRIME64_1;
        do {
            v1 = XXH_rotl64(v1 + XXH64_read64(p) * PRIME64_2, 31) * PRIME64_1;
            p += 8;
            v2 = XXH_rotl64(v2 + XXH64_read64(p) * PRIME64_2, 31) * PRIME64_1;
            p += 8;
            v3 = XXH_rotl64(v3 + XXH64_read64(p) * PRIME64_2, 31) * PRIME64_1;
            p += 8;
            v4 = XXH_rotl64(v4 + XXH64_read64(p) * PRIME64_2, 31) * PRIME64_1;
            p += 8;
        } while (p <= limit);
        h64 = XXH_rotl64(v1, 1) + XXH_rotl64(v2, 7) +
              XXH_rotl64(v3, 12) + XXH_rotl64(v4, 18);
        v1 = XXH_rotl64(v1 * PRIME64_2, 31) * PRIME64_1;
        h64 = (h64 ^ v1) * PRIME64_1 + PRIME64_4;
        v2 = XXH_rotl64(v2 * PRIME64_2, 31) * PRIME64_1;
        h64 = (h64 ^ v2) * PRIME64_1 + PRIME64_4;
        v3 = XXH_rotl64(v3 * PRIME64_2, 31) * PRIME64_1;
        h64 = (h64 ^ v3) * PRIME64_1 + PRIME64_4;
        v4 = XXH_rotl64(v4 * PRIME64_2, 31) * PRIME64_1;
        h64 = (h64 ^ v4) * PRIME64_1 + PRIME64_4;
    } else {
        h64 = PRIME64_5;
    }
    h64 += (uint64_t)len;
    while (p + 8 <= end) {
        uint64_t k1 = XXH_rotl64(XXH64_read64(p) * PRIME64_2, 31) * PRIME64_1;
        h64 ^= k1;
        h64 = XXH_rotl64(h64, 27) * PRIME64_1 + PRIME64_4;
        p += 8;
    }
    if (p + 4 <= end) {
        h64 ^= (uint64_t)XXH64_read32(p) * PRIME64_1;
        h64 = XXH_rotl64(h64, 23) * PRIME64_2 + PRIME64_3;
        p += 4;
    }
    while (p < end) {
        h64 ^= (*p) * PRIME64_5;
        h64 = XXH_rotl64(h64, 11) * PRIME64_1;
        p++;
    }
    h64 ^= h64 >> 33;
    h64 *= PRIME64_2;
    h64 ^= h64 >> 29;
    h64 *= PRIME64_3;
    h64 ^= h64 >> 32;
    return h64;
}