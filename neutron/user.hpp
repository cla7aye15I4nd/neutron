#ifndef USER_HPP
#define USER_HPP

#include <string>
#include "tools.hpp"
#include "BTree.hpp"
using namespace sjtu;

class user {
	int used = 2018;
	std::string name, password, email, phone;
	int id;
	FILE *file;
	bptree<int, userData> tree;
public:
	user() {
		file = fopen("userFile", "rb");
		if (file == nullptr) {
			file = fopen("userFile", "wb");
			fclose(file);
		}
		else {
			file = fopen("userFile", "rb");
			fread(&used, sizeof(int), 1, file);
			fclose(file);
		}
	}
	~user() {
		file = fopen("userFile", "wb");
		fclose(file);
	}
	int reg() {
		used++;
		ss >> name >> password >> email >> phone;
		tree.insert(used, userData(used, name, password, email, phone, 1 + (used == 2019)));
		return used;
	}
	int log() {
		ss >> id >> password;
		if (!tree.count(id)) return 0;
		return (tree.find(id)).log(str<20>(password));
	}
	std::string qry() {
		ss >> id;
		if (!tree.count(id)) return "0";
		return (tree.find(id)).query();
	}
	int mod() {
		ss >> id >> name >> password >> email >> phone;
		if (!tree.count(id)) return 0;//optimize: judge out of bound
		int priv = tree.find(id).priv;
		tree.erase(id);
		tree.insert(id, userData(used, name, password, email, phone, priv));
		return 1;
	}
	int mop() {
		int id1, id2, priv;
		ss >> id1 >> id2 >> priv;
		if (!tree.count(id1) || !tree.count(id2)) return 0;
		int p1 = (tree.find(id1)).priv;
		userData u = tree.find(id2);
		int p2 = u.priv;
		if (p1 != 2) return 0;
		if (p2 == 2) return priv == 2;
		if (priv < 2) return priv;
		tree.erase(id2);
		u.priv = 2;
		tree.insert(id2, u);
		return 1;
	}
};

#endif