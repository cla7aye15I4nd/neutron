#ifndef BTREE_HPP
#define BTREE_HPP

#include <iostream>
#include <cstdio>
#include <cstring>
#include "myVector.hpp"
#include "constant.hpp"
#include "bpNode.hpp"
#include "bufferpool.hpp"
using std::cout;
using std::cin;
using std::endl;
//#define DEBUG

namespace sjtu{
    //B+-tree
    template <class key_t, class value_t>
    class bptree {
        typedef bpNode<key_t, value_t> Node;
    private:
        char filename[50];

    public:
        BufferManager<key_t, value_t> bm;

    private:
        size_t leaf_max, nonleaf_max;

        Node pool[50];

        size_t cnt;

        int find_child(Node &cur, const key_t &Key, Node &ch) {
            int idx = cur.search_upper(Key);

            if (idx == -1) {
                bm.get_node_by_offset(cur.child.back(), ch);
                return cur.child.size() - 1;
            }
            else {
                bm.get_node_by_offset(cur.child[idx], ch);
                return idx;
            }
        }

        void find_leaf(const key_t &Key, Node &ret) {
            bm.get_root(ret);

            while (!ret.isLeaf) {
                find_child(ret, Key, ret);
            }
        }

        //midKey上升一层
        key_t split_nonleaf(Node &t, Node &tnext) {
            int mid = (nonleaf_max + 1) / 2;
            key_t midKey = t.key[mid - 1];

            tnext.key.copy(t.key, mid, t.key.size());
            tnext.child.copy(t.child, mid, t.child.size());

            t.key.update_size(mid - 1);
            t.child.update_size(mid);

            return midKey;
        }

        void split_leaf(Node &lf, Node &lfnext) {
            int mid = (leaf_max + 1) / 2;
            key_t midKey = lf.key[mid - 1];

            lfnext.key.copy(lf.key, mid, lf.key.size());
            lfnext.value.copy(lf.value, mid, lf.value.size());

            lf.key.update_size(mid);
            lf.value.update_size(mid);
        }


        bool insert_in_leaf(Node &cur, const key_t &Key, const value_t &Value) {
            int idx = cur.search_upper(Key);

            if (idx == -1) {
                if (cur.key[cur.key.size() - 1] == Key)
                    return false;
            }
            else {
                if (idx > 0 && cur.key[idx - 1] == Key)
                    return false;
            }

            if (idx == -1) {
                cur.key.push_back(Key);
                cur.value.push_back(Key);
                return true;
            }
            else {
                cur.key.insert(idx, Key);
                cur.value.insert(idx, Value);
                return true;
            }

        }

        bool insert_node(Node &cur, const key_t &Key, const value_t &Value) {
            if (cur.isLeaf) {
                bool flag = insert_in_leaf(cur, Key, Value);
                return flag;
            }
            else {
                Node &ch = pool[cnt++];

                int chPos = find_child(cur, Key, ch);
                insert_node(ch, Key, Value);

                if (ch.isLeaf) {
                    if (ch.key.size() <= leaf_max) {
                        bm.write_node(ch);
                        cnt--;
                        return false;
                    }
                    else {
                        Node &newLeaf = pool[cnt++];
                        bm.append_node(newLeaf, true);

                        newLeaf.next = ch.next;
                        ch.next = newLeaf.addr;

                        if (bm.tail_off == ch.addr) {
                            bm.tail_off = newLeaf.addr;
                        }

                        split_leaf(ch, newLeaf);

                        cur.key.insert(chPos, newLeaf.key[0]);
                        cur.child.insert(chPos + 1, newLeaf.addr);

                        bm.write_node(ch);
                        bm.write_node(newLeaf);
                        cnt -= 2;

                        return true;
                    }
                }
                else {
                    if (ch.child.size() <= nonleaf_max) {
                        bm.write_node(ch);
                        cnt--;
                        return false;
                    }
                    else {
                        key_t midKey;
                        Node &newInner = pool[cnt++];
                        bm.append_node(newInner, false);

                        midKey = split_nonleaf(ch, newInner);
                        cur.key.insert(chPos, midKey);
                        cur.child.insert(chPos + 1, newInner.addr);

                        bm.write_node(ch);
                        bm.write_node(newInner);
                        cnt -= 2;

                        return true;
                    }
                }
            }
        }

