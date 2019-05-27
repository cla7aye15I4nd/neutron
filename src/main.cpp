#include "saleSystem.hpp"

bool isLinux = true;
bool debug = true;
hashCity hashC;
hashTrain hashT;
bitManager bitMgr;
int bridgeN = 0;
ticketData bridge[100];

saleSystem sys;
int main() {
    double begin = clock();
    sys.processing();
    fprintf(stderr, "%f\n", (clock() - begin) / CLOCKS_PER_SEC);
    return 0;
}
