#ifndef BTREE_BPTREE_HPP
#define BTREE_BPTREE_HPP

#include <iostream>
#include <cstdio>
#include <cstring>
#include "vector.hpp"
#include "exception.hpp"

namespace sjtu{
    const int blockSize = 4096;

    template <class key_t, class value_t>
    class bptree{
    private:
        struct Node{
            vector<Node *> child;
            vector<key_t> key;
            vector<value_t> value;

            //不确定是否需要指向父亲的指针
            
            bool isLeaf;
            //如果是叶节点，next将指向下一个叶节点
            Node *next;

            Node(bool is_leaf = true) {
                isLeaf = is_leaf;
                next = NULL;
            }
        };

        Node *root;

        Node find_leaf(const key_t &Key) {
            Node *t = root;
            while (!t->isLeaf) {
                int idx;
                for (idx = 0; idx < t->key.size(); ++idx) {
                    if (Key > t->key[idx])
                        break;
                }
                t = t->child[idx];
            }
            return t;
        }

        void insert_in_leaf(Node *lf, const key_t &Key, const value_t &Value) {
            if (Key < lf->key[0]) {
                lf->key.insert(0, Key);
                lf->value.insert(0, Value);
            }
        }

        //todo and unsure whether it is necessary
        void insert_in_parent(Node *n, const key_t &Key) {
            if (n == root) {

            }
        }

        //todo
        void erase_entry(Node *n, key_t &Key) {
            //从n中删除Key和对应value
            for (int i = 0; i < n->key.size(); ++i) {
                if (n->key[i] == Key) {
                    n->key.erase(i);
                    if (n->isLeaf)
                        n->value.erase(i);

                    break;
                }
            }

            //todo
            if (n == root && n->child.size() == 1) {

            }
        }

    public:
        int count(const key_t &Key) {
            Node *t = root;
            while (!t->isLeaf) {
                int idx;
                for (idx = 0; idx < t->key.size(); ++idx) {
                    if (Key > t->key[idx])
                        break;
                }
                t = t->child[idx];
            }
            //t is leafNode now
            int idx;
            for (idx = 0; idx < t->key.size(); ++idx) {
                if (Key == t->key[idx])
                    break;
            }

            if (idx != t->key.size())
                return 1;
            else
                return 0;
        }

        value_t find(const key_t &Key) {
            Node *t = root;
            while (!t->isLeaf) {
                int idx;
                for (idx = 0; idx < t->key.size(); ++idx) {
                    if (Key > t->key[idx])
                        break;
                }
                t = t->child[idx];
            }
            //t is leafNode now
            int idx;
            for (idx = 0; idx < t->key.size(); ++idx) {
                if (Key == t->key[idx])
                    break;
            }

            if (idx != t->key.size()) {
                return t->value[idx];
            }
            else {
                //return default if not found
                return value_t();
            }
        }

        void insert(const key_t &Key, const value_t &Value) {
            Node *t = root;
            if (root == NULL) {
                root = new Node();
            }
            else {
                //找到应该包含Key的叶节点
                while (!t->isLeaf) {
                    int idx;
                    for (idx = 0; idx < t->key.size(); ++idx) {
                        if (Key > t->key[idx])
                            break;
                    }
                    t = t->child[idx];
                }
            }

            if (t->key.size() < blockSize - 1) {
                insert_in_leaf(t, Key, Value);
            }
            else {
                Node *t2 = new Node();
                for (int i = (blockSize + 1) / 2 - 1; i < blockSize; ++i) {
                    t2->key.push_back(t->key[i]);
                    t2->value.push_back(t->value[i]);
                }
                for (int i = (blockSize + 1) / 2 - 1; i < blockSize; ++i) {
                    t->key.pop_back();
                    t->value.pop_back();
                }
                t2->next = t->next;
                t->next = t2;

                //todo
                insert_in_parent();
            }
        }

        void erase(const key_t &Key) {
            Node *lf = find_leaf(Key);
            erase_entry(lf, Key);
        }


    };
}


#endif //BTREE_BPTREE_HPP
