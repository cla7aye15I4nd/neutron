#include <iostream>
#include <cstring>
#include <cstdio>
#include <map>
#include "BTree.hpp"
using std::cin;
using std::cout;
using std::string;
using std::endl;

sjtu::bptree <int, int> tree;
std::map <int, int> mp;
const int n = 5001;

void test_insert() {
    puts("Test: insert");
    //insert even number
    for (int i = 0; i < n; i += 2) {
        tree.insert(i, -i);
    }
    //insert odd number
    for (int i = n; i > 0; i -= 2) {
        tree.insert(i, -i);
    }

    puts("Test insert passed!");
}

void test_insert_random() {
    puts("Test: insert");
    for (int i = 0; i < n; ++i) {
        int tmp(rand());
        tree.insert(i, tmp);
        mp[i] = tmp;
    }
    puts("Test insert passed!");
}

void test_count() {
    puts("Test count");
    for (int i = 0; i < n; i += 2) {
        if (!tree.count(i)) {
            puts("count error!");
            return;
        }
    }

    puts("Test count passed!");
}


void test_find() {
    puts("Test: find");
    for (int i = 0; i <= n; i++) {
        if (tree.find(i) != -i) {
            puts("find error!");
            return;
        }
    }
//    for (int i = n; i > 0; i -= 2) {
//        if (tree.find(i) != -i) {
//            puts("find error!");
//            return;
//        }
//    }
    puts("Test find passed!");
}

void test_find_random() {
    puts("Test: find");
    for (int i = 0; i < n; ++i) {
        if (tree.find(i) != mp[i]) {
            puts("find error!");
            return;
        }
    }
    puts("Test find passed!");
}

void test_erase() {
    puts("Test erase");
    for (int i = 0; i < n; ++i) {
        tree.erase(i);
    }

    puts("Test erase passed!");
}



int main() {
//    test_insert();
    test_insert_random();
//    test_count();
//    test_find();
    test_find_random();
    test_erase();

    puts("Test Over");
}