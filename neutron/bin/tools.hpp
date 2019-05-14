#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <string>
#include <sstream>
#include <iostream>
#include "hash.hpp"
#include "bitManager.hpp"

extern hash<0> hashC;
extern hash<1> hashT;
extern bitManager bitMgr;

std::stringstream ss;





class userData {
public:
	int id, priv;
	str<20> password, email, phone;
	str<40> name;
	userData() {}
	userData(int _id, str<40> &_name, str<20> &_password, str<20> &_email, str<20> &_phone, int _priv = 1)
		:id(_id), name(_name), password(_password), email(_email), phone(_phone), priv(_priv) {};
	userData(int _id, std::string &_name, std::string &_password, std::string &_email, std::string &_phone, int _priv = 1)
		:id(_id), name(str<40>(_name)), password(str<20>(_password)), email(str<20>(_email)), phone(str<20>(_phone)), priv(_priv) {};
	~userData() {}
	bool log(str<20> pw) {
		return password == pw;
	}
	std::string query() {
		return name.toString() + " " + email.toString() + " " + phone.toString() + " " + toString(priv);
	}
};

class timeType {
public:
	int time;
	timeType() {}
	~timeType() {}
	operator std::string () const{
		return this->toString();
	}
	std::string toString() const{
		if (time == -1) return "xx:xx";
		return "" + ('0' + time / 600) + ('0' + time / 60 % 10) + ':' + ('0' + time % 60 / 10) + ('0' + time % 10);
	}
};
std::istream& operator >> (std::istream &is, timeType &t) {
	std::string s;
	is >> s;
	if (s[0] == 'x')
		t.time = -1;
	else
		t.time = ((s[0] - '0') * 10 + (s[1] - '0')) * 60 + (s[2] - '0') * 10 + (s[3] - '0');
	return is;
}

struct stopInfo {
	timeType t[3];
	str<40> name;
	double sumPrice[5];
	std::string toString() {
		std::string ret;
		ret = name.toString() + ' ' + t[0].toString + ' ' + t[1].toString() + ' ' + t[2].toString();
	}
};
std::istream& operator >> (std::istream &is, stopInfo &stop) {
	is >> stop.name >> stop.t[0] >> stop.t[1] >> stop.t[2];
	return is;
}


class trainData {
	int numStation, numPrice, left[5];
	str<10> catalog;
	str<20> trainID;
	str<40> name, priceName[5];
	stopInfo stop[60];
public:
	bool saled;
	trainData(str<20> _trainID) :trainID(_trainID), saled(0){
		ss >> name >> catalog >> numStation >> numPrice;
		for (int i = 0; i < numPrice; i++)
			ss >> priceName[i];
		double sum[5], t;
		memset(sum, 0, sizeof(sum));
		std::string s;
		for (int i = 0; i < numStation; i++) {
			std::cin >> stop[i];
			for (int j = 0; i < numPrice; i++) {
				scanf("гд%lf", t);
				sum[j] += t;
				stop[i].sumPrice[j] = sum[j];
			}
		}
	}
	~trainData() {}
	std::string query() {
		std::string ret;
		ret = trainID.toString() + ' ' + name.toString() + ' ' + catalog.toString() + ' ' + toString(numStation) + ' ' + toString(numPrice);
		for (int i = 0; i < numPrice; i++)
			ret += ' ' + priceName[i].toString();
		for (int i = 0; i < numStation; i++) {
			ret += '\n' + stop[i].toString();
			for (int j = 0; j < numPrice; j++)
				ret += " гд" + toString(stop[i].sumPrice[j]);
		}
		return ret;
	}
};

class saledTicket {

};


#endif