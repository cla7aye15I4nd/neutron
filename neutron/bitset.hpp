#ifndef BITSET_HPP
#define BITSET_HPP
#include <string.h>

typedef unsigned long long ull;

class bitset {
	static const int B = 93;
	ull block[B];
public:
	bitset() {
		memset(block, 0, sizeof(block));
	}
	void to1(int k) {
		block[k >> 6] |= (ull)1 << (k & 63);
	}
	bool operator [] (int k) {
		return block[k >> 6] & ((ull)1 << (k & 63));
	}
	bitset operator & (bitset &other) {
		bitset ret;
		for (int i = 0; i < B; i++)
			ret.block[i] = block[i] & other.block[i];
		return ret;
	}
};

#endif // !BITSET_HPP