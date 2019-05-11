#ifndef TRAIN_HPP
#define TRAIN_HPP

#include <string>
#include "tools.hpp"
#include "BTree.hpp"
#include "hash.hpp"

class train {
	bptree<int, trainData> tree;
	str<20> trainID;
	hash Hash;
public:
	train() {}
	~train() {}
	bool add() {
		ss >> trainID;
		if (Hash.count(trainID)) return 0;
		Hash.insert(trainID);
		tree.insert(Hash[trainID], trainData(trainID));
		return 1;
	}
	bool sale() {

		if (!tree.count(Hash[trainID])) return 0;

	}
	std::string qry() {
		ss >> trainID;
		return tree.find(Hash[trainID]).query();
	}
	int del() {
		ss >> trainID;
		if (!tree.count(Hash[trainID])) return 0;
		if (!(tree.find(Hash[trainID])).saled) return 0;
		tree.erase(Hash[trainID]);
		return 1;
	}
	int mod() {
		ss >> trainID;
		if (!tree.count(Hash[trainID])) return 0;
		if (!(tree.find(Hash[trainID])).saled) return 0;
		tree.erase(Hash[trainID]);
		tree.insert(Hash[trainID], trainData(trainID));
		return 1;
	}
	void clear() {
		Hash.clear();
	}
};

#endif // !TRAIN_HPP
