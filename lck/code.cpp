#include <iostream>
using namespace std;

int main() {
    char *buf;
    int n;
    cin >> n;
    for (int i = 0; i < n; ++i) {
        *((double *)(buf + sizeof(double) * i)) = i;
    }

    for (int i = 0; i < n; ++i) {
        cout << *((double *)(buf + sizeof(double) * i)) << endl;
    }

    return 0;
}