#ifndef TRAIN_HPP
#define TRAIN_HPP

#include <string>
#include "tools.hpp"
#include "bptree.hpp"
#include "hash_train.hpp"
#include "str.hpp"

using namespace sjtu;

extern hashCity hashC;
extern hashTrain hashT;
extern int bridgeN;
extern ticketData bridge[100];

class train {
	bptree<long long, ticketData> ticketInfo;
	str<20> idArray_s[200], trainID, loc1, loc2, ticketKind;
    str<10> catalog, date_s;
	int idArray[200], h, date, userID, num, start, end, p, left;
    trainData t, _t;
    ticketData tt;
    long long key, upper;
    bitset can, can1, can2;
    bool virgin = true;
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
	void init() {
	    ticketInfo.init("ticket_Bptree");
	}
	void write_back() {
        ticketInfo.write_back();
	}
	bool add() {
	    trainID.read();
		if (hashT.count(trainID)) {
			trainData(trainID);
			return false;
		}
		hashT.insert(trainID);
		hashT.insertFile(hashT[trainID], trainData(trainID, hashT[trainID]), true);
		return true;
	}
	bool sale() {
        trainID.read();
        if (!hashT.count(trainID)) return false;
		h = hashT[trainID];
		hashT.getTrainData(h, t);
		if (t.saled) return false;
		t.saled = 1;
		hashT.insertFile(h, t);
		t.update(h);
		return true;
	}
	void qry() {
        trainID.read();
		if (!hashT.count(trainID)) { printf("0\n"); return; }
		hashT.getTrainData(hashT[trainID], t);
		t.print();
	}
	bool del() {
        trainID.read();
		if (!hashT.count(trainID)) return false;
		h = hashT[trainID];
		hashT.getTrainData(h, t);
		if (t.saled) return false;
		//TODO
		//hashT.erase(h);
		return true;
	}
	bool mod() {
        trainID.read();
		if (!hashT.count(trainID)) return false;
		h = hashT[trainID];
		hashT.getTrainData(h, t);
		if (t.saled) return false;
		hashT.insertFile(h, trainData(trainID, h), true);
		return true;
	}
	void clear() {
		hashT.clear();
		ticketInfo.clear_file();
	}
	void qryDirect() {
	    loc1.read();
	    loc2.read();
	    date_s.read();
	    catalog.read();
		date = (date_s[8] - '0') * 10 + (date_s[9] - '0');
		if (!hashC.count(loc1) || !hashC.count(loc2)) { printf("-1\n"); return; }
		start = hashC[loc1];
		end = hashC[loc2];
		can = bitMgr.intersection(start, end);
		int cnt = 0;
		for (int i = 0; i < 5953; i++)
			if (can[i]) {
				idArray[cnt] = i;
				hashT.getTrainData(i, t);
				idArray_s[cnt] = t.trainID;
                //printf("can i = %d s = %s\n", i, tmp.trainID.ch);
				if (t.order(start, end) && in(t.catalog[0], catalog)) cnt++;
			}
		//TODO: optimize the damn sort
		for (int i = 0; i < cnt; i++)
			for (int j = i + 1; j < cnt; j++)
				if (idArray_s[j] < idArray_s[i]) {
					std::swap(idArray[i], idArray[j]);
					std::swap(idArray_s[i], idArray_s[j]);
				}
		printf("%d\n", cnt);
		for (int i = 0; i < cnt; i++) {
			hashT.getTrainData(idArray[i], t);
			t.print_detailed(start, end, date);
		}
	}
	void qryTrans() {
        loc1.read();
        loc2.read();
        date_s.read();
        catalog.read();
		date = (date_s[8] - '0') * 10 + (date_s[9] - '0');
		if (!hashC.count(loc1) || !hashC.count(loc2)) { printf("-1\n"); return; }
		start = hashC[loc1];
		end = hashC[loc2];
		int mtra = -1, mt1ID, mt1Time = 9999, mt2ID, mt2Time = 9999, when, d1, d2;
		for (int tra = 0; tra < hashC.used; tra++) {
            if (tra == start || tra == end) continue;
            can1 = bitMgr.intersection(start, tra);
            can2 = bitMgr.intersection(tra, end);
            for (int t1 = 0; t1 < 5953; t1++)
                if (can1[t1]) {
                	hashT.getTrainData(t1, t);
                    if (t.order(start, tra) && in(t.catalog[0], catalog)) {
                        when = t.whenEnd(tra);
                        d1 = t.duration(start, tra);
                        for (int t2 = 0; t2 < 5953; t2++)
                            if (can2[t2]) {
                            	hashT.getTrainData(t2, _t);
                                if (_t.order(tra, end) && in(_t.catalog[0], catalog) && _t.whenStart(tra) >= when){
                                    d2 = _t.duration(tra, end);
                                    if (d1 + d2 < mt1Time + mt2Time) {
                                        mt1ID = t1;
                                        mt2ID = t2;
                                        mt1Time = d1;
                                        mt2Time = d2;
                                        mtra = tra;
                                    }
                                }
                            }
                    }
                }
		}
		if (mtra == -1) {printf("-1\n"); return; }
		hashT.getTrainData(mt1ID, t);
		t.print_detailed(start, mtra, date);
		hashT.getTrainData(mt2ID, t);
		t.print_detailed(mtra, end, date);
	}
	bool buy() {
        if (virgin) {ticketInfo.init("Bptree:ticketDatabase"); virgin = false;}
		scanf("%d%d", &userID, &num);
		trainID.read();
		loc1.read();
		loc2.read();
		date_s.read();
		ticketKind.read();
		if (!hashT.count(trainID)) return false;
		date = (date_s[8] - '0') * 10 + (date_s[9] - '0');
		start = hashC[loc1];
		end = hashC[loc2];
		h = hashT[trainID];
		hashT.getTrainData(h, t);
		if (!t.order(start, end)) return false;
		if (!t.buy(start, end, date, num, ticketKind)) return false;
		key = combine(userID, date, h, start, end);
		if (ticketInfo.count(key)) {
			tt = ticketInfo.find(key);
			for (int i = 0; i < tt.numPrice; i++)
				if (tt.priceName[i] == ticketKind) {
                    tt.num[i] += num;
					break;
				}
			ticketInfo.erase(key);
			ticketInfo.insert(key, bridge[0]);
		}
		else
			ticketInfo.insert(key, bridge[0]);
		//printf("key = %lld\n", key);
		return true;
	}
	void qryTicket() {
        if (virgin) {ticketInfo.init("Bptree:ticketDatabase"); virgin = false;}
		scanf("%d", &userID);
		date_s.read();
		catalog.read();
		date = (date_s[8] - '0') * 10 + (date_s[9] - '0');
		key = combine(userID, date);
		//printf("qry key = %lld\n", key);
		upper = combine(userID, date + 1);
		//printf("upper key = %lld\n", upper);
		bridgeN = 0;
		for (bptree<long long, ticketData>::iterator t = ticketInfo.lower_bound(key); t.check() && ticketInfo.retKey(t) < upper; ticketInfo.next_iterator(t)) {
		    //printf("new key = %lld\n", t.retKey());
            if (in(ticketInfo.retValue(t).catalog.ch[0], catalog))
                bridge[bridgeN++] = ticketInfo.retValue(t);
		}
		printf("%d\n", bridgeN);
		for (int i = 0; i < bridgeN; i++)
			bridge[i].print(trainID);
	}
	bool refund() {
        if (virgin) {ticketInfo.init("Bptree:ticketDatabase"); virgin = false;}
		scanf("%d%d", &userID, &num);
		trainID.read();
		loc1.read();
		loc2.read();
		date_s.read();
		ticketKind.read();
		if (!hashT.count(trainID)) return false;
		date = (date_s[8] - '0') * 10 + (date_s[9] - '0');
		start = hashC[loc1];
		end = hashC[loc2];
		h = hashT[trainID];
		key = combine(userID, date, h, start, end);
		if (!ticketInfo.count(key)) return false;
		tt = ticketInfo.find(key);
		p = 0;
		while (p < tt.numPrice && tt.priceName[p] != ticketKind) p++;
		if (p == tt.numPrice || tt.num[p] < num) return false;
		ticketInfo.erase(key);
		hashT.getTrainData(h, t);
		t.refund(start, end, date, num, p);
        tt.num[p] -= num;
		left = 0;
		for (int i = 0; i < tt.numPrice; i++)
			left += tt.num[i];
		if (left) ticketInfo.insert(key, tt);
		return true;
	}
};

#endif // !TRAIN_HPP
