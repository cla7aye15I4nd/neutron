#ifndef TRAIN_HPP
#define TRAIN_HPP
/*
#include <string>
#include "tools.h"
#include "BTree.hpp"
#include "hash.hpp"
#include "str.hpp"

extern hash<0> hashC;
extern hash<1> hashT;
extern str<100> bridge;

class train {
	bptree<int, trainData> trainInfo;
	bptree<long long, str<100>> ticketInfo;
	str<20> trainID;
	str<20> idArray_s[200];
	int idArray[200];
public:
	train() {}
	~train() {}
	bool add() {
		scanf("%s", trainID.ch);
		if (hashT.count(trainID)) return 0;
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
	}
	void qry() {
		scanf("%s", trainID.ch);
		trainInfo.find(hashT[trainID]).print();
	}
	int del() {
		scanf("%s", trainID.ch);
		if (!trainInfo.count(hashT[trainID])) return 0;
		if (!(trainInfo.find(hashT[trainID])).saled) return 0;
		trainInfo.erase(hashT[trainID]);
		return 1;
	}
	int mod() {
		scanf("%s", trainID.ch);
		if (!trainInfo.count(hashT[trainID])) return 0;
		if (!(trainInfo.find(hashT[trainID])).saled) return 0;
		trainInfo.erase(hashT[trainID]);
		trainInfo.insert(hashT[trainID], trainData(trainID));
		return 1;
	}
	void clear() {
		hashC.clear();
		hashT.clear();
		//TODO BÊ÷Çå¿Õ
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
		for (int i = 0; i < hashC.used; i++)
			if (can[i]) {
				idArray[cnt] = i;
				trainData tmp = trainInfo.find(i);
				idArray_s[cnt] = tmp.trainID;
				if (tmp.order(start, end)) cnt++;
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
		//TODO
	}
	int buy() {
		int userID, num, date;
		str<10> date_s;
		str<20> trainID, loc1, loc2, ticketKind;
		scanf("%d%d%s%s%s", &userID, &num, trainID.ch, loc1.ch, loc2.ch, date_s, ticketKind);
		if (!hashT.count(trainID) || !trainInfo.count(hashT[trainID])) return 0;
		date = (date_s[8] - '0') * 10 + (date_s[9] - '0');
		int start = hashC[loc1], end = hashC[loc2];
		trainData train = trainInfo.find(hashT[trainID]);
		if (!train.order(start, end)) return 0;
		if (!train.buy(start, end, date, num, ticketKind)) return false;
		ticketInfo.insert(((long long)userID * 100 + date) * 100000 + hashT[trainID], bridge);
	}
	void qryTicket() {
		int userID, date;
		str<10> date_s, catalog;
		scanf("%d%s%s", &userID, date_s, catalog);
		date = (date_s[8] - '0') * 10 + (date_s[9] - '0');
		for (bptree<long long, str<100>>::iterator t = ticketInfo.lower_bound(
			((long long)userID * 100 + date) * 100000);
	}
	int refund() {

	}
};
*/
#endif // !TRAIN_HPP