        bool erase_node(Node &cur, const key_t &Key) {
            offset sbl_off = 1;

            if (cur.isLeaf) {
                int delPos = cur.search(Key);

                if (delPos == -1) {
                    return false;
                }
                else {
                    cur.key.erase(delPos);
                    cur.value.erase(delPos);
                    return true;
                }
            }
            else {
                Node &ch = pool[cnt++];
                ch.clear();

                int chPos = find_child(cur, Key, ch);

                bool modified = erase_node(ch, Key);

                if (ch.isLeaf) {
                    if (ch.key.size() >= (leaf_max + 1) / 2) {
                        bm.write_node(ch);
                        cnt--;
                        return false;
                    }
                    else {
                        Node &sbl = pool[cnt++];
                        int sblPos, keyPos;
                        key_t key_bt;

                        Node *leftNode, *rightNode;

                        if (chPos == 0) {
                            bm.get_node_by_offset(cur.child[1], sbl);
                            sblPos = 1;
                        }
                        else if (chPos == cur.child.size() - 1){
                            bm.get_node_by_offset(cur.child[cur.child.size() - 2], sbl);
                            sblPos = cur.child.size() - 2;
                        }
                        else {
                            bm.get_node_by_offset(cur.child[chPos + sbl_off], sbl);
                            sblPos = chPos + sbl_off;
                            sbl_off *= -1;
                        }

                        //todo unsure
                        keyPos = std::max(chPos, sblPos) - 1;

                        if (ch.key.size() + sbl.key.size() <= leaf_max) {
                            if (chPos < sblPos) {
                                leftNode = &ch;
                                rightNode = &sbl;
                            }
                            else {
                                leftNode = &sbl;
                                rightNode = &ch;
                            }

                            leftNode->next = rightNode->next;
                            if (bm.tail_off == rightNode->addr) {
                                bm.tail_off = leftNode->addr;
                            }

                            for (int i = 0; i < rightNode->key.size(); ++i) {
                                leftNode->key.push_back(rightNode->key[i]);
                                leftNode->value.push_back(rightNode->value[i]);
                            }

                            //todo unsure
                            cur.key.erase(keyPos);
                            cur.child.erase(keyPos + 1);

                            bm.write_node(*leftNode);

                            //todo unsure
                            rightNode->next = bm.trash_off;
                            bm.trash_off = rightNode->addr;
                            bm.write_node(*rightNode);

                            cnt -= 2;

                            return true;
                        }
                        else {
                            if (sblPos < chPos) {
                                key_t borrow_key = sbl.key.back();
                                value_t borrow_value = sbl.value.back();

                                sbl.key.pop_back();
                                sbl.value.pop_back();

                                ch.key.insert(0, borrow_key);
                                ch.value.insert(0, borrow_value);

                                cur.key[keyPos] = ch.key[0];

                                bm.write_node(ch);
                                bm.write_node(sbl);
                                cnt -= 2;

                                return true;
                            }
                            else {
                                key_t borrow_key = sbl.key[0];
                                value_t borrow_value = sbl.value[0];

                                sbl.key.erase(0);
                                sbl.value.erase(0);

                                ch.key.push_back(borrow_key);
                                ch.value.push_back(borrow_value);

                                cur.key[keyPos] = sbl.key[0];

                                bm.write_node(ch);
                                bm.write_node(sbl);
                                cnt -= 2;

                                return true;
                            }
                        }
                    }
                }
                //child is nonleaf
                else {
                    if (ch.child.size() >= (nonleaf_max + 1) / 2) {
                        if (modified) {
                            bm.write_node(ch);
                        }
                        cnt--;
                        return true;
                    }
                    else {
                        Node &sbl = pool[cnt++];
                        int sblPos, keyPos;
                        Node *leftNode, *rightNode;

                        if (chPos == 0) {
                            bm.get_node_by_offset(cur.child[1], sbl);
                            sblPos = 1;
                        }
                        else if (chPos == cur.child.size() - 1) {
                            bm.get_node_by_offset(cur.child[cur.child.size() - 2], sbl);
                            sblPos = cur.child.size() - 2;
                        }
                        else {
                            bm.get_node_by_offset(cur.child[chPos + sbl_off], sbl);
                            sblPos = chPos + sbl_off;
                            sbl_off *= -1;
                        }

                        keyPos = std::max(chPos, sblPos) - 1;

                        //todo unsure
                        if (ch.child.size() + sbl.child.size() <= nonleaf_max) {
                            if (chPos < sblPos) {
                                leftNode = &ch;
                                rightNode = &sbl;
                            }
                            else {
                                leftNode = &sbl;
                                rightNode = &ch;
                            }

                            leftNode->key.push_back(cur.key[keyPos]);
                            for (int i = 0; i < rightNode->key.size(); ++i)
                                leftNode->key.push_back(rightNode->key[i]);
                            for (int i = 0; i < rightNode->child.size(); ++i)
                                leftNode->child.push_back(rightNode->child[i]);

                            cur.key.erase(keyPos);
                            cur.child.erase(keyPos + 1);

                            bm.write_node(*leftNode);


                            //todo unsure
                            rightNode->next = bm.trash_off;
                            bm.trash_off = rightNode->addr;
                            bm.write_node(*rightNode);

                            cnt -= 2;

                            return true;
                        }
                        else {
                            if (sblPos < chPos) {
                                key_t borrow_key = sbl.key.back();
                                offset borrow_child = sbl.child.back();

                                sbl.key.pop_back();
                                sbl.child.pop_back();

                                ch.key.insert(0, cur.key[keyPos]);
                                ch.child.insert(0, borrow_child);

                                cur.key[keyPos] = borrow_key;

                                bm.write_node(ch);
                                bm.write_node(sbl);
                                cnt -= 2;

                                return true;
                            }
                            else {
                                key_t borrow_key = sbl.key[0];
                                offset borrow_child = sbl.child[0];

                                sbl.key.erase(0);
                                sbl.child.erase(0);

                                //todo unsure
                                ch.key.push_back(cur.key[keyPos]);
                                ch.child.push_back(borrow_child);

                                cur.key[keyPos] = borrow_key;

                                bm.write_node(ch);
                                bm.write_node(sbl);
                                cnt -= 2;

                                return true;
                            }
                        }
                    }
                }
            }
        }


