//
// Created by mintgreen on 19-6-1.
//

#ifndef NEUTRON_TICKETNUM_MGR_HPP
#define NEUTRON_TICKETNUM_MGR_HPP

#include <iostream>

const int trainN = 6000;
const int stopN = 56000;

class ticketnum_manager{
    const char STR[14] = "ticketNumData";
    int posTicketNum[trainN];
    short ticketNum[stopN * 150];
    int usedTicketNum = 0;
    FILE *file;
public:
    //read all from file
    void init() {
        file = fopen(STR, "rb");
        if (file == nullptr) {
            file = fopen(STR, "wb");
            fclose(file);
            clear();
        }
        else {
            fread(posTicketNum, sizeof(int), trainN, file);
            fread(ticketNum, sizeof(short), stopN * 150, file);
            fread(&usedTicketNum, sizeof(short), 1, file);
            fclose(file);
        }
        file = fopen(STR, "rb+");
    }
    void write_back() {
        fwrite(posTicketNum, sizeof(int), trainN, file);
        fwrite(ticketNum, sizeof(short), stopN * 150, file);
        fwrite(&usedTicketNum, sizeof(short), 1, file);
        fclose(file);
    }
    //get (id)train's ticket info from file [n stops * 5 ticketKinds]
    void getTicketNum(int id, short* &a) {
        a = ticketNum + posTicketNum[id];
    }
    void clear() {
        memset(ticketNum, 0, sizeof(ticketNum));
    }
};

#endif //NEUTRON_TICKETNUM_MGR_HPP
