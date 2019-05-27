#ifndef _BITSET_HPP
#define _BITSET_HPP

#include <cstring>
#define BLOCKSIZE (96)
#define BLOCK4    (14)
#define BLOCK8    (7)

void normal_and(u64 *a, u64 *b, u64 *c) {
    for (int i = 0; i < BLOCKSIZE; ++i) {
        *c = *a & *b;
        c++; a++; b++;
    }
}

void fast_and4(u64 *a, u64 *b, u64 *c) {
    for (int i = 0; i < BLOCK4; ++i) {
        *c = *a & *b;
        *(c + 1) = *(a + 1) & *(b + 1);
        *(c + 2) = *(a + 2) & *(b + 2);
        *(c + 3) = *(a + 3) & *(b + 3);
        c += 4; a += 4; b += 4;
    }
}

void fast_and8(u64 *a, u64 *b, u64 *c) {
    for (int i = 0; i < BLOCK8; ++i) {
        *c = *a & *b;
        *(c + 1) = *(a + 1) & *(b + 1);
        *(c + 2) = *(a + 2) & *(b + 2);
        *(c + 3) = *(a + 3) & *(b + 3);
        *(c + 4) = *(a + 4) & *(b + 4);
        *(c + 5) = *(a + 5) & *(b + 5);
        *(c + 6) = *(a + 6) & *(b + 6);
        *(c + 7) = *(a + 7) & *(b + 7);
        c += 8; a += 8; b += 8;
    }
}

using u64 = unsigned long long;

class bitset {
    u64 block[BLOCKSIZE];
    
public:
    bitset() { memset(block, 0, sizeof(block)); }
    
    void to1(int k) { block[k >> 6] |= 1ull << (k & 63); }
    bool operator [] (int k) { return block[k >> 6] & (1ull << (k & 63)); }
    bitset operator & (bitset &other) {
        bitset ret;
        for (int i = 0; i < B; i++)
            ret.block[i] = block[i] & other.block[i];
        return ret;
    }
};

#endif
