#ifndef BTREE_HPP
#define BTREE_HPP

#include <iostream>
#include <cstdio>
#include <cstring>
#include "exceptions.hpp"
#include "constant.hpp"
#include "alloc.hpp"
#include "node.hpp"

using std::cout;
using std::cin;
using std::endl;
//#define DEBUG

namespace sjtu{
    template <class key_t, class value_t>
    class bptree {
        friend class iterator;
        typedef node<key_t, value_t> Node;

        char filename[50];

        Rank leaf_max;
        Rank nonleaf_max;

        file_alloc alloc;

        Node pool[50];
        short cnt;

    private:

        void find_leaf(const key_t &Key, Node &ret) {
            alloc.get_root(ret);

            while (!ret.isLeaf) {
                find_child(Key, ret, ret);
            }

        }

        Rank find_child(const key_t &Key, Node &cur, Node &ch) {
            Rank idx = cur.search_upper_inner(Key);

            if (idx == -1) {
                alloc.get_block(cur.child(cur.childSize - 1), ch);
                return cur.childSize - 1;
            }
            else {
                alloc.get_block(cur.child(idx), ch);
                return idx;
            }
        }

        key_t split_nonleaf(Node &n, Node &n2) {
            Rank mid = (nonleaf_max + 1) / 2;

            key_t midKey = *(n.keyInner(mid - 1));

            for (Rank i = mid; i < n.keySize; ++i) {
                *(n2.keyInner(i - mid)) = *(n.keyInner(i));
                *(n2.child(i - mid)) = *(n.child(i));
            }
            *(n2.child(n.keySize - mid)) = *(n.child(n.keySize));

            n2.keySize = n.keySize - mid;
            n.keySize = mid - 1;
            n2.childSize = n2.keySize + 1;
            n.childSize = n.keySize + 1;

            return midKey;
        }

        void split_leaf(Node &lf, Node &lf2) {
            Rank mid = (leaf_max + 1) / 2;

            for (Rank i = mid; i < lf.keySize; ++i) {
                *(lf2.keyLeaf(i - mid)) = *(lf.keyLeaf(i));
                *(lf2.value(i - mid)) = *(lf.value(i));
            }

            lf2.keySize = lf.keySize - mid;
            lf.keySize = mid;
        }

        //pass
        bool insert_in_leaf(Node &cur, const key_t &Key, const value_t &Value) {
            Rank idx = cur.search_upper_leaf(Key);

            if (idx == -1) {
                if (*(cur.keyLeaf(cur.keySize - 1)) == Key)
                    return false;
            }
            else {
                if (idx != 0 && *(cur.keyLeaf(cur.keySize - 1)) == Key)
                    return false;
            }

            if (idx == -1) {
                cur.insert_key_leaf(cur.keySize, Key);
                cur.insert_value(cur.keySize, Value);
                cur.keySize++;

                return true;
            }
            else {
                cur.insert_key_leaf(idx, Key);
                cur.insert_value(idx, Value);
                cur.keySize++;

                return true;
            }
        }

        //我去到底哪里写错了，insert写的非常有道理啊
        bool insert_node(Node &cur, const key_t &Key, const value_t &Value) {
            if (cur.isLeaf) {
                bool flag = insert_in_leaf(cur, Key, Value);
                return flag;
            }
            //curent node is inner node
            else {
                //先找到孩子ch和它的位置
                Node &ch = pool[cnt++];
                ch.init();

                Rank chPos = find_child(Key, cur, ch);

                //往下递归
                bool flag = insert_node(ch, Key, Value);

                if (!flag) {
                    cnt--;

                    return false;
                }
                else {
                    if (ch.isLeaf) {
                        if (ch.keySize <= leaf_max) {
                            alloc.write_block(ch);
                            cnt--;

                            return true;
                        }
                        else {
                            Node &newLeaf = pool[cnt++];
                            newLeaf.init();
                            alloc.append_block(newLeaf, true);

                            newLeaf.next = ch.next;
                            ch.next = newLeaf;

                            split_leaf(ch, newLeaf);

                            cur.insert_key_inner(chPos, *(newLeaf.keyInner(0)));
                            cur.insert_child(chPos + 1, newLeaf.addr);
                            cur.keySize++;
                            cur.childSize++;

                            alloc.write_block(ch);
                            alloc.write_block(newLeaf);
                            cnt -= 2;

                            return true;
                        }
                    }
                    else {
                        if (ch.childSize <= nonleaf_max) {
                            alloc.write_block(ch);
                            cnt--;

                            return true;
                        }
                        else {
                            key_t midKey;
                            Node &newInner = pool[cnt++];
                            newInner.init();
                            alloc.append_block(newInner, false);

                            midKey = split_nonleaf(ch, newInner);

                            cur.insert_key_inner(chPos, midKey);
                            cur.insert_child(chPos + 1, newInner.addr);
                            cur.keySize++;
                            cur.childSize++;

                            alloc.write_block(ch);
                            alloc.write_block(newInner);

                            cnt -= 2;

                            return true;
                        }
                    }
                }
            }
        }

