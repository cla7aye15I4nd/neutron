#ifndef TRAIN_HPP
#define TRAIN_HPP

#include <string>
#include "tools.hpp"
#include "bptree.hpp"
#include "hash.hpp"
#include "str.hpp"

using namespace sjtu;

extern hashCity hashC;
extern hashTrain hashT;
extern int bridgeN;
extern ticketData bridge[100];

class train {
    bptree<int, trainData> trainInfo;
    bptree<long long, ticketData> ticketInfo;
    str<20> trainID;
    str<20> idArray_s[200];
    int idArray[200];
    long long combine(int userID, int date, int trainID = 0, int start = 0, int end = 0) {
        return ((((long long)userID * 100 + date) * 100000 + trainID) * 100 + start) * 100 + end;
    }
    bool in(char c, str<10> s) {
        for (int i = 0; i < 10; i++) {
            if (s[i] == '\0') return false;
            if (s[i] == c) return true;
        }
    }
public:
    train() {}
    ~train() {}
    bool add() {
        scanf("%s", trainID.ch);
        if (hashT.count(trainID)) {
            trainData(trainID);
            return 0;
        }
        hashT.insert(trainID);
        trainInfo.insert(hashT[trainID], trainData(trainID));
        return 1;
    }
    bool sale() {
        scanf("%s", trainID.ch);
        int h = hashT[trainID];
        if (!trainInfo.count(h)) return 0;
        trainData t = trainInfo.find(h);
        if (t.saled) return 0;
        t.saled = 1;
        trainInfo.erase(h);
        trainInfo.insert(h, t);
        t.update(h);
        return 1;
    }
    void qry() {
        scanf("%s", trainID.ch);
        if (!hashT.count(trainID) || !trainInfo.count(hashT[trainID])) { printf("0\n"); return; }
        trainInfo.find(hashT[trainID]).print();
    }
    int del() {
        scanf("%s", trainID.ch);
        if (!trainInfo.count(hashT[trainID])) return 0;
        if (trainInfo.find(hashT[trainID]).saled) return 0;
        trainInfo.erase(hashT[trainID]);
        return 1;
    }
    int mod() {
        scanf("%s", trainID.ch);
        if (!trainInfo.count(hashT[trainID])) return 0;
        if (trainInfo.find(hashT[trainID]).saled) return 0;
        trainInfo.erase(hashT[trainID]);
        trainInfo.insert(hashT[trainID], trainData(trainID));
        return 1;
    }
    void clear() {
        hashC.clear();
        hashT.clear();
        //TODO B�����
    }
    void qryDirect() {
        str<20> loc1, loc2;
        str<10> catalog, date_s;
        int date;
        scanf("%s%s%s%s", loc1.ch, loc2.ch, date_s.ch, catalog.ch);
        date = (date_s[8] - '0') * 10 + (date_s[9] - '0');
        if (!hashC.count(loc1) || !hashC.count(loc2)) { printf("-1\n"); return; }
        int start = hashC[loc1], end = hashC[loc2];
        bitset can = bitMgr.intersection(start, end);
        int cnt = 0;
        for (int i = 0; i < 12281; i++)
            if (can[i]) {
                idArray[cnt] = i;
                if (!trainInfo.count(i)) {
                    printf("fuck\n");
                    break;
                }
                trainData tmp = trainInfo.find(i);
                idArray_s[cnt] = tmp.trainID;
                //printf("can i = %d s = %s\n", i, tmp.trainID.ch);
                if (tmp.order(start, end) && in(tmp.catalog[0], catalog)) cnt++;
            }
        //TODO: optimize the damn sort
        for (int i = 0; i < cnt; i++)
            for (int j = i + 1; j < cnt; j++)
                if (idArray_s[j] < idArray_s[i]) {
                    std::swap(idArray[i], idArray[j]);
                    std::swap(idArray_s[i], idArray_s[j]);
                }
        printf("%d\n", cnt);
        for (int i = 0; i < cnt; i++)
            trainInfo.find(idArray[i]).print_detailed(start, end, date);
    }
    void qryTrans() {
        str<20> loc1, loc2;
        str<10> catalog, date_s;
        int date;
        scanf("%s%s%s%s", loc1.ch, loc2.ch, date_s.ch, catalog.ch);
        date = (date_s[8] - '0') * 10 + (date_s[9] - '0');
        if (!hashC.count(loc1) || !hashC.count(loc2)) { printf("-1\n"); return; }
        int start = hashC[loc1], end = hashC[loc2];
        bitset can = bitMgr.intersection(start, end);
        for (int i = 0; i < hashC.used; i++) {

        }
    }
    int buy() {
        int userID, num, date;
        str<10> date_s;
        str<20> loc1, loc2, ticketKind;
        scanf("%d%d%s%s%s%s%s", &userID, &num, trainID.ch, loc1.ch, loc2.ch, date_s.ch, ticketKind.ch);
        if (!hashT.count(trainID) || !trainInfo.count(hashT[trainID])) return 0;
        date = (date_s[8] - '0') * 10 + (date_s[9] - '0');
        int start = hashC[loc1], end = hashC[loc2];
        trainData train = trainInfo.find(hashT[trainID]);
        if (!train.order(start, end)) return 0;
        if (!train.buy(start, end, date, num, ticketKind)) return false;
        long long key = combine(userID, date, hashT[trainID], start, end);
        if (trainInfo.count(key)) {
            ticketData t = ticketInfo.find(key);
            for (int i = 0; i < t.numPrice; i++)
                if (t.priceName[i] == ticketKind) {
                    t.num[i] += num;
                    break;
                }
            ticketInfo.erase(key);
            ticketInfo.insert(key, bridge[0]);
        }
        else
            ticketInfo.insert(key, bridge[0]);
        //printf("key = %lld\n", key);
        return 1;
    }
    void qryTicket() {
        int userID, date;
        str<10> date_s, catalog;
        scanf("%d%s%s", &userID, date_s.ch, catalog.ch);
        date = (date_s[8] - '0') * 10 + (date_s[9] - '0');
        long long key = combine(userID, date);
        //printf("qry key = %lld\n", key);
        long long upper = combine(userID, date + 1);
        //printf("upper key = %lld\n", upper);
        bridgeN = 0;
        for (bptree<long long, ticketData>::iterator t = ticketInfo.lower_bound(key); t.check() && t.retKey() < upper; t++) {
            //printf("new key = %lld\n", t.retKey());
            if (in(t.retValue().catalog.ch[0], catalog))
                bridge[bridgeN++] = t.retValue();
        }
        printf("%d\n", bridgeN);
        for (int i = 0; i < bridgeN; i++)
            bridge[i].print();
    }
    int refund() {
        int userID, num, date;
        str<10> date_s;
        str<20> loc1, loc2, ticketKind;
        scanf("%d%d%s%s%s%s%s", &userID, &num, trainID.ch, loc1.ch, loc2.ch, date_s.ch, ticketKind.ch);
        if (!hashT.count(trainID) || !trainInfo.count(hashT[trainID])) return 0;
        date = (date_s[8] - '0') * 10 + (date_s[9] - '0');
        int start = hashC[loc1], end = hashC[loc2];
        long long key = combine(userID, date, hashT[trainID], start, end);
        if (!ticketInfo.count(key)) return 0;
        ticketData t = ticketInfo.find(key);
        int p = 0;
        while (p < t.numPrice && t.priceName[p] != ticketKind) p++;
        if (p == t.numPrice || t.num[p] < num) return 0;
        ticketInfo.erase(key);
        trainInfo.find(hashT[trainID]).refund(start, end, date, num, p);
        t.num[p] -= num;
        int left = 0;
        for (int i = 0; i < t.numPrice; i++)
            left += t.num[i];
        if (left) ticketInfo.insert(key, t);
        return 1;
    }
};

#endif // !TRAIN_HPP
