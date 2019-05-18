#include "tools.h"
extern int bridgeN;
extern ticketData bridge[100];

//-----------------------------------------------userData begin
userData::userData() {}
userData::userData(int _id, str<40> &_name, str<20> &_password, str<20> &_email, str<20> &_phone, int _priv)
	:id(_id), name(_name), password(_password), email(_email), phone(_phone), priv(_priv) {};
userData::~userData() {}
bool userData::log(str<20> pw) {
	return password == pw;
}
void userData::print() {
	printf("%s %s %s %d\n", name.ch, email.ch, phone.ch, priv);
}
//-----------------------------------------------userData end

/*
//-----------------------------------------------dateType begin
dateType::dateType() {}
dateType::~dateType() {}
std::istream& operator >> (std::istream &is, dateType &t) {
	std::string s;
	is >> s;
	t.date = (s[8] - '0') * 10 + (s[9] - '0');
}
//-----------------------------------------------dateType end
*/

//-----------------------------------------------stopInfo begin
stopInfo::stopInfo() {
	for (int date = 1; date < 31; date++)
		for (int i = 0; i < 5; i++)
			left[date][i] = 2000;
}
//-----------------------------------------------stopInfo end

//-----------------------------------------------trainData begin
int trainData::timeParser(str<5> t) {
	if (t[0] == 'x') return -1;
	return (t[0] - '0') * 600 + (t[1] - '0') * 60 + (t[2] - '0') * 10 + (t[3] - '0');
}
void trainData::datePrint(int date) {
	//TODO change to putchar
	printf("2019-06-%c%c", date / 10 + '0', date % 10 + '0');
}
void trainData::timePrint(int time) {
	//TODO change to putchar
	printf("%c%c:%c%c", time / 600 + '0', time / 60 % 10 + '0', time % 60 / 10 + '0', time % 10 + '0');
}
/*str<10> trainData::dateForm(int date) {
	str<10> ret;
	ret[0] = '2';
	ret[1] = '0';
	ret[2] = '1';
	ret[3] = '9';
	ret[4] = '-';
	ret[5] = '0';
	ret[6] = '6';
	ret[7] = '-';
	ret[8] = '0' + date / 10;
	ret[9] = '0' + date % 10;
	return ret;
}
str<5> trainData::timeForm(int time) {
	str<5> ret;
	if (time == -1) {
		ret[0] = 'x';
		ret[1] = 'x';
		ret[2] = ':';
		ret[3] = 'x';
		ret[4] = 'x';
	}
	else {
		ret[0] = '0' + time / 600;
		ret[1] = '0' + time / 60 % 10;
		ret[2] = ':';
		ret[3] = '0' + time % 60 / 10;
		ret[4] = '0' + time % 10;
	}
	return ret;
}*/
trainData::trainData() {}
trainData::trainData(str<20> _trainID) :trainID(_trainID), saled(0) {
	scanf("%s%s%d%d", name, catalog, &numStation, &numPrice);
	for (int i = 0; i < numPrice; i++)
		scanf("%s", priceName[i].ch);
	double sum[5], t;
	memset(sum, 0, sizeof(sum));
	for (int i = 0; i < numStation; i++) {
		scanf("%s", loc.ch);
		stop[i].loc = hashC[loc];
		for (int j = 0; j < 3; j++) {
			scanf("%s", time.ch);
			stop[i].t[j] = timeParser(time);
			stop[i].t_s[j] = time;
		}
		for (int j = 0; i < numPrice; i++) {
			scanf("гд%lf", &t);
			sum[j] += t;
			stop[i].sumPrice[j] = sum[j];
		}
	}
}
trainData::~trainData() {}
bool trainData::order(int start, int end) {
	for (int i = 0; i < numStation; i++) {
		if (stop[i].loc == start) return true;
		if (stop[i].loc == end) return false;
	}
}
void trainData::print() {
	printf("%s %s %s %d %d", trainID.ch, name.ch, catalog.ch, numStation, numPrice);
	for (int i = 0; i < numPrice; i++)
		printf(" %d", priceName[i]);
	printf("\n");
	for (int i = 0; i < numStation; i++) {
		printf("%s", hashC[stop[i].loc]);
		for (int j = 0; j < 3; j++)
			printf(" %s", stop[i].t_s[j]);
		printf(" гд%.1f", stop[i].sumPrice[0]);
		for (int j = 1; j < numPrice; j++)
			printf(" гд%.1f", stop[i].sumPrice[j] - stop[i].sumPrice[j - 1]);
		printf("\n");
	}
}
void trainData::print_detailed(int start, int end, int date) {
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
	datePrint(date);
	putchar(' ');
	timePrint(stop[p].t[1]);
	printf(" %s ", hashC[end].ch);
	datePrint(date);
	putchar(' ');
	timePrint(stop[q].t[0]);
	//TODO change to putchar
	for (int i = 0; i < numPrice; i++)
		printf(" %s %d %.2f", priceName[i].ch, left[i], stop[q].sumPrice[i] - stop[p].sumPrice[i]);
	putchar('\n');
}
void trainData::update(int newLine) {
	for (int i = 0; i < numStation; i++)
		bitMgr.add(stop[i].loc, newLine);
}
bool trainData::buy(int start, int end, int date, int num, str<20> kind_s) {
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
		trainID, hashC[start], '0' + date / 10, '0' + date % 10, stop[p].t_s[1], hashC[end], 
		'0' + date / 10, '0' + date % 10, stop[q].t_s[0]);
	bridge[0].catalog = catalog;
	bridge[0].numPrice = numPrice;
	for (int i = 0; i < numPrice; i++) {
		bridge[0].priceName[0] = priceName[i];
		bridge[0].price[i] = stop[q].sumPrice[i] - stop[p].sumPrice[i];
		bridge[0].num[i] = num;
	}
	return true;
}
void trainData::refund(int start, int end, int date, int num, int kind) {
	int p = 0;
	while (stop[p].loc != start) p++;
	int q = p;
	while (stop[q].loc != end) q++;
	for (int i = p; i < q; i++)
		stop[i].left[date][kind] += num;
}
//-----------------------------------------------trainData end

//-----------------------------------------------ticketData begin
void ticketData::print() {
	printf("%s", head.ch);
	for (int i = 0; i < numPrice; i++)
		printf(" %s %d %.3f", priceName[i], num[i], price[i]);
	putchar('\n');
}
//-----------------------------------------------ticketData begin