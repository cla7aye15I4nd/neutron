#ifndef BTREE_HPP
#define BTREE_HPP

#include <iostream>
#include <cstdio>
#include <cstring>
#include "myVector.hpp"
#include "exceptions.hpp"
#include "constant.hpp"
using std::cout;
using std::cin;
using std::endl;
//#define DEBUG

namespace sjtu{
    const Rank leaf_max = blockSize - 1;
    const Rank nonleaf_max = blockSize;
    template <class key_t, class value_t>
    class bptree {
        friend class iterator;
    private:
        struct Node {
            vector <Node *> child;
            vector <key_t> key;
            vector <value_t> value;

            bool isLeaf;

            Node *next;

            Node(bool is_leaf = true) {
                isLeaf = is_leaf;
                next = NULL;
            }

            ~Node() {

            }

            void view() {
                cout << "Nodesize = " << key.size() << endl;
                if (isLeaf) {
                    for (Rank i = 0; i < key.size(); ++i) {
                        cout << key[i] << ":" << value[i] << ' ';
                    }
                    puts("");
                }
                else {
                    for (Rank i = 0; i < key.size(); ++i) {
                        cout << key[i] << ' ';
                    }
                    puts("");
                }
            }

            Rank search(const key_t &Key) {
                Rank idx;
                for (idx = 0; idx < key.size(); ++idx) {
                    if (key[idx] == Key)
                        break;
                }

                if (idx == key.size()) {
                    return -1;
                }
                else {
                    return idx;
                }
            }

            Rank search_upper(const key_t &Key) {
                Rank idx;
                for (idx = 0; idx < key.size(); ++idx) {
                    if (key[idx] > Key)
                        break;
                }

                if (idx == key.size()) {
                    return -1;
                }
                else {
                    return idx;
                }
            }
        };

    public:
        Node *root;

    private:

        Node *find_leaf(const key_t &Key) {
            Node *t = root;
            while (!t->isLeaf) {
                Rank idx;
                for (idx = 0; idx < t->key.size(); ++idx) {
                    if (Key < t->key[idx])
                        break;
                }
                t = t->child[idx];
            }

            return t;
        }

        key_t split_nonleaf(Node *n, Node *n2) {
            Rank mid = (nonleaf_max + 1) / 2;

            key_t midKey = n->key[mid - 1];

            n2->key.copy(n->key, mid, n->key.size());
            n2->child.copy(n->child, mid, n->child.size());

            n->key.update_size(mid - 1);
            n->child.update_size(mid);

            return midKey;
        }

        void split_leaf(Node *lf, Node *lf2) {
            Rank mid = (leaf_max + 1) / 2;

            lf2->key.copy(lf->key, mid, lf->key.size());
            lf2->value.copy(lf->value, mid, lf->value.size());

            lf->key.update_size(mid);
            lf->value.update_size(mid);
        }

        bool insert_in_leaf(Node *cur, const key_t &Key, const value_t &Value) {
            Rank idx = cur->search_upper(Key);

            //has existed
            if (idx == -1) {
                if (cur->key[cur->key.size() - 1] == Key)
                    return false;
            }
            else {
                if (idx != 0 && cur->key[idx - 1] == Key)
                    return false;
            }

            if (idx == -1) {
                cur->key.push_back(Key);
                cur->value.push_back(Value);
                return true;
            }
            else {
                cur->key.insert(idx, Key);
                cur->value.insert(idx, Value);
                return true;
            }

        }

        bool insert_node(Node *cur, const key_t &Key, const value_t &Value) {
            if (cur->isLeaf) {
                bool flag = insert_in_leaf(cur, Key, Value);
                return flag;
            }
            else {
                Node *ch;
                Rank chPos = cur->search_upper(Key);
                if (chPos == -1) {
                    ch = cur->child.back();
                    chPos = cur->child.size() - 1;
                }
                else {
                    ch = cur->child[chPos];
                }

                bool flag = insert_node(ch, Key, Value);

                if (!flag) {
                    return false;
                }
                else {
                    if (ch->isLeaf) {
                        if (ch->key.size() <= leaf_max) {
                            return true;
                        }
                        else {
                            Node *newLeaf = new Node();
                            newLeaf->next = ch->next;
                            ch->next = newLeaf;

                            split_leaf(ch, newLeaf);

                            cur->key.insert(chPos, newLeaf->key[0]);
                            cur->child.insert(chPos + 1, newLeaf);

                            return true;
                        }
                    }
                    else {
                        if (ch->child.size() <= nonleaf_max) {
                            return true;
                        }
                        else {
                            key_t midKey;
                            Node *newInner = new Node(false);

                            midKey = split_nonleaf(ch, newInner);

                            cur->key.insert(chPos, midKey);
                            cur->child.insert(chPos + 1, newInner);

                            return true;
                        }
                    }
                }
            }
        }

