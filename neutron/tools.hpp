#pragma warning (disable:4996)
#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <string>
#include <sstream>
#include <iostream>

std::stringstream ss;

template <typename T>
std::string toString(T &x) {
	std::stringstream ss;
	std::string str;
	ss << x;
	ss >> str;
	return str;
}

template <size_t len>
class str {
	char ch[len + 1];
public:
	str() {
		ch[0] = '\0';
	}
	str(std::string str) {
		for (int i = 0; i < str.length(); i++)
			ch[i] = str[i];
		ch[str.length()] = '\0';
	}
	bool operator ==(const str &other) {
		for (int i = 0; i < len; i++)
			if (ch[i] != other.ch[i]) return false;
		return true;
	}
	str& operator =(const std::string str) {
		for (int i = 0; i < str.length(); i++)
			ch[i] = str[i];
		ch[str.length()] = '\0';
		return *this;
	}
	std::string toString() {
		std::stringstream ss;
		std::string str;
		ss << ch;
		ss >> str;
		return str;
	}
};
template <size_t len>
std::istream& operator << (std::istream &is, str<len> &obj) {
	std::string str;
	is >> str;
	obj = str;
	return is;
}

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

/*
class timeType {
	int time;
	str<5> str;
};

class stopInfo {
	timeType t[3];
	str<40> name;
	double sumPrice[5];
};


class trainData {
	int numStation, numPrice, left[5];
	str<10> catalog;
	str<20> trainID;
	str<40> name, priceName[5];
	stopInfo stop[60];
public:
	bool saled;
	trainData(str<20> _trainID, std::string str) :trainID(_trainID), saled(0){
		std::stringstream ss;
		ss << str;
		ss >> name >> catalog >> numStation >> numPrice;
		for (int i = 0; i < numPrice; i++)
			ss >> priceName[i];

	}
	~trainData() {}
};

class saledTicket {

};
*/

#endif