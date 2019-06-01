#ifndef HASH_HPP
#define HASH_HPP

#include <cstring>
#include "tools.hpp"

//city  p = 4093  size = 4096
//train p = 12281 size = 12288

const int trainHashSize = 12288;

class hashTrain {
    const int P = 12281;
    const char STR[12] = "hashTrainID";
    str<20> numToName[trainN];
    int posTrainData[trainN];
    short nameToNum[trainHashSize];
    FILE *file;
public:
    int used = 0;
    //read all from file
    void init() {
        file = fopen(STR, "rb");
        if (file == nullptr) {
            file = fopen(STR, "wb");
            fclose(file);
            file = fopen("trainData", "wb");
            fclose(file);
            clear();
        }
        else {
            fread(&used, sizeof(short), 1, file);
            fread(numToName, sizeof(str<20>), used, file);
            fread(posTrainData, sizeof(int), trainN, file);
            fread(nameToNum, sizeof(short), trainHashSize, file);
            fclose(file);
        }
        file = fopen("trainData", "rb+");
    }
    //write all to file
    void write_back() {
        fclose(file);
        file = fopen(STR, "wb");
        fwrite(&used, sizeof(short), 1, file);
        fwrite(numToName, sizeof(str<20>), used, file);
        fwrite(posTrainData, sizeof(int), trainN, file);
        fwrite(nameToNum, sizeof(short), trainHashSize, file);
        fclose(file);
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
            if (numToName[nameToNum[p]] == s)
                return posTrainData[nameToNum[p]] != -1;
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
        memset(posTrainData, -1, sizeof(posTrainData));
        used = 0;
    }

    //----------------------------------------below this line are file operations
    //ATTENTION: following id(s) are tight!

    //write trainData to file (If newPos == true, it must append in the end, overwrite otherwise.)
    //TODO optimize
    void insertFile(int id, trainData data, bool newPos = false) {
        if (newPos) {
            fseek(file, 0, SEEK_END);
            posTrainData[id] = ftell(file);
            fwrite(&data, sizeof(trainData), 1, file);
        }
        else {
            fseek(file, posTrainData[id], SEEK_SET);
            fwrite(&data, sizeof(trainData), 1, file);
        }
    }
    //read trainData from file
    //TODO optimize
    void getTrainData(short id, trainData &data) {
        fseek(file, posTrainData[id], SEEK_SET);
        fread(&data, sizeof(trainData), 1, file);
    }
};
#endif
