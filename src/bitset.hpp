#ifndef _BITSET_HPP
#define _BITSET_HPP

#include <cstring>
#define BLOCKSIZE (93)
#define BLOCK4    (24)
#define BLOCK8    (12)

using u64 = unsigned long long;
using ull = unsigned long long;

void normal_and(u64 *a, u64 *b, u64 *c) {
    for (int i = 0; i < BLOCKSIZE; ++i) {
        *c = *a & *b;
        c++; a++; b++;
    }
}

struct bitset {
    u64 block[BLOCKSIZE];
    
    bitset() { memset(block, 0, sizeof(block)); }
    
    void set(int k) { block[k >> 6] |= 1ull << (k & 63); }
    bool operator [] (int k) {
        return block[k >> 6] & (1ull << (k & 63));
    }
    bitset operator & (bitset &other) {
        bitset ret;
        normal_and(block, other.block, ret.block);
        return ret;
    }
};

#endif
