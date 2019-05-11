#ifndef HASH_HPP
#define HASH_HPP

#include "tools.hpp"
class hash {
	const int P = 1009;
	int used = 0, nameToNum[P];
	str<20> numToName[P];
	FILE *file;
public:
	hash() {
		file = fopen("hash", "rb");
		if (file == nullptr) {
			file = fopen("hash", "wb");
			fclose(file);
			memset(nameToNum, -1, sizeof(nameToNum));
			used = 0;
		}
		else {
			file = fopen("hash", "rb");
			fread(&used, sizeof(int), 1, file);
			fread(nameToNum, sizeof(int), P, file);
			fread(numToName, sizeof(str<20>), P, file);
			fclose(file);
		}
	}
	~hash() {
		file = fopen("hash", "wb");
		fwrite(&used, sizeof(int), 1, file);
		fwrite(nameToNum, sizeof(int), P, file);
		fwrite(numToName, sizeof(str<20>), P, file);
		fclose(file);
	}
	int calc(str<20> &str) {
		int ret = 0;
		for (int i = 0; str[i] != '\0'; i++)
			ret = (ret * 127 + str[i] - '0') % P;
		return ret;
	}
	bool count(str<20> &str) {
		int p = calc(str);
		while (nameToNum[p] != -1) {
			if (numToName[nameToNum[p]] == str) return 1;
			if (++p == P) p = 0;
		}
		return 0;
	}
	void insert(str<20> &str) {
		int p = calc(str);
		while (nameToNum[p] == -1)
			if (++p == P) p = 0;
		numToName[used] = str;
		nameToNum[p] = used++;
	}
	int operator [] (str<20> &str) {
		int p = calc(str);
		while (nameToNum[p] != -1) {
			if (numToName[nameToNum[p]] == str) return p;
			if (++p == P) p = 0;
		}
		return -1;
	}
	std::string list() {
		//change to streamstream?
		std::string ret;
		for (int i = 0; i < used; i++)
			ret = ret + numToName[i].toString();
		return ret;
	}
	void clear() {
		memset(nameToNum, -1, sizeof(nameToNum));
		used = 0;
	}
};

#endif