//
// Created by mintgreen on 19-6-1.
//

#ifndef NEUTRON_TICKETNUM_MGR_HPP
#define NEUTRON_TICKETNUM_MGR_HPP

#include <iostream>

const int trainN = 5940;
const int stopN = 50150;

class ticketnum_manager{
    const char STR[14] = "ticketNumData";
    int posTicketNum[trainN];
    short ticketNum[stopN * 150], ref[trainN];
    int usedTicketNum = 0, usedRef = 0;
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
            fread(&usedTicketNum, sizeof(int), 1, file);
            fread(&usedRef, sizeof(int), 1, file);
            fread(posTicketNum, sizeof(int), usedRef, file);
            fread(ticketNum, sizeof(short), usedTicketNum, file);
            fread(ref, sizeof(short), trainN, file);
            fclose(file);
        }
        file = fopen(STR, "rb+");
    }
    void write_back() {
        fwrite(&usedTicketNum, sizeof(int), 1, file);
        fwrite(&usedRef, sizeof(int), 1, file);
        fwrite(posTicketNum, sizeof(int), usedRef, file);
        fwrite(ticketNum, sizeof(short), usedTicketNum, file);
        fwrite(ref, sizeof(short), trainN, file);
        fclose(file);
    }
    void addNewTrain(int id, int n) {
        ref[id] = usedRef++;
        posTicketNum[ref[id]] = usedTicketNum;
        usedTicketNum += n * 150;
    }
    //get (id)train's ticket info from file [n stops * 5 ticketKinds]
    void getTicketNum(int id, short *&a) {
        a = ticketNum + posTicketNum[ref[id]];
    }
    void clear() {
        memset(ticketNum, 0, sizeof(ticketNum));
        usedTicketNum = 0;
        usedRef = 0;
    }
};

#endif //NEUTRON_TICKETNUM_MGR_HPP
