#ifndef TOOLS_H
#define TOOLS_H

#include <string>
#include <sstream>
#include <iostream>
#include "str.hpp"
#include "hash.hpp"
#include "bitManager.hpp"

extern hash<0> hashC;
extern hash<1> hashT;
extern bitManager bitMgr;
extern std::stringstream ss;


//store single user's data
class userData {
public:
	int id, priv;
	str<20> password, email, phone;
	str<40> name;
	userData() {}
	userData(int _id, str<40> &_name, str<20> &_password, str<20> &_email, str<20> &_phone, int _priv = 1);
	userData(int _id, std::string &_name, std::string &_password, std::string &_email, std::string &_phone, int _priv = 1);
	~userData();
	bool log(str<20> pw);
	std::string query();
};

//store time in a day
class timeType {
public:
	int time;
	timeType();
	~timeType();
	operator std::string() const;
	std::string toString() const;
};
//get time
std::istream& operator >> (std::istream &is, timeType &t);

class dateType {
public:
	int date;
	dateType();
	~dateType();
	operator int const{
		return date;
	}
};
//get date
std::istream& operator >> (std::istream &is, dateType &t);

//store infomation about one stop station
struct stopInfo {
	timeType t[3];
	int name;
	double sumPrice[5];
	//return stop_name and three times
	std::string toString();
};
//get stop_name and three times
std::istream& operator >> (std::istream &is, stopInfo &stop);

//store information about a train
class trainData {
	int numStation, numPrice, left[5];
	str<10> catalog;
	str<20> trainID;
	str<40> name, priceName[5];
	stopInfo stop[60];
public:
	bool saled;
	trainData();
	//get basic information about train from stream, get more stop information from std::in
	trainData(str<20> _trainID);
	~trainData();
	//return compound data of a train, including trainID, name, catalog, station, price and stop information
	std::string query();
	//return information between two stops
	std::string query(int start, int end, int date);
	//add newLine to the bitset of along cities
	void update(int newLine);
};

//store information about saled tickets
class saledTicket {

};


#endif