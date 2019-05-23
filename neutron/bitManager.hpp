#pragma warning(disable:4996)
#ifndef BITMANAGER_HPP
#define BITMANAGER_HPP

#include "bitset.hpp"
#include "hash.hpp"

extern bool debug;
extern hash<0> hashC;
extern hash<1> hashT;

class bitManager {
	bitset b[30011];
	FILE *file;
public:
	bitManager() {
		if (!debug) {
			file = fopen("cityBitset", "rb");
			if (file == nullptr) {
				file = fopen("cityBitset", "wb");
				fclose(file);
			}
			else {
				file = fopen("cityBitset", "rb");
				fread(b, sizeof(bitset), 3000, file);
				fclose(file);
			}
		}
	}
	~bitManager() {
		if (!debug) {
			file = fopen("cityBitset", "wb");
			fwrite(b, sizeof(bitset), 3000, file);
			fclose(file);
		}
	}
	//add a newLine to a city
	void add(int city, int newLine) {
		b[city].to1(newLine);
	}
	//return bitset of train-intersection between two cities
	bitset intersection(int city1, int city2) {
		return b[city1] & b[city2];
	}
	//return bitset of train-intersection between one city and a bitset
	bitset intersection(int city, bitset &other) {
		return b[city] & other;
	}
};

#endif // !BITMANAGER_HPP
