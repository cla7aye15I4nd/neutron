#ifndef BTREE_BPTREE_HPP
#define BTREE_BPTREE_HPP

#include <iostream>
#include <cstdio>
#include <cstring>
#include "myVector.hpp"
#include "exceptions.hpp"
using std::cout;

namespace sjtu {
    template<class key_t, class value_t>
    class bptree {
    private:
        struct Node {
            //"vector.hpp"是采用动态数组
            //"myVector.hpp"是采用静态数组
            vector<Node *> child;
            vector<key_t> key;
            vector<value_t> value;

            //不确定是否需要指向父亲的指针

            bool isLeaf;
            //如果是叶节点，next将指向下一个叶节点
            Node *next;
            Node *parent;

            Node(bool is_leaf = true) {
                isLeaf = is_leaf;
                next = NULL;
                parent = NULL;
            }

            ~Node() {

            }

            void view() {
                if (isLeaf) {
                    for (int i = 0; i < key.size(); ++i) {
                        cout << key[i] << "->" << value[i] << ' ';
                    }
                    puts("");
                }
                else {
                    for (int i = 0; i < key.size(); ++i) {
                        cout << key[i];
                    }
                    puts("");
                }
            }
        };

        Node *root;

        Node *find_leaf(const key_t &Key) {
            Node *t = root;
            Node *tmp;
            while (!t->isLeaf) {
                int idx;
                for (idx = 0; idx < t->key.size(); ++idx) {
                    if (Key < t->key[idx])
                        break;
                }
                //在寻找的过程中记录parent
                tmp = t;
                t = t->child[idx];
                t->parent = tmp;
            }
            return t;
        }

        void insert_in_leaf(Node *lf, const key_t &Key, const value_t &Value) {
            int idx;
            for (idx = 0; idx < lf->key.size(); ++idx) {
                if (Key < lf->key[idx]) {
                    break;
                }
            }
            lf->key.insert(idx, Key);
            lf->value.insert(idx, Value);

            return;
        }

        //unsure whether it is necessary
        void insert_in_parent(Node *n, const key_t &Key, Node *n2) {
            if (n == root) {
                // Create a new node R containing N, K′, N′
                // Make R the root of the tree
                Node *newRoot = new Node();
                newRoot->key.insert(0, Key);
                newRoot->child.push_back(n);
                newRoot->child.push_back(n2);
                root = newRoot;
                return;
            }
            Node *p = n->parent;
            if (p->child.size() < blockSize) {
                // insert (K ′, N′) in P just after N
                int i;
                for (i = 0; i < p->child.size(); ++i) {
                    if (p->child[i] == n)
                        break;
                }
                p->key.insert(i, Key);
                p->child.insert(i + 1, n2);
            } else {
                /* Split P */
                //p2是分裂产生的节点
                // todo wrong!
                Node *p2 = new Node();
                for (int i = (blockSize + 1) / 2; i < blockSize; ++i) {
                    p2->child.push_back(p->child[i]);
                }
                for (int i = (blockSize + 1) / 2; i < blockSize; ++i) {
                    p->child.pop_back();
                }
                for (int i = (blockSize + 1) / 2 - 1; i < blockSize - 1; ++i) {
                    p2->key.push_back(p->key[i]);
                }
                for (int i = (blockSize + 1) / 2 - 1; i < blockSize - 1; ++i) {
                    p2->key.push_back(p->key[i]);
                }

                p2->child.push_back(n2);
                p2->key.push_back(Key);

                insert_in_parent(p, p2->key[0], p2);
            }
        }


