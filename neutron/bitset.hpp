#ifndef BITSET_HPP
#define BITSET_HPP
#include <string.h>

class bitset {
	static const int size = 60013;
	static const int B = (size - 1) / 64 + 1;
	unsigned long long block[B];
public:
	bitset() {
		memset(block, 0, sizeof(block));
	}
	void to1(int k) {
		block[k >> 6] |= 1 << (k & 63);
	}
	int operator [] (int k) {
		return block[k >> 6] & (1 << (k & 63));
	}
	bitset operator & (bitset &other) {
		bitset ret;
		for (int i = 0; i < size; i++)
			ret.block[i] = block[i] & other.block[i];
		return ret;
	}
};

#endif // !BITSET_HPP