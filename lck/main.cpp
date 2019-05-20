#include <iostream>
#include <cstring>
#include <cstdio>
#include <map>
#include "bptree.hpp"
#include <vector>

using std::cin;
using std::cout;
using std::string;
using std::endl;

sjtu::bptree<int, int> tree;
std::map<int, int> mp;
std::vector<int> v1;
std::vector<int> v2;
const int n = 10000;

long long aa = 13131, bb = 5353, MOD = (long long) (1e9 + 7), now = 1;

int rand() {
    for (int i = 1; i < 3; i++)
        now = (now * aa + bb) % MOD;
    return now;
}

void make_vector() {
    for (int i = 0; i <= n; ++i) {
        v1.push_back(rand());
        v2.push_back(rand());
    }
}

void test_insert() {
    puts("Test: insert");
    for (int i = 0; i <= n; ++i) {
        tree.insert(i, -i);
    }

    puts("Test insert passed!");
}

void test_insert_random() {
    puts("Test: insert randomly");
    for (int i = 0; i <= n; ++i) {
        tree.insert(v1[i], v2[i]);
        mp[v1[i]] = v2[i];
    }
    puts("Test insert passed!");
}

void test_count() {
    puts("Test count");
    for (int i = 0; i <= n; ++i) {
        if (!tree.count(i)) {
            puts("count error!");
            return;
        }
    }

    puts("Test count passed!");
}


void test_find() {
    puts("Test: find");
    for (int i = 0; i <= n; ++i) {
        if (tree.find(i) != -i) {
            puts("find error!");
            return;
        }
    }
    puts("Test find passed!");
}

void test_find_random() {
    puts("Test: find");
    for (int i = 0; i <= n; ++i) {
        if (tree.find(v1[i]) != mp[v1[i]]) {
            puts("find error!");
            return;
        }
    }
    puts("Test find passed!");
}

void test_erase() {
    puts("Test erase");
    for (int i = 0; i <= n; ++i) {
        tree.erase(i);
    }

    puts("Test erase passed!");
}

void test_erase_random() {
    puts("Test erase");
    for (int i = 0; i <= n; ++i) {
        tree.erase(v1[i]);
    }

    puts("Test erase passed!");
}

void test_iterator() {
    puts("Test iterator");
    sjtu::bptree<int, int>::iterator it;
    it = tree.lower_bound(5000);

    while (it.check()) {
//        cout << it.retKey() << ':' << it.retValue() << endl;
        ++it;
    }

    puts("test of iterator passed");
}

void test_iterator_random() {
    puts("Test iterator");
    sjtu::bptree<int, int>::iterator it;
    it = tree.lower_bound(v1[1000]);

    while (it.check()) {
//        cout << it.retKey() << ':' << it.retValue() << endl;
        if (mp[it.retKey()] != it.retValue()) {
            puts("iterator error!");
            return;
        }

        ++it;
    }

    puts("test of iterator passed");
}

void test1() {
    test_insert();
    test_count();
    test_find();
    test_iterator();

    test_erase();
    tree.view_root();
    puts("simple test over");
}

void test2() {
    make_vector();
    test_insert_random();
    test_find_random();
    test_iterator_random();

    test_erase_random();
    tree.view_root();

    puts("Random test over");
}

int main() {
//    test1();

    test2();
}