        void erase_entry(Node *n, const key_t &Key, Node *delNode) {
            //从n中删除Key和对应value或child指针
            for (int i = 0; i < n->key.size(); ++i) {
                if (n->key[i] == Key) {
                    n->key.erase(i);
                    if (n->isLeaf)
                        n->value.erase(i);
                    break;
                }
            }
            if (!n->isLeaf) {
                for (int i = 0; i < n->child.size(); ++i) {
                    if (n->child[i] == delNode) {
                        n->child.erase(i);
                        break;
                    }
                }
            }

            //n is the root and n has only one remaining child
            if (n == root && n->child.size() == 1) {
                root = n->child[0];
                delete n;
            }
                // if n has too few values/pointers
            else if ((n->isLeaf && n->key.size() < blockSize / 2)
                     ||
                     (!n->isLeaf && n->child.size() < (blockSize + 1) / 2)) {
                Node *p = n->parent;
                Node *n2;
                key_t k2;
                //prev为true时表示n2是n的前一个节点
                bool prev = false;
                int idx;
                for (idx = 0; idx < p->child.size(); ++idx) {
                    if (p->child[idx] == n)
                        break;
                }
                if (p->child[idx - 1]) {
                    n2 = p->child[idx - 1];
                    k2 = p->key[idx - 1];
                    prev = true;
                } else {
                    n2 = p->child[idx + 1];
                    k2 = p->key[idx];
                }

                if (n->key.size() + n2->key.size() <= blockSize - 1) {
                    /* Coalesce nodes */
                    if (prev == false) {
                        std::swap(n, n2);
                    }
                    if (!n->isLeaf) {
                        //append k2 and all pointers and values in n to n2
                        n2->key.push_back(k2);
                        for (int i = 0; i < n->key.size(); ++i) {
                            n2->key.push_back(n->key[i]);
                        }

                        for (int i = 0; i < n->child.size(); ++i) {
                            n2->child.push_back(n->child[i]);
                        }
                    }
                    else {
                        for (int i = 0; i < n->key.size(); ++i) {
                            n2->key.push_back(n->key[i]);
                            n2->value.push_back(n->value[i]);
                        }
                    }

                    erase_entry(n->parent, k2, n);
                    delete n;
                }
                else {
                    /* Redistribution: borrow an entry from n2 */
                    if (prev) {
                        // n2是n的前一个节点
                        if (!n->isLeaf) {
                            n->key.insert(0, k2);
                            n->child.insert(0, n2->child.back());

                            for (int i = 0; i < p->key.size(); ++i) {
                                if (p->key[i] == k2) {
                                    p->key[i] = n2->key.back();
                                    break;
                                }
                            }

                            n2->key.pop_back();
                            n2->child.pop_back();
                        }
                        else {
                            n->key.insert(0, n2->key.back());
                            n->value.insert(0, n2->value.back());

                            for (int i = 0; i < p->key.size(); ++i) {
                                if (p->key[i] == k2) {
                                    p->key[i] = n2->key.back();
                                    break;
                                }
                            }

                            n2->key.pop_back();
                            n2->value.pop_back();
                        }
                    }
                    // symmetric to the prev case
                    else {
                        //n2是n的后一个节点
                        if (!n->isLeaf) {
                            n->key.push_back(k2);
                            n->child.push_back(n2->child.front());

                            for (int i = 0; i < p->key.size(); ++i) {
                                if (p->key[i] == k2) {
                                    p->key[i] = n2->key.front();
                                    break;
                                }
                            }

                            n2->key.erase(0);
                            n2->child.erase(0);
                        }
                        else {
                            n->key.push_back(n2->key.front());
                            n->value.push_back(n2->value.front());

                            for (int i = 0; i < p->key.size(); ++i) {
                                if (p->key[i] == k2) {
                                    p->key[i] = n2->key.back();
                                    break;
                                }
                            }

                            n2->key.pop_back();
                            n2->value.pop_back();
                        }
                    }
                }
            }
        }

        void clear(Node *t) {
            if (t == NULL)
                return;

            for (int i = 0; i < t->child.size(); ++i)
                clear(t->child[i]);

            delete t;
        }

    public:
        bptree() {
            root = NULL;
        }

        ~bptree() {
            clear(root);
        }

        int count(const key_t &Key) {
            Node *t = find_leaf(Key);
            int idx;
            for (idx = 0; idx < t->key.size(); ++idx) {
                if (Key == t->key[idx])
                    break;
            }
            if (idx != t->key.size()) {
                return 1;
            }
            else {
                return 0;
            }
        }

        value_t find(const key_t &Key) {
            Node *t = find_leaf(Key);
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
                //todo
                //return default if not found
                return value_t();
            }
        }

        void insert(const key_t &Key, const value_t &Value) {
            Node *t;
            if (root == NULL) {
                root = new Node();
                t = root;
            } else {
                //找到应该包含Key的叶节点
                t = find_leaf(Key);
            }

            if (t->key.size() < blockSize - 1) {
                insert_in_leaf(t, Key, Value);
            } 
            else {
                //t2是分裂出的节点
                Node *t2 = new Node();
                for (int i = (blockSize + 1) / 2; i < blockSize; ++i) {
                    t2->key.push_back(t->key[i]);
                    t2->value.push_back(t->value[i]);
                }
                for (int i = (blockSize + 1) / 2; i < blockSize; ++i) {
                    t->key.pop_back();
                    t->value.pop_back();
                }
                t2->next = t->next;
                t->next = t2;


                insert_in_parent(t, t2->key[0], t2);
            }
        }

        void erase(const key_t &Key) {
            Node *lf = find_leaf(Key);
            erase_entry(lf, Key, NULL);
        }
    };
}


#endif //BTREE_BPTREE_HPP
