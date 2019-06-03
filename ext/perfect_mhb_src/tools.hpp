#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <string>
#include <sstream>
#include <iostream>
#include "str.hpp"
#include "bitmgr.hpp"
#include "hash_city.hpp"
#include "ticketnum_mgr.hpp"

extern hashCity hashC;
extern bitManager bitMgr;
extern char bin[20];
extern ticketnum_manager ticketMgr;

void scanDouble(int &x) {
    scanf("%s", bin);
    int p = 0;
    x = 0;
    while (bin[p] < '0' || bin[p] > '9') p++;
    while (bin[p] >= '0' && bin[p] <= '9')
        x = x * 10 + bin[p++] - '0';
    p++;
    for (int i = 0; i < 3; i++)
        if (bin[p] == '\0')
            x = x * 10;
        else
            x = x * 10 + bin[p++] - '0';
}

void printDouble(int x, bool sign = false) {
    char ch[3] = {'\357', '\277', '\245'};
    if (sign)
        printf("%s%d", ch, x / 1000);
    else
        printf("%d", x / 1000);
    x = x % 1000;
	if (x == 0) ;
	else if (x % 100 == 0) printf(".%d", x / 100);
	else if (x % 10 == 0) printf(".%02d", x / 10);
}

void printTime(int time) {
	if (time == -1)
		printf("xx:xx");
	else {
		time %= 1440;
		putchar('0' + time / 600);
		putchar('0' + time / 60 % 10);
		putchar(':');
		putchar('0' + time % 60 / 10);
		putchar('0' + time % 10);
	}
}

//store infomation about one stop station
struct stopInfo {
	short loc, t[3];
	int sumPrice[5];
};

//store information about saled tickets
struct ticketData {
    str<10> catalog;
    int price[5];
    short numPrice, num[5], start, end, startTime, endTime, date;
    str<20> priceName[5];
    void print(str<20> &trainID) {
    	printf("%s %s 2019-06-%c%c ", trainID.ch, hashC[start].ch, '0' + date / 10, '0' + date % 10);
		printTime(startTime);
		printf(" %s 2019-06-%c%c ", hashC[end].ch, '0' + date / 10, '0' + date % 10);
		printTime(endTime);
        for (int i = 0; i < numPrice; i++) {
            printf(" %s %d ", priceName[i].ch, num[i]);
            printDouble(price[i]);
        }
        putchar('\n');
    }
};

extern int bridgeN;
extern ticketData bridge[100];

