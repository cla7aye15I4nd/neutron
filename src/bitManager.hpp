#ifndef BITMANAGER_HPP
#define BITMANAGER_HPP

#include "bitset.hpp"
#include "hash.hpp"

extern hashCity hashC;
extern hashTrain hashT;

class bitManager {
    bitset b[2750];
    FILE *file;
public:
    void init() {
#ifdef IO
        file = fopen("cityBitset", "rb");
        if (file == nullptr) {
            file = fopen("cityBitset", "wb");
            fclose(file);
        }
        else {
            fread(b, sizeof(bitset), 2750, file);
            fclose(file);
        }
#endif
    }
    void write_back() {
#ifdef IO
        file = fopen("cityBitset", "wb");
        fwrite(b, sizeof(bitset), 2750, file);
        fclose(file);
#endif
    }
    //add a newLine to a city
    void add(int city, int newLine) {
        //fprintf(stderr, "City %d has added a new line %d\n", city, newLine);
        b[city].to1(newLine);
    }
    //return bitset of train-intersection between two cities
    bitset intersection(int city1, int city2) {
        return b[city1] & b[city2];
    }

};

#endif // !BITMANAGER_HPP
