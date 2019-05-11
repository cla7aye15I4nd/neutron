template <size_t size>
class bitset {
	int B = (size - 1) / 64 + 1;
	unsigned long long block[B];
public:
	bitset() {
		memset(block, 0, sizeof(block));
	}
	void to1(int k) {
		block[k >> 6] |= 1 << (k & 63);
	}
	bitset and (bitset &other) {
		bitset ret;
		for (int i = 0; i < size; i++)
			ret[i] = block[i] & other.block[i];
		return ret;
	}
};