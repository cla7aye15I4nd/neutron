#ifndef HASH_HPP
#define HASH_HPP

#include "tools.h"

extern bool debug;

template <int id>
class hash {
	const int P[2] = { 30011, 60013 };
	const char STR[2][12] = { "hashCity", "hashTrainID" };
	int nameToNum[60013];
	str<20> numToName[60013];
	FILE *file;
public:
	int used = 0;
	hash() {
		if (!debug) {
			file = fopen(STR[id], "rb");
			if (file == nullptr) {
				file = fopen(STR[id], "wb");
				fclose(file);
				memset(nameToNum, -1, sizeof(nameToNum));
				used = 0;
			}
			else {
				file = fopen(STR[id], "rb");
				fread(&used, sizeof(int), 1, file);
				fread(nameToNum, sizeof(int), P[id], file);
				fread(numToName, sizeof(str<20>), P[id], file);
				fclose(file);
			}
		}
		else {
			memset(nameToNum, -1, sizeof(nameToNum));
			used = 0;
		}
	}
	~hash() {
		if (!debug) {
			file = fopen(STR[id], "wb");
			fwrite(&used, sizeof(int), 1, file);
			fwrite(nameToNum, sizeof(int), P[id], file);
			fwrite(numToName, sizeof(str<20>), P[id], file);
			fclose(file);
		}
	}
	int calc(str<20> &s) {
		int ret = 0;
		for (int i = 0; s[i] != '\0'; i++)
			ret = (ret * 127 + s[i] - '0') % P[id];
		return (ret + P[id]) % P[id];
	}
	bool count(str<20> &s) {
		int p = calc(s);
		while (nameToNum[p] != -1) {
			if (numToName[nameToNum[p]] == s) return 1;
			if (++p == P[id]) p = 0;
		}
		return 0;
	}
	void insert(str<20> &s) {
		int p = calc(s);
		while (nameToNum[p] != -1)
			if (++p == P[id]) p = 0;
		numToName[used] = s;
		nameToNum[p] = used++;
	}
	int operator [] (str<20> &s) {
		int p = calc(s);
		while (nameToNum[p] != -1) {
			if (numToName[nameToNum[p]] == s) return p;
			if (++p == P[id]) p = 0;
		}
		return -1;
	}
	str<20> operator [] (int k) {
		return numToName[k];
	}
	void list() {
		for (int i = 0; i < used; i++)
			printf("%s\n", numToName[i].ch);
	}
	void clear() {
		memset(nameToNum, -1, sizeof(nameToNum));
		used = 0;
	}
};

#endif