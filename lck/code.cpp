#include <iostream>
#include <cstdio>
#include <fstream>
#include <cmath>
using namespace std;
long long aa = 13131, bb = 5353, MOD = (long long) (1e9 + 7), now = 1;

int rand() {
    for (int i = 1; i < 3; i++)
        now = (now * aa + bb) % MOD;
    return now;
}

int a[100000000000] = {0};

int main() {
    for (int i  = 0; i < 3000000; ++i) {
        if (a[rand()] == 0)
            a[rand()] = 1;
        else {
            puts("error");
            cout << i << endl;
            break;
        }
        cout << i << endl;
    }
}