#pragma warning(disable:4996)
#include "saleSystem.hpp"

hashCity hashC;
hashTrain hashT;
bitManager bitMgr;
int bridgeN = 0;
ticketData bridge[100];
char bin[20];
trainData temp;

saleSystem sys;
int main() {
    //fprintf(stderr, "%d\n", sizeof(temp));
    double begin = clock();
    //freopen("all.in", "r", stdin);
	//freopen("all.out", "w", stdout);
	sys.processing();
    //fprintf(stderr, "%f\n", (clock() - begin) / CLOCKS_PER_SEC);
	return 0;
}