        //
        bool erase_node(Node &cur, const key_t &Key) {
            if (cur.isLeaf) {
                Rank delPos = cur.search_leaf(Key);

                if (delPos == -1)
                    return false;
                else {
                    cur.erase_key_leaf(delPos);
                    cur.erase_value(delPos);
                    cur.keySize--;

                    return true;
                }
            }
            else {
                Node &ch = pool[cnt++];
                ch.init();
                //先找到孩子ch以及它的位置
                Rank chPos = find_child(Key, cur, ch);

                bool flag = erase_node(ch, Key);

                if (!flag) {
                    cnt--;
                    return false;
                }
                else {
                    if (ch.isLeaf) {
                        if (ch.keySize >= (leaf_max + 1) / 2) {
                            alloc.write_block(ch);
                            cnt--;

                            return true;
                        }
                        else {
                            Node &sbl = pool[cnt++];
                            sbl.init();

                            Rank sblPos;
                            Rank keyPos;

                            Node *leftNode, *rightNode;

                            if (chPos == 0) {
                                sblPos = 1;
                                alloc.get_block(*(cur.child(sblPos)), sbl);
                            }
                            else {
                                sblPos = chPos - 1;
                                alloc.get_block(*(cur.child(sblPos)), sbl);
                            }

                            keyPos = std:;min(chPos, sblPos);

                            // 合并叶子节点
                            if (ch.keySize + sbl.keySize <= leaf_max) {
                                if (chPos < sblPos) {
                                    leftNode = &ch;
                                    rightNode = &sbl;
                                }
                                else {
                                    leftNode = &sbl;
                                    rightNode = &ch;
                                }

                                leftNode->next = rightNode->next;

                                for (Rank i = 0; i < rightNode->keySize; ++i) {
                                    leftNode->insert_key_leaf(leftNode->keySize + i, *(rightNode->keyLeaf(i)));
                                    leftNode->insert_value(leftNode->keySize + i, *(rightNode->value(i)));
                                }
                                leftNode->keySize += rightNode->keySize;

                                cur.erase_key_inner(keyPos);
                                cur.erase_child(keyPos + 1);
                                cur.keySize--;
                                cur.childSize--;

                                rightNode->next = alloc.trash_off;
                                alloc.trash_off = rightNode->addr;

                                alloc.write_block(*leftNode);
                                alloc.write_block(*rightNode);
                                cnt -= 2;

                                return true;
                            }
                            else {
                                if (sblPos < chPos) {
                                    key_t borrow_key = sbl.keyLeaf(sbl.keySize - 1);
                                    value_t borrow_value = sbl.value(sbl.keySize - 1);

                                    sbl.erase_key_leaf(sbl.keySize - 1);
                                    sbl.erase_value(sbl.keySize - 1);
                                    sbl.keySize--;

                                    ch.insert_key_leaf(0, borrow_key);
                                    ch.insert_value(0, borrow_value);
                                    ch.keySize--;

                                    *(cur.keyInner(keyPos)) = *(ch.keyLeaf(0));

                                    alloc.write_block(ch);
                                    alloc.write_block(sbl);
                                    cnt -= 2;

                                    return true;
                                }
                                else {
                                    key_t borrow_key = sbl.keyLeaf(0);
                                    value_t borrow_value = sbl.value(0);

                                    sbl.erase_key_leaf(0);
                                    sbl.erase_value(0);

                                    ch.insert_key_leaf(ch.keySize, borrow_key);
                                    ch.insert_value(ch.keySize, borrow_value);
                                    ch.keySize++;

                                    *(cur.keyLeaf(keyPos)) = *(sbl.keyLeaf(0));

                                    alloc.write_block(ch);
                                    alloc.write_block(sbl);
                                    cnt -= 2;

                                    return true;
                                }
                            }
                        }
                    }
                    //child is inner node
                    else {
                        if (ch.childSize >= (nonleaf_max + 1) / 2) {
                            alloc.write_block(ch);
                            cnt--;

                            return true;
                        }
                        else {
                            Node &sbl = pool[cnt++];
                            sbl.init();

                            Rank sblPos;
                            Rank keyPos;

                            Node *leftNode, *rightNode;

                            if (chPos == 0) {
                                sblPos = 1;
                                alloc.get_block(*(cur.child(sblPos)), sbl);
                            }
                            else {
                                sblPos = chPos - 1;
                                alloc.get_block(*(cur.child(sblPos)), sbl);
                            }

                            keyPos = std:;min(chPos, sblPos);

                            //合并内部节点
                            if (ch.childSize + sbl.childSize <= nonleaf_max) {
                                if (chPos < sblPos) {
                                    leftNode = &ch;
                                    rightNode = &sbl;
                                }
                                else {
                                    leftNode = &sbl;
                                    rightNode = &ch;
                                }

                                leftNode->insert_key_inner(leftNode->keySize, *(cur.keyInner(keyPos)));
                                for (Rank i = 0; i < rightNode->keySize; ++i)
                                    leftNode->insert_key_inner(leftNode->keySize + 1 + i, *(rightNode->keyInner(i)));
                                for (Rank i = 0; i < rightNode->keySize; ++i)
                                    leftNode->insert_child(leftNode->keySize + i, *(rightNode->child(i)));
                                leftNode->keySize += 1 + rightNode->keySize;
                                leftNode->childSize = leftNode->keySize + 1;

                                cur.erase_key_inner(keyPos);
                                cur.erase_child(keyPos + 1);
                                cur.keySize--;
                                cur.childSize--;

                                rightNode->next = alloc.trash_off;
                                alloc.trash_off = rightNode->addr;

                                alloc.write_block(*leftNode);
                                alloc.write_block(*rightNode);
                                cnt -= 2;

                                return true;
                            }
                            //borrow from sibling
                            else {
                                if (sblPos < chPos) {
                                    key_t borrow_key = *(sbl.keyInner(sbl.keySize - 1));
                                    offset borrow_child = *(sbl.child(sbl.keySize));

                                    sbl.erase_key_inner(sbl.keySize - 1);
                                    sbl.erase_child(sbl.childSize - 1);
                                    sbl.keySize--;
                                    sbl.childSize--;

                                    ch.insert_key_inner(0, *(cur.keyInner(keyPos)));
                                    ch.insert_child(0, borrow_child);
                                    ch.keySize++;
                                    ch.childSize++;

                                    *(cur.keyInner(keyPos)) = borrow_key;

                                    alloc.write_block(ch);
                                    alloc.write_block(sbl);
                                    cnt -= 2;

                                    return true;
                                }
                                else {
                                    key_t borrow_key = *(sbl.keyInner(0));
                                    offset borrow_child = *(sbl.child(0));

                                    sbl.erase_key_inner(0);
                                    sbl.erase_child(0);
                                    sbl.keySize--;
                                    sbl.childSize--;

                                    ch.insert_key_inner(*(cur.keyInner(keyPos)));
                                    ch.insert_child(borrow_child);
                                    ch.keySize++;
                                    ch.childSize++;

                                    *(cur.keyInner(keyPos)) = borrow_key;

                                    alloc.write_block(ch);
                                    alloc.write_block(sbl);
                                    cnt -= 2;

                                    return true;
                                }
                            }
                        }
                    }
                }
            }
        }


