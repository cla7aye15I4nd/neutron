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

sjtu::bptree<int, long long> tree("aaa");
std::map<int, int> mp;
std::vector<int> v1;
std::vector<int> v2;
const int n = 200000;

long long aa = 13131, bb = 5353, MOD = (long long) (1e9 + 7), now = 1;

int rand() {
    for (int i = 1; i < 3; i++)
        now = (now * aa + bb) % MOD;
    return now;
}

void make_vector() {
    for (int i = 1; i <= n + 1; ++i) {
        v1.push_back(rand());
        v2.push_back(rand());
    }
    for (int i = 0; i <= n; ++i) {
        mp[v1[i]] = v2[i];
    }
}

void test_insert() {
    puts("Test: insert");
    for (int i = 1; i <= n; ++i) {
//        cout << "insert:" << i << endl;
        tree.insert(i, -i);
//        tree.view_root();
    }

    puts("Test insert passed!");
}

void test_insert_random() {
    puts("Test: insert randomly");
    for (int i = n; i >= 1; --i) {
        tree.insert(v1[i], v2[i]);
//        tree.view_root();
    }
    puts("Test insert passed!");
}


void test_find() {
    puts("Test: find");
    for (int i = 1; i <= n; ++i) {
//        cout << i << endl;
        if (tree.find(i) != -i) {
            cerr << "find error in " << tree.find(i) << endl;
            return;
        }
    }
    puts("Test find passed!");
}

void test_find_random() {
    puts("Test: find random");
    for (int i = 1; i <= n; ++i) {
        if (tree.find(v1[i]) != mp[v1[i]]) {
            puts("find error!");
            return;
        }
    }
    puts("Test find passed!");
}

void test_erase() {
    puts("Test erase");
    bool flag;
    for (int i = 1; i <= n; ++i) {
        flag = tree.erase(i);
        if (!flag) {
            cerr << "error in erase " << i << endl;
            return;
        }
    }

    puts("Test erase passed!");
}

void test_erase_random() {
    puts("Test erase random");
    bool flag;
    for (int i = 1; i <= n; ++i) {
        flag = tree.erase(v1[i]);
        if (!flag) {
            cerr << "error in erase " << i << endl;
            return;
        }
    }

    puts("Test erase passed!");
}

void test_iterator() {
    puts("Test iterator");
    sjtu::bptree<int, long long>::iterator it;
    it = tree.lower_bound(500);

    while (it.check()) {
//        cout << tree.retKey(it) << ':' << tree.retValue(it) << endl;
        tree.next_iterator(it);
        if (tree.retKey(it) != -tree.retValue(it)) {
            puts("iterator error");
            return;
        }
    }

    puts("test of iterator passed");
}

void test_iterator_random() {
    puts("Test iterator random");
    sjtu::bptree<int, long long>::iterator it;
    it = tree.lower_bound(v1[200]);

    while (it.check()) {
        if (mp[tree.retKey(it)] != tree.retValue(it)) {
            puts("iterator error!");
            return;
        }

        tree.next_iterator(it);
    }

    puts("test of iterator passed");
}

void test1() {
    test_insert();
//    tree.view_root();
//    tree.traverse();
    test_find();
    test_iterator();

    test_erase();

    tree.view_root();
    puts("simple test over");
}

void test2() {
    make_vector();
    test_insert_random();
//    tree.traverse();
    test_find_random();
    test_iterator_random();
    test_erase_random();
//
    tree.view_root();

    puts("Random test over");
}

int keys[n + 1];
long long vals[n + 1];

void make_array() {
    for (int i = 1; i <= n; ++i) {
        keys[i] = i;
        vals[i] = -i;
    }
}

void build_tree() {
    tree.build(keys + 1, vals + 1, n);
}

void test3() {
    make_array();
    build_tree();
//    tree.traverse();
    test_find();
    test_erase();

}

int main() {
    tree.clear_file();
    tree.tree_info();

//    test1();

//    test2();

    test3();
    tree.traverse();
}