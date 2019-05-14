#ifndef TRAIN_HPP
#define TRAIN_HPP

#include <string>
#include "tools.h"
#include "BTree.hpp"
#include "hash.hpp"
#include "str.hpp"

extern hash<0> hashC;
extern hash<1> hashT;

class train {
	bptree<int, trainData> trainInfo;
	str<20> trainID;
	str<20> idArray[200];
	std::string trainArray[200];
public:
	train() {}
	~train() {}
	bool add() {
		ss >> trainID;
		if (hashT.count(trainID)) return 0;
		hashT.insert(trainID);
		trainInfo.insert(hashT[trainID], trainData(trainID));
		return 1;
	}
	bool sale() {
		ss >> trainID;
		int h = hashT[trainID];
		if (!trainInfo.count(h)) return 0;
		trainData t = trainInfo.find(h);
		if (t.saled) return 0;
		t.saled = 1;
		trainInfo.erase(h);
		trainInfo.insert(h, t);
		t.update(h);
	}
	std::string qry() {
		ss >> trainID;
		return trainInfo.find(hashT[trainID]).query();
	}
	int del() {
		ss >> trainID;
		if (!trainInfo.count(hashT[trainID])) return 0;
		if (!(trainInfo.find(hashT[trainID])).saled) return 0;
		trainInfo.erase(hashT[trainID]);
		return 1;
	}
	int mod() {
		ss >> trainID;
		if (!trainInfo.count(hashT[trainID])) return 0;
		if (!(trainInfo.find(hashT[trainID])).saled) return 0;
		trainInfo.erase(hashT[trainID]);
		trainInfo.insert(hashT[trainID], trainData(trainID));
		return 1;
	}
	void clear() {
		hashC.clear();
		hashT.clear();
		//BÊ÷Çå¿Õ
	}
	std::string qryDirect() {
		str<20> loc1, loc2;
		dateType date;
		str<10> catalog;
		std::string s;
		ss >> loc1 >> loc2 >> date >> catalog;
		if (!hashC.count(loc1) || !hashC.count(loc2)) return "-1";
		bitset can = bitMgr.intersection(loc1, loc2);
		int cnt = 0;
		for (int i = 0; i < hashC.used; i++)
			if (can[i]) {
				trainArray[cnt] = trainInfo.find(i).query(hashC[loc1], hashC[loc2], date.date);
				ss.str(s);
				ss >> idArray[cnt++];
			}
	}
	std::string qryTrans() {

	}
};

#endif // !TRAIN_HPP