    public:
        bptree() {
            cnt = 0;
            leaf_max = (blockSize -  node_byte) / (sizeof(key_t) + sizeof(value_t));
            nonleaf_max = (blockSize - node_byte + sizeof(key_t)) / (sizeof(key_t) + sizeof(offset));
        }

        ~bptree() {
            alloc.close_file();
        }

        void set_filename(const char *fname) {
            strcpy(filename, fname);
            alloc.set_filename(filename);
        }

        void open_file() {
            alloc.open_file();
        }

        //todo
        void init() {

        }

        value_t find(const key_t &Key) {
            Node &t = pool[cnt++];
            find_leaf(Key, t);

            Rank idx = t.search_leaf(Key);

            cnt--;
            if (idx == -1)
                return value_t();
            else
                return *(t.keyLeaf(idx));
        }

        int count(const key_t &Key) {
            Node &t = pool[cnt++];
            find_leaf(Key, t);

            Rank idx = t.search_leaf(Key);

            cnt--;
            if (idx == -1)
                return 0;
            else
                return 1;
        }

        bool modify(const key_t &Key, const value_t &Value) {
            Node &t = pool[cnt++];
            find_leaf(Key, t);

            Rank idx = t.search_leaf(Key);

            if (idx == -1)
                return false;
            else {
                *(t.value(idx)) = Value;
                return true;
            }
        }