//store information about a train
class trainData {
	str<20> loc, priceName[5];
	str<40> name;
	stopInfo stop[50];
	short timeParser(str<5> t) {
		if (t[0] == 'x') return -1;
		return (t[0] - '0') * 600 + (t[1] - '0') * 60 + (t[3] - '0') * 10 + (t[4] - '0');
	}
	void datePrint(short date) {
		//TODO change to putchar
		if (date > 30)
		    printf("2019-07-01");
		else
		    printf("2019-06-%c%c", date / 10 + '0', date % 10 + '0');
	}
	void timePrint(short time) {
		//TODO change to putchar
		printf("%c%c:%c%c", time / 600 + '0', time / 60 % 10 + '0', time % 60 / 10 + '0', time % 10 + '0');
	}
public:
    short numStation, numPrice, trainID_h;
    str<10> catalog;
	str<20> trainID;
	bool saled;
	trainData() {}
	//get basic information about train from stream, get more stop information from std::in
	trainData(str<20> _trainID, short _trainID_h) :trainID(_trainID), trainID_h(_trainID_h), saled(false) {
        str<5> time;
        name.read();
        catalog.read();
		scanf("%hd%hd", &numStation, &numPrice);
		for (int i = 0; i < numPrice; i++)
			priceName[i].read();
		int sum[5], t;
		char bin[50];
		memset(sum, 0, sizeof(sum));
		for (int i = 0; i < numStation; i++) {
			loc.read();
            if (!hashC.count(loc)) hashC.insert(loc);
            stop[i].loc = hashC[loc];
			for (int j = 0; j < 2; j++) {
				time.read();
				stop[i].t[j] = timeParser(time);
				if (j) {
                	if (i > 0 && stop[i].t[1] < stop[i].t[0])
                    	stop[i].t[j] += 1440;
				}else
					if (i > 0 && stop[i].t[0] < stop[i - 1].t[1])
                    	stop[i].t[j] += 1440;
			}
            time.read();
            stop[i].t[2] = timeParser(time);
			for (int j = 0; j < numPrice; j++) {
				scanDouble(t);
				sum[j] += t;
				stop[i].sumPrice[j] = sum[j];
			}
		}
	}
	~trainData() {}
	//check whether start-stop before end-stop
	bool order(short start, short end) {
		for (int i = 0; i < numStation; i++) {
			if (stop[i].loc == start) return true;
			if (stop[i].loc == end) return false;
		}
	}
	//calc time from start to end
	short duration(short start, short end) {
        short from;
        for (int i = 0; i < numStation; i++) {
            if (stop[i].loc == start) from = stop[i].t[1];
            if (stop[i].loc == end) return stop[i].t[0] - from;
        }
	}
    //find start time
    short whenStart(short start) {
        for (int i = 0; i < numStation; i++)
            if (stop[i].loc == start) return stop[i].t[1];
    }
    //find end time
    short whenEnd(short end) {
        for (int i = 0; i < numStation; i++)
            if (stop[i].loc == end) return stop[i].t[0];
    }
    //return compound data of a train, including trainID, name, catalog, station, price and stop information
	void print() {
	    if (!saled) {printf("0\n"); return;}
		printf("%s %s %s %d %d", trainID.ch, name.ch, catalog.ch, numStation, numPrice);
		for (int i = 0; i < numPrice; i++)
			printf(" %s", priceName[i].ch);
		printf("\n");
		for (int i = 0; i < numStation; i++) {
		    //printf("%d ", stop[i].loc);
			printf("%s", hashC[stop[i].loc].ch);
			for (int j = 0; j < 3; j++) {
				putchar(' ');
				printTime(stop[i].t[j]);
			}
			for (int j = 0; j < numPrice; j++) {
			    putchar(' ');
                printDouble(i == 0 ? stop[i].sumPrice[0] : stop[i].sumPrice[j] - stop[i  - 1].sumPrice[j], true);
			}
			putchar('\n');
		}
	}
	int det(int stopN, int date, int priceN) {
		return stopN * 150 + date * 5 + priceN;
	}
	//print detailed information about start-end in date
    void print_detailed(short start, short end, short date, bool mhb, bool brief) {
        int p = 0;
        while (stop[p].loc != start) p++;
        short *oc;
        ticketMgr.getTicketNum(trainID_h, oc);
        int q = p, occupied[5] = { 0, 0, 0, 0, 0 };
        while (stop[q].loc != end) {
            for (int i = 0; i < numPrice; i++)
                if (oc[det(q, date, i)] > occupied[i])
                    occupied[i] = oc[det(q, date, i)];
            q++;
        }
        if (mhb) {
            if (brief) {
                for (int i = 0; i < numPrice; i++)
                    if (occupied[i] < 2000) { brief = false; break; }
                if (brief) return;
            }
            printf("%s %s ", trainID.ch, hashC[start].ch);
            printTime(stop[p].t[1]);
            printf(" %s ", hashC[end].ch);
            printTime(stop[q].t[0]);
            printf(" %d ", stop[p].t[1] < 1440 && stop[q].t[0] >= 1440);
            int det = stop[q].t[0] - stop[p].t[1];
            if (det >= 1440) printf("%dd", det / 1440);
            det %= 1440;
            if (det >= 60) printf("%dh", det / 60);
            det %= 60;
            printf("%dm", det);
            putchar('\n');
            printf("%d\n", numPrice);
            for (int i = 0; i < numPrice; i++) {
                printf("%s %d ", priceName[i].ch, 2000 - occupied[i]);
                printDouble(stop[q].sumPrice[i] - stop[p].sumPrice[i]);
                putchar('\n');
            }
        }
        else {
            printf("%s %s ", trainID.ch, hashC[start].ch);
            datePrint(date + (stop[p].t[1] >= 1440));
            putchar(' ');
            printTime(stop[p].t[1]);
            //printf("%s", stop[p].t_s[1].ch);
            printf(" %s ", hashC[end].ch);
            datePrint(date + (stop[q].t[0] >= 1440));
            putchar(' ');
            printTime(stop[q].t[0]);
            //printf("%s", stop[q].t_s[0].ch);
            //TODO change to putchar
            for (int i = 0; i < numPrice; i++) {
                printf(" %s %d ", priceName[i].ch, 2000 - occupied[i]);
                printDouble(stop[q].sumPrice[i] - stop[p].sumPrice[i]);
            }
            putchar('\n');
        }
    }
	//add newLine to the bitset of along cities
	void update(int newLine) {
		for (int i = 0; i < numStation; i++)
			bitMgr.add(stop[i].loc, newLine);
	}
	//buy ticket
	//TODO:finish change in ticketnum
	bool buy(short start, short end, short date, short num, str<20> kind_s) {
		int kind = 0;
		while (kind < numPrice && priceName[kind] != kind_s) kind++;
		if (kind == numPrice) return false;
		int p = 0;
		while (stop[p].loc != start) p++;
		int q = p;
		short *oc;
        ticketMgr.getTicketNum(trainID_h, oc);
		short occupied = 0;
		while (stop[q].loc != end) {
			if (oc[det(q, date, kind)] < occupied)
				occupied = oc[det(q, date, kind)];
			q++;
		}
		if (occupied + num > 2000) return false;
		for (int i = p; i < q; i++)
			oc[det(i, date, kind)] += num;
		bridgeN = 0;
		bridge[0].start = start;
		bridge[0].end = end;
		bridge[0].startTime = stop[p].t[1];
		bridge[0].endTime = stop[q].t[0];
		bridge[0].catalog = catalog;
		bridge[0].numPrice = numPrice;
		bridge[0].date = date;
		for (int i = 0; i < numPrice; i++) {
			bridge[0].priceName[i] = priceName[i];
			bridge[0].price[i] = stop[q].sumPrice[i] - stop[p].sumPrice[i];
			if (kind_s == priceName[i]) bridge[0].num[i] = num;
		}
		return true;
	}
	//refund ticket
	void refund(short start, short end, short date, short num, int kind) {
		int p = 0;
		while (stop[p].loc != start) p++;
		int q = p;
		while (stop[q].loc != end) q++;
		short *oc;
        ticketMgr.getTicketNum(trainID_h, oc);
		for (int i = p; i < q; i++)
			oc[det(i, date, kind)] -= num;
	}
};
#endif
