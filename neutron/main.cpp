#pragma warning(disable:4996)
#include "saleSystem.hpp"

hash<0> hashC;
hash<1> hashT;
bitManager bitMgr;
int bridgeN = 0;
ticketData bridge[100];

saleSystem sys;
int main() {
	freopen("all.in", "r", stdin);
	freopen("all.out", "w", stdout);
	sys.processing();
	return 0;
}