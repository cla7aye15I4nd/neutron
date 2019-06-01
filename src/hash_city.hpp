//
// Created by mintgreen on 19-6-1.
//

#ifndef NEUTRON_HASH_CITY_HPP
#define NEUTRON_HASH_CITY_HPP

#include <cstring>
#include <iostream>
#include "str.hpp"

class hashCity {
    const int P = 4093;
    const char STR[9] = "hashCity";
    short nameToNum[4096];
    str<20> numToName[2750];
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
            fread(numToName, sizeof(str<20>), 2750, file);
            fclose(file);
        }
    }
    void write_back() {
        file = fopen(STR, "wb");
        fwrite(&used, sizeof(short), 1, file);
        fwrite(nameToNum, sizeof(short), 4096, file);
        fwrite(numToName, sizeof(str<20>), 2750, file);
        fclose(file);
    }
    int calc(str<20> &s) {//city
        int ret = 0;
        for (int i = 0; s[i] != '\0'; i++)
            ret = ret * 109 - s[i];
        return (ret % P + P) % P;
    }
    bool count(str<20> &s) {
        int p = calc(s);
        while (nameToNum[p] != -1) {
            if (numToName[nameToNum[p]] == s) return true;
            if (++p == P) p = 0;
        }
        return false;
    }
    void insert(str<20> &s) {
        int p = calc(s);
        while (nameToNum[p] != -1)
            if (++p == P) p = 0;
        numToName[used] = s;
        nameToNum[p] = used++;
    }
    short operator [] (str<20> &s) {
        int p = calc(s);
        while (nameToNum[p] != -1) {
            if (numToName[nameToNum[p]] == s) return nameToNum[p];
            if (++p == P) p = 0;
        }
        return -1;
    }
    str<20> operator [] (short k) {
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