    public:
        bptree() {
            cnt = 0;

            nonleaf_max = (blockSize - node_byte + sizeof(key_t)) / (sizeof(offset) + sizeof(key_t));
            leaf_max = (blockSize - tree_byte - node_byte) / (sizeof(key_t) + sizeof(value_t));
        }

        bptree(const char *fname) {
            strcpy(filename, fname);
            bm.setFilename(fname);

            cnt = 0;

            nonleaf_max = (blockSize - node_byte + sizeof(key_t)) / (sizeof(offset) + sizeof(key_t));
            leaf_max = (blockSize - tree_byte - node_byte) / (sizeof(key_t) + sizeof(value_t));
        }

        ~bptree() {
            if (bm.isOpened) {
                bm.closeFile();
            }
        }

        void set_filename(const char * fname) {
            strcpy(filename, fname);
            bm.setFilename(fname);
        }

        void open_file() {
            bm.openFile();
        }

        void close_file() {
            bm.closeFile();
        }

        value_t find(const key_t &Key) {
            Node &ret = pool[cnt++];

            find_leaf(Key, ret);

            int idx = ret.search(Key);

            --cnt;

            if (idx == -1) {
                return value_t();
            }
            else {
                return ret.value[idx];
            }
        }

        bool insert(const key_t &Key, const value_t &Value) {
            Node &root = pool[cnt++];

            //if bptree is empty
            if (bm.root_off == invalid_off) {
                bm.append_node(root, true);

                root.key.push_back(Key);
                root.value.push_back(Value);

                bm.root_off = root.addr;

                bm.write_node(root);
                cnt--;

                bm.head_off = bm.tail_off = root.addr;

                bm.theSize++;

                return true;
            }

            bm.get_root(root);

            bool modified = insert_node(root, Key, Value);

            if (root.isLeaf) {
                if (root.key.size() > leaf_max) {
                    Node &newRoot = pool[cnt++];
                    Node &newLeaf = pool[cnt++];

                    bm.append_node(newLeaf, true);
                    split_leaf(root, newLeaf);

                    newLeaf.next = root.next;
                    root.next = newLeaf.addr;

                    if (bm.tail_off == root.addr) {
                        bm.tail_off = newLeaf.addr;
                    }

                    bm.append_node(newRoot, false);
                    newRoot.key.push_back(newLeaf.key[0]);
                    newRoot.child.push_back(root.addr);
                    newRoot.child.push_back(newLeaf.addr);

                    bm.write_node(root);
                    bm.write_node(newLeaf);
                    bm.write_node(newRoot);
                    cnt -= 3;

                    bm.theSize++;

                    return true;
                }
                else {
                    if (modified) {
                        bm.write_node(root);
                        cnt--;

                        bm.theSize++;
                        return true;
                    }
                }
            }
            else {
                if (root.child.size() > nonleaf_max) {
                    Node &newInner = pool[cnt++];
                    Node &newRoot = pool[cnt++];
                    key_t midKey;

                    bm.append_node(newInner, false);
                    midKey = split_nonleaf(root, newInner);

                    bm.append_node(newRoot, false);
                    newRoot.key.push_back(midKey);
                    newRoot.child.push_back(root.addr);
                    newRoot.child.push_back(newInner.addr);

                    bm.root_off = newRoot.addr;

                    bm.write_node(root);
                    bm.write_node(newInner);
                    bm.write_node(newRoot);
                    cnt -= 3;

                    bm.theSize++;
                    return true;
                }
                else {
                    if (modified) {
                        bm.write_node(root);
                    }
                    cnt--;

                    bm.theSize++;

                    return true;
                }
            }
        }

        bool erase(const key_t &Key) {
            //if bptree is empty
            if (bm.root_off == invalid_off) {
                return false;
            }

            //erase successfully
            Node &root = pool[cnt++];
            bm.get_root(root);

            bool modified = erase_node(root, Key);

            if (root.isLeaf) {
                if (root.key.size() == 0) {
                    bm.root_off = bm.head_off = bm.tail_off = invalid_off;

                    root.next = bm.trash_off;
                    bm.trash_off = root.addr;

                    bm.write_node(root);
                    cnt--;

                    bm.theSize--;
                    return true;
                }
                else {
                    if (modified) {
                        bm.write_node(root);
                        cnt--;

                        bm.theSize--;
                        return true;
                    }
                }
            }
            else {
                if (root.child.size() == 1) {
                    bm.root_off = root.child[0];

                    root.next = bm.trash_off;
                    bm.trash_off = root.addr;

                    bm.write_node(root);
                    cnt--;

                    bm.theSize--;
                    return true;
                }
                else {
                    if (modified) {
                        bm.write_node(root);
                        cnt--;

                        bm.theSize--;
                        return true;
                    }
                }
            }
        }


        size_t size() {
            return bm.theSize;
        }
    };
}

#endif