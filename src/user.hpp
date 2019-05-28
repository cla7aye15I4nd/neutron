#pragma warning(disable:4996)
#ifndef USER_HPP
#define USER_HPP

#include <string>
#include "tools.hpp"
#include "bptree.hpp"

using namespace sjtu;

class user {
	int used = 2018;
	str<40> name;
	str<20> password, email, phone;
	int id, id1, id2, p1, p2, priv;
    userData u;
	FILE *file;
    bptree<int, userData> userInfo;
public:
	user() :userInfo("userDatabase") {
#ifdef IO
	    file = fopen("userFile", "rb");
	    if (file == nullptr) {
            file = fopen("userFile", "wb");
            fclose(file);
        }
        else {
            fread(&used, sizeof(int), 1, file);
            fclose(file);
        }
#endif
	}
	~user() {
#ifdef IO
        file = fopen("userFile", "wb");
        fwrite(&used, sizeof(int), 1, file);
        fclose(file);
#endif
	}
	int reg() {
		used++;
		name.read();
		password.read();
		email.read();
		phone.read();
        userInfo.insert(used, userData(used, name, password, email, phone, 1 + (used == 2019)));
		return used;
	}
	int log() {
		scanf("%d", &id);
		password.read();
		if (!userInfo.count(id)) return 0;
		return (userInfo.find(id)).log(password);
	}
	void qry() {
		scanf("%d", &id);
		if (!userInfo.count(id)) {printf("0\n"); return;}
        userInfo.find(id).print();
	}
	int mod() {
		scanf("%d", &id);
        name.read();
        password.read();
        email.read();
        phone.read();
		if (!userInfo.count(id)) return 0;//TODO optimize: judge out of bound
		priv = userInfo.find(id).priv;
        userInfo.erase(id);
        userInfo.insert(id, userData(used, name, password, email, phone, priv));
		return 1;
	}
	int mop() {
		scanf("%d%d%d", &id1, &id2, &priv);
		if (!userInfo.count(id1) || !userInfo.count(id2)) return 0;
		p1 = (userInfo.find(id1)).priv;
		u = userInfo.find(id2);
		p2 = u.priv;
		if (p1 != 2) return 0;
		if (p2 == 2) return priv == 2;
		if (priv < 2) return priv;
        userInfo.erase(id2);
		u.priv = 2;
        userInfo.insert(id2, u);
		return 1;
	}
	void clear() {
		used = 2018;
        userInfo.clear_file();
	}
};

#endif