        bool erase_node(Node *cur, const key_t &Key) {
            if (cur->isLeaf) {
                Rank delPos = cur->search(Key);

                if (delPos == -1) {
                    return false;
                }
                else {
                    cur->key.erase(delPos);
                    cur->value.erase(delPos);
                    return true;
                }
            }
            else {
                Node *ch;
                Rank chPos = cur->search_upper(Key);
                if (chPos == -1) {
                    ch = cur->child.back();
                    chPos = cur->child.size() - 1;
                }
                else {
                    ch = cur->child[chPos];
                }

                bool flag = erase_node(ch, Key);

                if (!flag) {
                    return false;
                }
                else {
                    if (ch->isLeaf) {
                        if (ch->key.size() >= (leaf_max + 1) / 2) {
                            return true;
                        }
                        else {
                            Node *sbl;
                            Rank sblPos;
                            Rank keyPos;
                            Node *leftNode, *rightNode;

                            if (chPos == 0) {
                                sbl = cur->child[1];
                                sblPos = 1;
                            }
                            else {
                                sbl = cur->child[chPos - 1];
                                sblPos = chPos - 1;
                            }

                            keyPos = std::max(chPos, sblPos) - 1;

                            if (ch->key.size() + sbl->key.size() <= leaf_max) {
                                if (chPos < sblPos) {
                                    leftNode = ch;
                                    rightNode = sbl;
                                }
                                else {
                                    leftNode = sbl;
                                    rightNode = ch;
                                }

                                leftNode->next = rightNode->next;

                                for (Rank i = 0; i < rightNode->key.size(); ++i) {
                                    leftNode->key.push_back(rightNode->key[i]);
                                    leftNode->value.push_back(rightNode->value[i]);
                                }

                                cur->key.erase(keyPos);
                                cur->child.erase(keyPos + 1);

                                delete rightNode;

                                return true;
                            }
                            else {
                                if (sblPos < chPos) {
                                    key_t borrow_key = sbl->key.back();
                                    value_t borrow_value = sbl->value.back();

                                    sbl->key.pop_back();
                                    sbl->value.pop_back();

                                    ch->key.insert(0, borrow_key);
                                    ch->value.insert(0, borrow_value);

                                    cur->key[keyPos] = ch->key[0];

                                    return true;
                                }
                                else {
                                    key_t borrow_key = sbl->key.front();
                                    value_t borrow_value = sbl->value.front();

                                    sbl->key.erase(0);
                                    sbl->value.erase(0);

                                    ch->key.push_back(borrow_key);
                                    ch->value.push_back(borrow_value);

                                    cur->key[keyPos] = sbl->key[0];

                                    return true;
                                }
                            }
                        }
                    }
                        //ch is nonleaf
                    else {
                        if (ch->child.size() >= (nonleaf_max + 1) / 2) {
                            return true;
                        }
                        else {
                            Node *sbl;
                            Rank sblPos;
                            Rank keyPos;
                            Node *leftNode, *rightNode;

                            if (chPos == 0) {
                                sbl = cur->child[1];
                                sblPos = 1;
                            } else {
                                sbl = cur->child[chPos - 1];
                                sblPos = chPos - 1;
                            }

                            keyPos = std::max(chPos, sblPos) - 1;

                            if (ch->child.size() + sbl->key.size() <= nonleaf_max) {
                                if (chPos < sblPos) {
                                    leftNode = ch;
                                    rightNode = sbl;
                                }
                                else {
                                    leftNode = sbl;
                                    rightNode = ch;
                                }

                                leftNode->key.push_back(cur->key[keyPos]);
                                for (Rank i = 0; i < rightNode->key.size(); ++i)
                                    leftNode->key.push_back(rightNode->key[i]);
                                for (Rank i = 0; i < rightNode->child.size(); ++i)
                                    leftNode->child.push_back(rightNode->child[i]);


                                cur->key.erase(keyPos);
                                cur->child.erase(keyPos + 1);

                                delete rightNode;

                                return true;
                            }
                                //borrow from sibling
                            else {
                                if (sblPos < chPos) {
                                    key_t borrow_key = sbl->key.back();
                                    Node *borrow_child = sbl->child.back();

                                    sbl->key.pop_back();
                                    sbl->child.pop_back();

                                    ch->key.insert(0, cur->key[keyPos]);
                                    ch->child.insert(0, borrow_child);

                                    cur->key[keyPos] = borrow_key;

                                    return true;
                                }
                                else {
                                    key_t borrow_key = sbl->key.front();
                                    Node *borrow_child = sbl->child.front();

                                    sbl->key.erase(0);
                                    sbl->child.erase(0);

                                    ch->key.push_back(borrow_key);
                                    ch->child.push_back(borrow_child);

                                    cur->key[keyPos] = borrow_key;

                                    return true;
                                }
                            }
                        }
                    }
                }
            }
        }

