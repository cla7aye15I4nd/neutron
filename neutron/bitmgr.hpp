#ifndef BITMANAGER_HPP
#define BITMANAGER_HPP

#include "bitset.hpp"

#define CITY_SIZE 2714
#define CITY_BITSET "city_bitset.db"

class bitManager {
	bitset b[CITY_SIZE];
	FILE *fp;

public:
	void init() {
		fp = fopen(CITY_BITSET, "rb");
		if (!fp)
			fp = fopen(CITY_BITSET, "wb");
		else
			fread(b, sizeof(bitset), CITY_SIZE, fp);
		fclose(fp);
	}
	void write_back() {
		fp = fopen(CITY_BITSET, "wb");
		fwrite(b, sizeof(bitset), CITY_SIZE, fp);
		fclose(fp);
	}

	void add(int city, int newLine) {
		b[city].set(newLine);
	}
	bitset intersection(int city1, int city2) {
		return b[city1] & b[city2];
	}

};

#endif