        bool insert(const key_t &Key, const value_t &Value) {
            Node &root = pool[cnt++];

            if (alloc.root_off == invalid_off) {
                alloc.append_block(root, true);

                root.insert_key_leaf(Key);
                root.insert_value(Value);
                root.keySize++;

                alloc.root_off = root.addr;

                alloc.write_block(root);
                cnt--;

                return true;
            }

            alloc.get_root(root);
            bool flag = insert_node(root, Key, Value);

            if (!flag) {
                cnt--;
                return false;
            }
            else {
                if (root.isLeaf) {
                    if (root.keySize > leaf_max) {
                        Node &newLeaf = pool[cnt++];
                        alloc.append_block(newLeaf, true);

                        split_leaf(root, newLeaf);
                        newLeaf.next = root.next;
                        root.next = newLeaf.addr;

                        Node &newRoot = pool[cnt++];
                        alloc.append_block(newRoot, false);

                        newRoot.insert_key_inner(0, *(newLeaf.keyLeaf(0)));
                        newRoot.insert_child(0, root.addr);
                        newRoot.insert_child(1, newLeaf.addr);
                        newRoot.keySize = 1;
                        newRoot.childSize = 2;
                        alloc.root_off = newRoot.addr;

                        alloc.write_block(newRoot);
                        alloc.write_block(root);
                        alloc.write_block(newLeaf);
                        cnt -= 3;

                        return true;
                    }
                    else {
                        alloc.write_block(root);
                        cnt--;

                        return true;
                    }
                }
                else {
                    if (root.childSize > nonleaf_max) {
                        Node &newInner = pool[cnt++];
                        alloc.append_block(newInner, false);

                        split_nonleaf(root, newInner);

                        Node &newRoot = pool[cnt++];
                        alloc.append_block(newRoot, false);

                        key_t midKey = split_nonleaf(root, newInner);

                        newRoot.insert_key_inner(0, midKey);
                        newRoot.insert_child(0, root);
                        newRoot.insert_child(1, newInner);
                        newRoot.keySize = 1;
                        newRoot.childSize = 2;
                        alloc.root_off = newRoot.addr;

                        alloc.write_block(newRoot);
                        alloc.write_block(root);
                        alloc.write_block(newInner);
                        cnt -= 3;

                        return true;
                    }
                    else {
                        alloc.write_block(root);
                        cnt--;

                        return true;
                    }
                }
            }
        }

        bool erase(const key_t &Key) {
            Node &root = pool[cnt++];

            if (alloc.root_off == invalid_off) {
                cnt--;
                return false;
            }

            alloc.get_root(root);
            bool flag = erase_node(root, Key);

            if (!flag) {
                cnt--;
                return false;
            }
            else {
                if (root.isLeaf) {
                    if (root.keySize == 0) {
                        root.next = alloc.trash_off;
                        alloc.trash_off = root.addr;
                        alloc.root_off = invalid_off;

                        alloc.write_block(root);
                        cnt--;

                        return true;
                    }
                    else {
                        alloc.write_block(root);
                        cnt--;
                        return true;
                    }
                }
                else {
                    if (root.childSize == 1) {
                        alloc.root_off = *(root.child(0));

                        root.next = alloc.trash_off;
                        alloc.trash_off = root.addr;

                        alloc.write_block(root);
                        cnt--;

                        return true;
                    }
                    else {
                        alloc.write_block(root);
                        cnt--;
                        return true;
                    }
                }
            }
        }

        class iterator {
            friend class bptree;
        private:
            offset addr;
            Rank idx;

        public:
            iterator(offset addr = invalid_off, Rank idx = 0) : addr(addr), idx(idx) {}

            iterator(const iterator &other) : addr(other.addr), idx(other.idx) {}

            iterator operator++(int) {
                iterator ret = *this;
                ++(*this);

                return ret;
            }

            iterator & operator++() {
                Node &t = pool[cnt++];
                alloc.get_block(addr, t);

                if (idx == t.keySize - 1) {
                    addr = t.next;
                    idx = 0;
                }
                else {
                    idx++;
                }
                cnt--;

                return *this;
            }

            bool check() {
                if (addr == invalid_off)
                    return false;
                else
                    return true;
            }

            key_t retKey() {
                Node &t = pool[cnt++];
                alloc.get_block(addr, t);

                key_t k = *(t.keyLeaf(idx));
                cnt--;
                return k;
            }

            value_t retValue() {
                Node &t = pool[cnt++];
                alloc.get_block(addr, t);

                value_t v = *(t.value(idx));
                cnt--;
                return v;
            }
        };

        iterator lower_bound(const key_t &Key) {
            Node &tmp = pool[cnt++];
            find_leaf(Key, tmp);

            Rank idx;
            for (idx = 0; idx < tmp.keySize; ++idx) {
                if (*(tmp.keyLeaf(idx) >= Key))
                    break;
            }

            offset retaddr;
            if (idx == tmp.keySize) {
                retaddr = tmp.next;
                idx = 0;
            }
            else {
                retaddr = tmp.addr;
            }

            cnt--;

            iterator ret(retaddr, idx);
            return ret;
        }

//use for debug
        void view_root() {
            root->view();
        }
    };
}

#endif