        void clear(Node *t) {
            if (t == NULL)
                return;

            for (Rank i = 0; i < t->child.size(); ++i)
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

        value_t find(const key_t &Key) {
            Node *t = find_leaf(Key);

            Rank idx = t->search(Key);

            if (idx == -1)
                return value_t();
            else
                return t->value[idx];
        }

        bool insert(const key_t &Key, const value_t &Value) {
            if (root == NULL) {
                root = new Node();

                root->key.push_back(Key);
                root->value.push_back(Value);

                return true;
            }

            bool flag = insert_node(root, Key, Value);

            if (!flag) {
                return false;
            }
            else {
                if (root->isLeaf) {
                    if (root->key.size() > leaf_max) {
                        Node *newLeaf = new Node();
                        Node *newRoot = new Node(false);

                        split_leaf(root, newLeaf);
                        newLeaf->next = root->next;
                        root->next = newLeaf;

                        newRoot->key.push_back(newLeaf->key[0]);
                        newRoot->child.push_back(root);
                        newRoot->child.push_back(newLeaf);
                        root = newRoot;

                        return true;
                    }
                    else {
                        return true;
                    }
                }
                else {
                    if (root->child.size() > nonleaf_max) {
                        Node *newInner = new Node(false);
                        Node *newRoot = new Node(false);

                        key_t midKey;

                        midKey = split_nonleaf(root, newInner);

                        newRoot->key.push_back(midKey);
                        newRoot->child.push_back(root);
                        newRoot->child.push_back(newInner);
                        root = newRoot;

                        return true;
                    }
                    else {
                        return true;
                    }
                }
            }
        }

        bool erase(const key_t &Key) {
            if (root == NULL)
                return false;

            bool flag = erase_node(root, Key);

            if (!flag) {
                return false;
            }
            else {
                if (root->isLeaf) {
                    if (root->key.size() == 0) {
                        ;
                    }
                }
                else {
                    if (root->child.size() == 1) {
                        Node *tmp = root;
                        root = root->child[0];
                        delete tmp;
                        root->next = NULL;
                    }
                }
                return true;
            }
        }

        class iterator {
            friend class bptree;
        private:
            Node *ptr;
            Rank idx;

        public:
            iterator(Node *p = NULL, Rank i = 0) : ptr(p), idx(i) {}

            iterator(const iterator &other) : ptr(other.ptr), idx(other.idx) {}

            iterator operator++(int) {
                iterator ret = *this;
                ++(*this);

                return ret;
            }

            iterator & operator++() {
                if (idx == ptr->key.size() - 1) {
                    ptr = ptr->next;
                    idx = 0;
                }
                else {
                    ++idx;
                }

                return *this;
            }

            bool check() {
                if (ptr == NULL)
                    return false;
                else
                    return true;
            }

            key_t retKey() {
                return ptr->key[idx];
            }

            value_t retValue() {
                return ptr->value[idx];
            }
        };

        iterator lower_bound(const key_t &Key) {
            Node *tmp = find_leaf(Key);
            Rank idx;
            for (idx = 0; idx < tmp->key.size(); ++idx) {
                if (tmp->key[idx] >= Key)
                    break;
            }
            if (idx == tmp->key.size()) {
                tmp = tmp->next;
                idx = 0;
            }

            iterator ret(tmp, idx);
            return ret;
        }

//use for debug
        void view_root() {
            root->view();
        }
    };
}

#endif