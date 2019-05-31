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
	str<20> idArray_s[200], trainID, loc1, loc2, ticketKind;
    str<10> catalog, date_s;
	int idArray[200], h, date, userID, num, start, end, p, left;
    trainData t, _t;
    ticketData tt;
    long long key, upper;
    bitset can, can1, can2;
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
	train() :trainInfo("trainDatabase"), ticketInfo("ticketDatabase") {}
	~train() {}
	bool add() {
	    trainID.read();
		if (hashT.count(trainID)) {
			trainData(trainID);
			return 0;
		}
		hashT.insert(trainID);
		trainInfo.insert(hashT[trainID], trainData(trainID));
		return 1;
	}
	bool sale() {
        trainID.read();
		h = hashT[trainID];
		if (!trainInfo.count(h)) return 0;
		t = trainInfo.find(h);
		if (t.saled) return 0;
		t.saled = 1;
		trainInfo.erase(h);
		trainInfo.insert(h, t);
		t.update(h);
		return 1;
	}
	void qry() {
        trainID.read();
		if (!hashT.count(trainID) || !trainInfo.count(hashT[trainID])) { printf("0\n"); return; }
		trainInfo.find(hashT[trainID]).print();
	}
	int del() {
        trainID.read();
		if (!trainInfo.count(hashT[trainID])) return 0;
		if (trainInfo.find(hashT[trainID]).saled) return 0;
		trainInfo.erase(hashT[trainID]);
		return 1;
	}
	int mod() {
        trainID.read();
		if (!trainInfo.count(hashT[trainID])) return 0;
		if (trainInfo.find(hashT[trainID]).saled) return 0;
		trainInfo.erase(hashT[trainID]);
		trainInfo.insert(hashT[trainID], trainData(trainID));
		return 1;
	}
	void clear() {
		trainInfo.clear_file();
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
		for (int j = 0; j < 5953; j += 64) {
                    if (!can.block[j >> 6]) continue;
                    ull x = can.block[j >> 6];
                    for (int i = j; x; ++i, x >>= 1)
			if (x & 1) {
				idArray[cnt] = i;
				if (!trainInfo.count(i)) {
				    printf("fuck\n");
				    break;
				}
				t = trainInfo.find(i);
				idArray_s[cnt] = t.trainID;
                //printf("can i = %d s = %s\n", i, tmp.trainID.ch);
				if (t.order(start, end) && in(t.catalog[0], catalog)) cnt++;
			}
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
                    t = trainInfo.find(t1);
                    if (t.order(start, tra) && in(t.catalog[0], catalog)) {
                        when = t.whenEnd(tra);
                        d1 = t.duration(start, tra);
                        for (int t2 = 0; t2 < 5953; t2++)
                            if (can2[t2]) {
                                _t = trainInfo.find(t2);
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
		trainInfo.find(mt1ID).print_detailed(start, mtra, date);
		trainInfo.find(mt2ID).print_detailed(mtra, end, date);
	}
	int buy() {
		scanf("%d%d", &userID, &num);
		trainID.read();
		loc1.read();
		loc2.read();
		date_s.read();
		ticketKind.read();
		if (!hashT.count(trainID) || !trainInfo.count(hashT[trainID])) return 0;
		date = (date_s[8] - '0') * 10 + (date_s[9] - '0');
		start = hashC[loc1];
		end = hashC[loc2];
		t = trainInfo.find(hashT[trainID]);
		if (!t.order(start, end)) return 0;
		if (!t.buy(start, end, date, num, ticketKind)) return false;
		trainInfo.modify(hashT[trainID], t);
		key = combine(userID, date, hashT[trainID], start, end);
		if (trainInfo.count(key)) {
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
		return 1;
	}
	void qryTicket() {
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
			bridge[i].print();
	}
	int refund() {
		scanf("%d%d", &userID, &num);
		trainID.read();
		loc1.read();
		loc2.read();
		date_s.read();
		ticketKind.read();
		if (!hashT.count(trainID) || !trainInfo.count(hashT[trainID])) return 0;
		date = (date_s[8] - '0') * 10 + (date_s[9] - '0');
		start = hashC[loc1];
		end = hashC[loc2];
		key = combine(userID, date, hashT[trainID], start, end);
		if (!ticketInfo.count(key)) return 0;
		tt = ticketInfo.find(key);
		p = 0;
		while (p < tt.numPrice && tt.priceName[p] != ticketKind) p++;
		if (p == tt.numPrice || tt.num[p] < num) return 0;
		ticketInfo.erase(key);
		trainInfo.find(hashT[trainID]).refund(start, end, date, num, p);
        tt.num[p] -= num;
		left = 0;
		for (int i = 0; i < tt.numPrice; i++)
			left += tt.num[i];
		if (left) ticketInfo.insert(key, tt);
		return 1;
	}
};

#endif // !TRAIN_HPP
