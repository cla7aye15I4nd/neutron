#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <string>
#include <sstream>
#include <iostream>
#include "str.hpp"
#include "hash.hpp"
#include "bitmgr.hpp"

extern hashCity hashC;
extern hashTrain hashT;
extern bitManager bitMgr;
extern char bin[20];

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

//store single user's data
struct userData {
    int id;
    str<40> name;
    str<20> password, email, phone;
    int priv;
    
    userData() {}
    userData(int _id, str<40> &_name, str<20> &_password, str<20> &_email, str<20> &_phone, int _priv)
        :id(_id), name(_name), password(_password), email(_email), phone(_phone), priv(_priv) {};
    ~userData() {}
    bool log(str<20> pw) {
        return password == pw;
    }
    void print() {
        printf("%s %s %s %d\n", name.ch, email.ch, phone.ch, priv);
    }
};

//store infomation about one stop station
struct stopInfo {
    int loc, t[3], left[31][5], sumPrice[5];
    str<5> t_s[3];
    stopInfo() {
        for (int date = 1; date < 31; date++)
            for (int i = 0; i < 5; i++)
                left[date][i] = 2000;
    }
};

//store information about saled tickets
struct ticketData {
    str<10> catalog;
    str<100> head;
    int numPrice, num[5], price[5];
    str<20> priceName[5];
    void print() {
        printf("%s", head.ch);
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
    int numStation, numPrice;
    str<20> loc, priceName[5];
    str<40> name;
    stopInfo stop[50];
    int timeParser(str<5> t) {
        if (t[0] == 'x') return -1;
        return (t[0] - '0') * 600 + (t[1] - '0') * 60 + (t[3] - '0') * 10 + (t[4] - '0');
    }
    void datePrint(int date) {
        //TODO change to putchar
        if (date > 30)
            printf("2019-07-01");
        else
            printf("2019-06-%c%c", date / 10 + '0', date % 10 + '0');
    }
    void timePrint(int time) {
        //TODO change to putchar
        printf("%c%c:%c%c", time / 600 + '0', time / 60 % 10 + '0', time % 60 / 10 + '0', time % 10 + '0');
    }
public:
    str<10> catalog;
    str<20> trainID;
    bool saled;
    trainData() {}
    //get basic information about train from stream, get more stop information from std::in
    trainData(str<20> _trainID) :trainID(_trainID), saled(0) {
        str<5> time;
        name.read();
        catalog.read();
        scanf("%d%d", &numStation, &numPrice);
        //printf("%s\n", name.ch);
        for (int i = 0; i < numPrice; i++)
            priceName[i].read();
        int sum[5], t;
        memset(sum, 0, sizeof(sum));
        for (int i = 0; i < numStation; i++) {
            loc.read();
            //printf("%s\n", loc.ch);
            if (!hashC.count(loc)) hashC.insert(loc);
            stop[i].loc = hashC[loc];
            //printf("%d\n", stop[i].loc);
            //printf("%s\n", hashC[stop[i].loc].ch);
            for (int j = 0; j < 2; j++) {
                time.read();
                //printf("%s\n", time.ch);
                stop[i].t[j] = timeParser(time);
                stop[i].t_s[j] = time;
                if (j) {
                    if (i > 0 && stop[i].t[1] < stop[i].t[0])
                    	stop[i].t[j] += 1440;
                }else
                    if (i > 0 && stop[i].t[0] < stop[i-1].t[1])
                    	stop[i].t[j] += 1440;
            }
            time.read();
            stop[i].t[2] = timeParser(time);
            stop[i].t_s[2] = time;
            for (int j = 0; j < numPrice; j++) {
                //char c = getchar();
                //while (c == ' ') c = getchar();
                scanDouble(t);
                //printDouble(t, true);
                //putchar('\n');
                //scanf("%.lf", &t);
                //scanf("%s", bin);
                //printf("%.2f\n", t);
                //printf("%s\n", bin);
                sum[j] += t;
                stop[i].sumPrice[j] = sum[j];
            }
        }
    }
    ~trainData() {}
    //check whether start-stop before end-stop
    bool order(int start, int end) {
        for (int i = 0; i < numStation; i++) {
            if (stop[i].loc == start) return true;
            if (stop[i].loc == end) return false;
        }
    }
    //calc time from start to end
    int duration(int start, int end) {
        for (int i = 0, from = 0; i < numStation; i++) {
            if (stop[i].loc == start) from = stop[i].t[1];
            else if (stop[i].loc == end) return stop[i].t[0] - from;
        }
    }
    //find start time
    int whenStart(int start) {
        for (int i = 0; i < numStation; i++)
            if (stop[i].loc == start) return stop[i].t[1];
    }
    //find end time
    int whenEnd(int end) {
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
            for (int j = 0; j < 3; j++)
                printf(" %s", stop[i].t_s[j].ch);
            for (int j = 0; j < numPrice; j++) {
                putchar(' ');
                printDouble(i == 0 ? stop[i].sumPrice[0] : stop[i].sumPrice[j] - stop[i  - 1].sumPrice[j], true);
            }
            putchar('\n');
        }
    }
    //print detailed information about start-end in date
    void print_detailed(int start, int end, int date) {
        int p = 0;
        while (stop[p].loc != start) p++;
        int q = p, left[5] = {2000, 2000, 2000, 2000, 2000};
        while (stop[q].loc != end) {
            for (int i = 0; i < numPrice; i++)
                if (stop[q].left[date][i] < left[i])
                    left[i] = stop[q].left[date][i];
            q++;
        }
        printf("%s %s ", trainID.ch, hashC[start].ch);
        datePrint(date + (stop[p].t[1] >= 1440));
        putchar(' ');
        printf("%s", stop[p].t_s[1].ch);
        //timePrint(stop[p].t[1]);
        printf(" %s ", hashC[end].ch);
        datePrint(date + (stop[q].t[0] >= 1440));
        putchar(' ');
        printf("%s", stop[q].t_s[0].ch);
        //timePrint(stop[q].t[0]);
        //TODO change to putchar
        for (int i = 0; i < numPrice; i++) {
            printf(" %s %d ", priceName[i].ch, left[i]);
            printDouble(stop[q].sumPrice[i] - stop[p].sumPrice[i]);
        }
        putchar('\n');
    }
    //add newLine to the bitset of along cities
    void update(int newLine) {
        for (int i = 0; i < numStation; i++)
            bitMgr.add(stop[i].loc, newLine);
    }
    //buy ticket
    bool buy(int start, int end, int date, int num, str<20> kind_s) {
        int kind = 0;
        while (kind < numPrice && priceName[kind] != kind_s) kind++;
        if (kind == numPrice) return false;
        int p = 0;
        while (stop[p].loc != start) p++;
        int q = p, left = 2000;
        while (stop[q].loc != end) {
            if (stop[q].left[date][kind] < left)
                left = stop[q].left[date][kind];
            q++;
        }
        if (left < num) return false;
        for (int i = p; i < q; i++)
            stop[i].left[date][kind] -= num;
        bridgeN = 0;
        sprintf(bridge[0].head.ch, "%s %s 2019-06-%c%c %s %s 2019-06-%c%c %s",
                trainID.ch, hashC[start].ch, '0' + date / 10, '0' + date % 10, stop[p].t_s[1].ch, hashC[end].ch,
                '0' + date / 10, '0' + date % 10, stop[q].t_s[0].ch);
        bridge[0].catalog = catalog;
        bridge[0].numPrice = numPrice;
        for (int i = 0; i < numPrice; i++) {
            bridge[0].priceName[i] = priceName[i];
            bridge[0].price[i] = stop[q].sumPrice[i] - stop[p].sumPrice[i];
            if (kind_s == priceName[i]) bridge[0].num[i] = num;
        }
        return true;
    }
    //refund ticket
    void refund(int start, int end, int date, int num, int kind) {
        int p = 0;
        while (stop[p].loc != start) p++;
        int q = p;
        while (stop[q].loc != end) q++;
        for (int i = p; i < q; i++)
            stop[i].left[date][kind] += num;
    }
};

#endif
