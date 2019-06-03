//
// Created by mintgreen on 19-6-1.
//

#ifndef NEUTRON_TICKETNUM_MGR_HPP
#define NEUTRON_TICKETNUM_MGR_HPP

#include <iostream>

typedef unsigned long long ull;

const int trainN = 5940;
const int bound = 40000 * 30;

class ticketnum_manager{
    const char STR[14] = "ticket_mgr.db";
    int posTicketNum[trainN];
    ull ticketNum[bound], pool[1800];
    short ref[trainN];
    int usedTicketNum = 0, usedRef = 0, det = 35648, start;
    FILE *file;
public:
    //read all from file
    int min(int x, int y) {
        return x < y ? x : y;
    }
    void init() {
        file = fopen(STR, "rb");
        if (file == nullptr) {
            file = fopen(STR, "wb");
            fwrite(&usedTicketNum, sizeof(int), 1, file);
            fwrite(&usedRef, sizeof(int), 1, file);
            fwrite(posTicketNum, sizeof(int), trainN, file);
            fwrite(ref, sizeof(short), trainN, file);
            fclose(file);
            clear();
        }
        else {
            fread(&usedTicketNum, sizeof(int), 1, file);
            fread(&usedRef, sizeof(int), 1, file);
            fread(posTicketNum, sizeof(int), trainN, file);
            fread(ref, sizeof(short), trainN, file);
            fread(ticketNum, sizeof(ull), min(usedTicketNum, bound), file);
            fclose(file);
        }
        file = fopen(STR, "rb+");
    }
    void write_back() {
        fseek(file, 0, SEEK_SET);
        fwrite(&usedTicketNum, sizeof(int), 1, file);
        fwrite(&usedRef, sizeof(int), 1, file);
        fwrite(posTicketNum, sizeof(int), trainN, file);
        fwrite(ref, sizeof(short), trainN, file);
        //fprintf(stderr, "%d\n", ftell(file));
        fwrite(ticketNum, sizeof(ull), min(usedTicketNum, bound), file);
        fclose(file);
    }
    void addNewTrain(int id, int n) {
        ref[id] = usedRef++;
        if (usedTicketNum <= bound && usedTicketNum + n * 30 > bound) {
            usedTicketNum = bound;
            fseek(file, det, SEEK_SET);
            fwrite(ticketNum, sizeof(ull), bound, file);
        }
        posTicketNum[ref[id]] = usedTicketNum;
        usedTicketNum += n * 30;
    }
    //get (id)train's ticket info from file [n stops * 5 ticketKinds]
    bool getTicketNum(int id, ull *&a) {
        if (posTicketNum[ref[id]] < bound) {
            a = ticketNum + posTicketNum[ref[id]];
            return false;
        }
        start = det + posTicketNum[ref[id]] * sizeof(ull);
        fseek(file, start, SEEK_SET);
        fread(pool, sizeof(ull), 1800, file);
        a = pool;
        return true;
    }
    void rectify(int n) {
        fseek(file, start, SEEK_SET);
        fwrite(pool, sizeof(ull), n * 30, file);
    }
    void clear() {
        memset(ticketNum, 0, sizeof(ticketNum));
        usedTicketNum = 0;
        usedRef = 0;
    }
};

#endif //NEUTRON_TICKETNUM_MGR_HPP
