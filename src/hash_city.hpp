//
// Created by mintgreen on 19-6-1.
//

#ifndef NEUTRON_HASH_CITY_HPP
#define NEUTRON_HASH_CITY_HPP

#include <cstring>
#include <iostream>
#include "str.hpp"

#define CITY_SIZE 2714
#define CITY_LENGTH 20

class hashCity {
    const int P = 4093;
    const char STR[9] = "hashCity";
    short nameToNum[4096];
    str<CITY_LENGTH> numToName[CITY_SIZE];
    FILE *file;
public:
    short used = 0;
    void init() {
        file = fopen(STR, "rb");
        if (file == nullptr) {
            file = fopen(STR, "wb");
            fclose(file);
            clear();
        }
        else {
            fread(&used, sizeof(short), 1, file);
            fread(nameToNum, sizeof(short), 4096, file);
            fread(numToName, sizeof(str<CITY_LENGTH>), CITY_SIZE, file);
            fclose(file);
        }
    }
    void write_back() {
        file = fopen(STR, "wb");
        fwrite(&used, sizeof(short), 1, file);
        fwrite(nameToNum, sizeof(short), 4096, file);
        fwrite(numToName, sizeof(str<CITY_LENGTH>), CITY_SIZE, file);
        fclose(file);
    }
    int calc(str<CITY_LENGTH> &s) {//city
        int ret = 0;
        for (int i = 0; s[i] != '\0'; i++)
            ret = ret * 109 - s[i];
        return (ret % P + P) % P;
    }
    bool count(str<CITY_LENGTH> &s) {
        int p = calc(s);
        while (nameToNum[p] != -1) {
            if (numToName[nameToNum[p]] == s) return true;
            if (++p == P) p = 0;
        }
        return false;
    }
    void insert(str<CITY_LENGTH> &s) {
        int p = calc(s);
        while (nameToNum[p] != -1)
            if (++p == P) p = 0;
        numToName[used] = s;
        nameToNum[p] = used++;
    }
    short operator [] (str<CITY_LENGTH> &s) {
        int p = calc(s);
        while (nameToNum[p] != -1) {
            if (numToName[nameToNum[p]] == s) return nameToNum[p];
            if (++p == P) p = 0;
        }
        return -1;
    }
    str<CITY_LENGTH> operator [] (short k) {
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


#endif //NEUTRON_HASH_CITY_HPP
