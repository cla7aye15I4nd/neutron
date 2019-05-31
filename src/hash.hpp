#ifndef HASH_HPP
#define HASH_HPP

#include <cstring>
#include "tools.hpp"

#define IO

//city  p = 4093  size = 4096
//train p = 12281 size = 12288

class hashCity {
    const int P = 4093;
    const char STR[9] = "hashCity";
    int nameToNum[4096];
    str<20> numToName[2750];
    FILE *file;
public:
    int used = 0;
    void init() {
#ifdef IO
        file = fopen(STR, "rb");
        if (file == nullptr) {
            file = fopen(STR, "wb");
            fclose(file);
            memset(nameToNum, -1, sizeof(nameToNum));
            used = 0;
        }
        else {
            fread(&used, sizeof(int), 1, file);
            fread(nameToNum, sizeof(int), 4096, file);
            fread(numToName, sizeof(str<20>), 2750, file);
            fclose(file);
        }
#endif
    }

    void write_back() {
#ifdef IO
        file = fopen(STR, "wb");
        fwrite(&used, sizeof(int), 1, file);
        fwrite(nameToNum, sizeof(int), 4096, file);
        fwrite(numToName, sizeof(str<20>), 2750, file);
        fclose(file);
#endif
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
    int operator [] (str<20> &s) {
        int p = calc(s);
        while (nameToNum[p] != -1) {
            if (numToName[nameToNum[p]] == s) return nameToNum[p];
            if (++p == P) p = 0;
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

class hashTrain {
    const int P = 12281;
    const char STR[12] = "hashTrainID";
    int nameToNum[12288];
    str<20> numToName[6000];
    FILE *file;
public:
    int used = 0;
    void init() {
#ifdef IO
        file = fopen(STR, "rb");
        if (file == nullptr) {
            file = fopen(STR, "wb");
            fclose(file);
            memset(nameToNum, -1, sizeof(nameToNum));
            used = 0;
        }
        else {
            fread(&used, sizeof(int), 1, file);
            fread(nameToNum, sizeof(int), 12288, file);
            fread(numToName, sizeof(str<20>), 6000, file);
            fclose(file);
        }
#endif
    }
    void write_back () {
#ifdef IO
        file = fopen(STR, "wb");
        fwrite(&used, sizeof(int), 1, file);
        fwrite(nameToNum, sizeof(int), 12288, file);
        fwrite(numToName, sizeof(str<20>), 6000, file);
        fclose(file);
#endif
    }
    int calc(str<20> &s) {//trainID
        int ret = 0;
        for (int i = 0; s[i] != '\0'; i++)
            ret = ret * 67 - s[i];
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
    int operator [] (str<20> &s) {
        int p = calc(s);
        while (nameToNum[p] != -1) {
            if (numToName[nameToNum[p]] == s) return nameToNum[p];
            if (++p == P) p = 0;
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
