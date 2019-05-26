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

//store single user's data
struct userData {
	int id, priv;
	str<20> password, email, phone;
	str<40> name;
	userData();
	userData(int _id, str<40> &_name, str<20> &_password, str<20> &_email, str<20> &_phone, int _priv = 1);
	~userData();
	bool log(str<20> pw);
	void print();
};

/*class dateType {
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
*/

//store infomation about one stop station
struct stopInfo {
	int loc, t[3], left[31][5];
	str<5> t_s[3];
	double sumPrice[5];
	stopInfo();
};

//store information about a train
class trainData {
	int numStation, numPrice;
	str<5> time;
	str<10> catalog;
	str<20> loc, priceName[5];
	str<40> name;
	double t;
	stopInfo stop[60];
	int timeParser(str<5> t);
	void datePrint(int);
	void timePrint(int);
	//str<10> dateForm(int);
	//str<5> timeForm(int);
public:
	str<20> trainID;
	bool saled;
	trainData();
	//get basic information about train from stream, get more stop information from std::in
	trainData(str<20> _trainID);
	~trainData();
	//check whether start-stop before end-stop
	bool order(int start, int end);
	//return compound data of a train, including trainID, name, catalog, station, price and stop information
	void print();
	//print detailed information about start-end in date
	void print_detailed(int start, int end, int date);
	//add newLine to the bitset of along cities
	void update(int newLine);
	//buy ticket
	bool buy(int start, int end, int date, int num, str<20> kind);
	//refund ticket
	void refund(int start, int end, int date, int num, int kind);
};

//store information about saled tickets
struct ticketData {
	str<10> catalog;
	str<100> head;
	int numPrice, num[5];
	str<20> priceName[5];
	double price[5];
	void print();
};

#endif