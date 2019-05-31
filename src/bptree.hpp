#ifndef BTREE_HPP
#define BTREE_HPP

#include <iostream>
#include <cstdio>
#include <cstring>
#include "tools.hpp"

using std::cout;
using std::cin;
using std::endl;
//#define DEBUG
#define VIEW

namespace sjtu{
    template <typename key_t>
    void copy(char* dest, char* src, size_t length, size_t step1) {
        size_t i;
        size_t step2 = step1 * 2, step3 = step1 * 3, step4 = step1 * 4;
        for (i = 0; i + 4 < length; i += 4) {
            *((key_t*) dest) = *((key_t*) src);
            *((key_t*) (dest + step1)) = *((key_t*) (src + step1));
            *((key_t*) (dest + step2)) = *((key_t*) (src + step2));
            *((key_t*) (dest + step3)) = *((key_t*) (src + step3));
            dest += step4; src += step4;
        }
        for (; i < length; ++i) {
            *((key_t*) dest) = *((key_t*) src);
            dest += step1; src += step1;
        }
    }

    template <typename key_t>
    void rcopy(char* dest, char* src, size_t length, size_t step1) {
        size_t i;
        size_t step2 = step1 * 2, step3 = step1 * 3, step4 = step1 * 4;
        for (i = 0; i + 4 < length; i += 4) {
            *((key_t*) dest) = *((key_t*) src);
            *((key_t*) (dest - step1)) = *((key_t*) (src - step1));
            *((key_t*) (dest - step2)) = *((key_t*) (src - step2));
            *((key_t*) (dest - step3)) = *((key_t*) (src - step3));
            dest -= step4; src -= step4;
        }
        for (; i < length; ++i) {
            *((key_t*) dest) = *((key_t*) src);
            dest -= step1; src -= step1;
        }
    }

    typedef short Rank;
    typedef int offset;

    const int blockSize = 131072;
    const offset invalid_off = -1;

    //root_off + end_off + trash_off = 12 bytes
    const offset bptree_byte = 12;
    //isLeaf + next + keySize + pointSize = 1 + 4 + 2 + 2 = 9 bytes
    const offset node_byte = 9;

    template <class key_t, class value_t>
    class bptree {
        friend class iterator;

    private:
        struct Node {
            offset addr;
            offset next;

            Rank keySize;  //number of key
            Rank pointSize; //number of value or child

            bool isLeaf;

            char info[blockSize - node_byte];

            Node(bool leaf = true) {
                isLeaf = leaf;
                addr = invalid_off;
                next = invalid_off;
                keySize = 0;
                pointSize = 0;
            }

            ~Node() {
            }

            void init(bool leaf = true) {
                isLeaf = leaf;
                addr = invalid_off;
                next = invalid_off;
                keySize = 0;
                pointSize = 0;
            }

#define LEAF_STEP  (sizeof(value_t) + sizeof(key_t))
#define INNER_STEP (sizeof(offset) + sizeof(key_t))
#define CHILD_STEP (sizeof(offset) + sizeof(key_t))
#define VALUE_STEP (sizeof(value_t) + sizeof(key_t))
#define next_key_inner(ptr) ((key_t*) ((char *) ptr + INNER_STEP))
#define next_key_leaf(ptr)  ((key_t*) ((char *) ptr + LEAF_STEP))

        key_t* key_inner(Rank n) {
            return (key_t *)(info + (sizeof(offset) + sizeof(key_t)) * n + sizeof(offset));
        }
        key_t* key_leaf(Rank n) {
            return (key_t *)(info + (sizeof(value_t) + sizeof(key_t)) * n + sizeof(value_t));
        }
        value_t* value(Rank n) {
            return (value_t *)(info + (sizeof(value_t) + sizeof(key_t)) * n);
        }
        offset* child(Rank n) {
            return (offset *)(info + (sizeof(offset) + sizeof(key_t)) * n);
        }

#define BINARY_SEARCH_LOWER(statement, rejudge) do{                     \
                Rank l = 0, r = keySize;                                \
                while (l < r) {                                         \
                    Rank mid = l + (r - l) / 2;                         \
                    if (statement) l = mid + 1;                         \
                    else r = mid;                                       \
                }                                                       \
                return (rejudge) ? l: -1;                               \
            } while (0);                                                \

#define LINEAR_SEARCH(statement, first, next) do {                      \
                Rank idx; key_t* ptr =  first;                          \
                for (idx = 0; idx < keySize; ++idx) {                   \
                    if (statement) break;                               \
                    ptr = next(ptr);                                    \
                }                                                       \
                return idx == keySize ? -1: idx;                        \
            } while(0);                                                 \

#ifdef VIEW
            void view() {
                cout << "Keysize = " << keySize << endl;
                if (isLeaf) {
                    for (Rank i = 0; i < keySize; ++i) {
                        cout << *key_leaf(i) << ':' << *value(i);
                        puts("");
                    }
                }
                else {
                    for (Rank i = 0; i < keySize; ++i) {
                        cout << *key_inner(i) << ' ';
                    }
                    puts("");
                }
            }

#endif

#define MAX_SIZE_LOWER 4
            Rank search_leaf(const key_t &Key) {
                if (keySize <= MAX_SIZE_LOWER) {
                    LINEAR_SEARCH(*ptr == Key, key_leaf(0), next_key_leaf);
                } else {
                    BINARY_SEARCH_LOWER(*key_leaf(mid) < Key, *key_leaf(l) == Key);
                }
            }

            Rank search_inner(const key_t &Key) {
                if (keySize <= MAX_SIZE_LOWER) {
                    LINEAR_SEARCH(*ptr == Key, key_inner(0), next_key_inner);
                } else {
                    BINARY_SEARCH_LOWER(*key_inner(mid) < Key, *key_inner(l) == Key);
                }
            }

            Rank search_lower_leaf(const key_t &Key) {
                if (keySize <= MAX_SIZE_LOWER) {
                    LINEAR_SEARCH(*ptr >= Key, key_leaf(0), next_key_leaf);
                } else {
                    BINARY_SEARCH_LOWER(*key_leaf(mid) < Key, *key_leaf(l) >= Key);
                }
            }

            Rank search_upper_inner(const key_t &Key) {
                if (keySize <= MAX_SIZE_LOWER) {
                    LINEAR_SEARCH(*ptr > Key, key_inner(0), next_key_inner);
                } else {
                    BINARY_SEARCH_LOWER(*key_inner(mid) <= Key, *key_inner(l) > Key)
                }
            }

            Rank search_upper_leaf(const key_t &Key) {
                if (keySize <= MAX_SIZE_LOWER) {
                    LINEAR_SEARCH(*key_leaf(idx) > Key, key_leaf(0), next_key_leaf);
                } else {
                    BINARY_SEARCH_LOWER(*key_leaf(mid) <= Key, *key_leaf(l) > Key)
                }
            }

            void insert_key_leaf(Rank idx, const key_t &Key) {
                rcopy<key_t>((char*) key_leaf(keySize), (char*) key_leaf(keySize - 1), keySize - idx + 1, LEAF_STEP);
                *key_leaf(idx) = Key;
                keySize++;
            }

            void insert_value(Rank idx, const value_t &Value) {
                rcopy<value_t>((char*) value(pointSize), (char*) value(pointSize - 1), pointSize - idx + 1, VALUE_STEP);
                *value(idx) = Value;
                pointSize++;
            }

            void insert_key_inner(Rank idx, const key_t &Key) {
                rcopy<key_t>((char*) key_inner(keySize), (char*) key_inner(keySize - 1), keySize - idx + 1, INNER_STEP);
                *key_inner(idx) = Key;
                keySize++;
            }

            void insert_child(Rank idx, const offset &off) {
                rcopy<offset>((char*) child(pointSize), (char*) child(pointSize - 1), pointSize - idx + 1, CHILD_STEP);
                *child(idx) = off; 
                pointSize++;
            }

            void erase_key_leaf(Rank idx) {
                copy<key_t>((char*) key_leaf(idx), (char*) key_leaf(idx + 1), keySize - idx, LEAF_STEP);
                keySize--;
            }

            void erase_value(Rank idx) {
                copy<value_t>((char*) value(idx), (char*) value(idx + 1), pointSize - idx, VALUE_STEP);
                pointSize--;
            }

            void erase_key_inner(Rank idx) {
                copy<key_t>((char*) key_inner(idx), (char*) key_inner(idx + 1), keySize - idx, INNER_STEP);
                keySize--;
            }

            void erase_child(Rank idx) {
                copy<offset>((char*) child(idx), (char*) child(idx + 1), pointSize - idx, CHILD_STEP);
                pointSize--;
            }

            void push_back_key_inner(const key_t &Key) {
                *key_inner(keySize) = Key;
                keySize++;
            }

            void push_back_key_leaf(const key_t &Key) {
                *key_leaf(keySize) = Key;
                keySize++;
            }

            void push_back_value(const value_t &Value) {
                *value(pointSize) = Value;
                pointSize++;
            }

            void push_back_child(const offset &Off) {
                *child(pointSize) = Off;
                pointSize++;
            }

            void pop_back_key() {
                keySize--;
            }

            void pop_back_child() {
                pointSize--;
            }

            void pop_back_value() {
                pointSize--;
            }


        };

        int cnt;
        Node pool[50];
        char buf[blockSize];

        Rank leaf_max;
        Rank nonleaf_max;

        char filename[50];
        FILE *file;

        Node trashNode;

    public:
        offset root_off;
        offset trash_off;
        offset end_off;

    private:
        void get_block(offset off, Node &ret) {
            if (ret.addr == off)
                return;
            ret.addr = off;
            fseek(file, off, SEEK_SET);

            fread(&ret.isLeaf, sizeof(bool), 1, file);
            fread(&ret.next, sizeof(offset), 1, file);
            fread(&ret.keySize, sizeof(Rank), 1, file);
            fread(&ret.pointSize, sizeof(Rank), 1, file);

            if (ret.isLeaf) {
                fread(ret.info, 1, sizeof(key_t) * ret.keySize + sizeof(value_t) * ret.pointSize, file);
            }
            else {
                fread(ret.info, 1, sizeof(key_t) * ret.keySize + sizeof(offset) * ret.pointSize, file);
            }
        }

        bool get_root(Node &ret) {
            if (root_off == invalid_off)
                return false;
            else {
                get_block(root_off, ret);
                return true;
            }
        }

        void write_block(Node &cur) {
            fseek(file, cur.addr, SEEK_SET);

            fwrite(&cur.isLeaf, sizeof(bool), 1, file);
            fwrite(&cur.next, sizeof(offset), 1, file);
            fwrite(&cur.keySize, sizeof(Rank), 1, file);
            fwrite(&cur.pointSize, sizeof(Rank), 1, file);

            if (cur.isLeaf) {
                fwrite(cur.info, 1, sizeof(key_t) * cur.keySize + sizeof(value_t) * cur.pointSize, file);
            }
            else {
                fwrite(cur.info, 1, sizeof(key_t) * cur.keySize + sizeof(offset) * cur.pointSize, file);
            }
        }

        void append_block(Node &ret, bool leaf) {
            ret.init();
            if (trash_off == invalid_off) {
                ret.addr = end_off;
                end_off += blockSize;
                ret.isLeaf = leaf;
            }
            else {
                ret.addr = trash_off;
                get_block(trash_off, trashNode);
                trash_off = trashNode.next;
                ret.isLeaf = leaf;
            }
        }


    private:

        void find_leaf(const key_t &Key, Node &ret) {
            get_root(ret);

            while (!ret.isLeaf) {
                find_child(Key, ret, ret);
            }
        }

        Rank find_child(const key_t &Key, Node &cur, Node &ch) {
            Rank idx = cur.search_upper_inner(Key);

            if (idx == -1) {
                get_block(*(cur.child(cur.pointSize - 1)), ch);
                return cur.pointSize - 1;
            }
            else {
                get_block(*(cur.child(idx)), ch);
                return idx;
            }
        }

        //pass
        key_t split_nonleaf(Node &n, Node &n2) {
            Rank mid = (nonleaf_max + 1) / 2;

            key_t midKey = *(n.key_inner(mid - 1));

            for (Rank i = mid; i < n.keySize; ++i) {
                *(n2.key_inner(i - mid)) = *(n.key_inner(i));
            }

            for (Rank i = mid; i < n.pointSize; ++i) {
                *(n2.child(i - mid)) = *(n.child(i));
            }

            n2.keySize = n.keySize - mid;
            n2.pointSize = n.pointSize - mid;
            n.keySize = mid - 1;
            n.pointSize = mid;

            return midKey;
        }

        //pass
        void split_leaf(Node &n, Node &n2) {
            Rank mid = (leaf_max + 1) / 2;

            for (Rank i = mid; i < n.keySize; ++i) {
                *(n2.key_leaf(i - mid)) = *(n.key_leaf(i));
            }

            for (Rank i = mid; i < n.pointSize; ++i) {
                *(n2.value(i - mid)) = *(n.value(i));
            }

            n2.keySize = n.keySize - mid;
            n2.pointSize = n.pointSize - mid;
            n.keySize = mid;
            n.pointSize = mid;
        }


        bool insert_in_leaf(Node &cur, const key_t &Key, const value_t &Value) {
            Rank idx = cur.search_upper_leaf(Key);

            if (idx == -1) {
                if (*(cur.key_leaf(cur.keySize - 1)) == Key)
                    return false;
                else {
                    cur.push_back_key_leaf(Key);
                    cur.push_back_value(Value);
                    return true;
                }
            }
            else {
                if (idx != 0 && *(cur.key_leaf(idx - 1)) == Key)
                    return false;
                else {
                    cur.insert_key_leaf(idx, Key);
                    cur.insert_value(idx, Value);
                    return true;
                }
            }

        }


        bool insert_node(Node &cur, const key_t &Key, const value_t &Value) {
            if (cur.isLeaf) {
                bool flag = insert_in_leaf(cur, Key, Value);
#ifdef DEBUG
                puts("view current node");
                cur.view();
#endif
                return flag;
            }
            else {
                Node &ch = pool[cnt++];
                Rank chPos = find_child(Key, cur, ch);

                bool flag = insert_node(ch, Key, Value);

                if (!flag) {
                    cnt--;
                    return false;
                }
                else {
                    if (ch.isLeaf) {
                        if (ch.keySize <= leaf_max) {
                            write_block(ch);
                            cnt--;
                            return true;
                        }
                        else {
                            Node &newLeaf = pool[cnt++];
                            append_block(newLeaf, true);

                            newLeaf.next = ch.next;
                            ch.next = newLeaf.addr;

                            split_leaf(ch, newLeaf);

                            cur.insert_key_inner(chPos, *(newLeaf.key_leaf(0)));
                            cur.insert_child(chPos + 1, newLeaf.addr);

                            write_block(ch);
                            write_block(newLeaf);
                            cnt -= 2;

                            return true;
                        }
                    }
                        // child is inner node
                    else {
                        if (ch.pointSize <= nonleaf_max) {
                            write_block(ch);
                            cnt--;
                            return true;
                        }
                        else {
                            key_t midKey;

                            Node &newInner = pool[cnt++];
                            append_block(newInner, false);

                            midKey = split_nonleaf(ch, newInner);

                            cur.insert_key_inner(chPos, midKey);
                            cur.insert_child(chPos + 1, newInner.addr);

                            write_block(ch);
                            write_block(newInner);
                            cnt -= 2;

                            return true;
                        }
                    }
                }
            }
        }

        bool erase_node(Node &cur, const key_t &Key) {
            if (cur.isLeaf) {
                Rank delPos = cur.search_leaf(Key);

                if (delPos == -1)
                    return false;
                else {
                    cur.erase_key_leaf(delPos);
                    cur.erase_value(delPos);
//#ifdef DEBUG
//                    puts("current node is a leaf node");
//                    cur.view();
//#endif
                    return true;
                }
            }
            else {
                Node &ch = pool[cnt++];

                Rank chPos = find_child(Key, cur, ch);

                bool flag = erase_node(ch, Key);

                if (!flag) {
                    cnt--;
                    return false;
                }

                if (ch.isLeaf) {
                    if (ch.keySize >= (leaf_max + 1) / 2) {
                        write_block(ch);
                        cnt--;
                        return true;
                    }
                    else {
                        Node &sbl = pool[cnt++];
                        Rank sblPos;
                        Rank keyPos;

                        Node *leftNode, *rightNode;

                        if (chPos == 0) {
                            sblPos = 1;
                        }
                        else {
                            sblPos = chPos - 1;
                        }
                        get_block(*(cur.child(sblPos)), sbl);

                        keyPos = std::min(chPos, sblPos);

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
                                leftNode->push_back_key_leaf(*(rightNode->key_leaf(i)));
                                leftNode->push_back_value(*(rightNode->value(i)));
                            }

                            cur.erase_key_inner(keyPos);
                            cur.erase_child(keyPos + 1);

                            rightNode->next = trash_off;
                            trash_off = rightNode->addr;

                            write_block(*leftNode);
                            write_block(*rightNode);
                            cnt -= 2;

                            return true;
                        }
                        else {
                            if (sblPos < chPos) {
                                key_t borrow_key = *(sbl.key_leaf(sbl.keySize - 1));
                                value_t borrow_value = *(sbl.value(sbl.pointSize - 1));

                                sbl.pop_back_key();
                                sbl.pop_back_value();

                                ch.insert_key_leaf(0, borrow_key);
                                ch.insert_value(0, borrow_value);

                                *(cur.key_inner(keyPos)) = *(ch.key_leaf(0));

                                write_block(ch);
                                write_block(sbl);
                                cnt -= 2;

                                return true;
                            }
                            else {
                                key_t borrow_key = *(sbl.key_leaf(0));
                                value_t borrow_value = *(sbl.value(0));

                                sbl.erase_key_leaf(0);
                                sbl.erase_value(0);

                                ch.push_back_key_leaf(borrow_key);
                                ch.push_back_value(borrow_value);

                                *(cur.key_inner(keyPos)) = *(sbl.key_leaf(0));

                                write_block(ch);
                                write_block(sbl);
                                cnt -= 2;

                                return true;
                            }
                        }
                    }
                }
                    // ch is inner node
                else {
                    if (ch.pointSize >= (nonleaf_max + 1) / 2) {
                        write_block(ch);
                        cnt--;
                        return true;
                    }
                    else {
                        Node &sbl = pool[cnt++];
                        Rank sblPos;
                        Rank keyPos;

                        Node *leftNode, *rightNode;

                        if (chPos == 0) {
                            sblPos = 1;
                        }
                        else {
                            sblPos = chPos - 1;
                        }
                        get_block(*(cur.child(sblPos)), sbl);

                        keyPos = std::min(chPos, sblPos);

                        if (ch.pointSize + sbl.pointSize <= nonleaf_max) {
                            if (chPos < sblPos) {
                                leftNode = &ch;
                                rightNode = &sbl;
                            }
                            else {
                                leftNode = &sbl;
                                rightNode = &ch;
                            }

                            leftNode->push_back_key_inner(*(cur.key_inner(keyPos)));
                            for (Rank i = 0; i < rightNode->keySize; ++i)
                                leftNode->push_back_key_inner(*(rightNode->key_inner(i)));
                            for (Rank i = 0; i < rightNode->pointSize; ++i)
                                leftNode->push_back_child(*(rightNode->child(i)));

                            cur.erase_key_inner(keyPos);
                            cur.erase_child(keyPos + 1);



                            rightNode->next = trash_off;
                            trash_off = rightNode->addr;

                            write_block(*leftNode);
                            write_block(*rightNode);
                            cnt -= 2;

                            return true;
                        }
                        else {
                            if (sblPos < chPos) {
                                key_t borrow_key = *(sbl.key_inner(sbl.keySize - 1));
                                offset borrow_child = *(sbl.child(sbl.pointSize - 1));

                                sbl.pop_back_key();
                                sbl.pop_back_child();

                                ch.insert_key_inner(0, *(cur.key_inner(keyPos)));
                                ch.insert_child(0, borrow_child);

                                *(cur.key_inner(keyPos)) = borrow_key;

                                write_block(ch);
                                write_block(sbl);
                                cnt -= 2;

                                return true;
                            }
                            else {
                                key_t borrow_key = *(sbl.key_inner(0));
                                offset borrow_child = *(sbl.child(0));

                                sbl.erase_key_inner(0);
                                sbl.erase_child(0);

                                ch.push_back_key_inner(*(cur.key_inner(keyPos)));
                                ch.push_back_child(borrow_child);

                                *(cur.key_inner(keyPos)) = borrow_key;

                                write_block(ch);
                                write_block(sbl);
                                cnt -= 2;

                                return true;
                            }
                        }
                    }
                }
            }
        }

    public:
        bptree(const char *fname) {
            strcpy(filename, fname);
            //先开小一点，免得出问题
            leaf_max = (blockSize -  node_byte) / (sizeof(key_t) + sizeof(value_t)) - 1;
            nonleaf_max = (blockSize - node_byte + sizeof(key_t)) / (sizeof(key_t) + sizeof(offset)) - 1;

            file = fopen(filename, "rb+");
            if (!file) {
                file = fopen(filename, "wb+");
                root_off = trash_off = invalid_off;
                end_off = bptree_byte;

                fwrite(&root_off, sizeof(offset), 1, file);
                fwrite(&end_off, sizeof(offset), 1, file);
                fwrite(&trash_off, sizeof(offset), 1, file);
            }
            else {
                fread(&root_off, sizeof(offset), 1, file);
                fread(&end_off, sizeof(offset), 1, file);
                fread(&trash_off, sizeof(offset), 1, file);
            }
        }

        ~bptree() {
            fseek(file, 0, SEEK_SET);
            fwrite(&root_off, sizeof(offset), 1, file);
            fwrite(&end_off, sizeof(offset), 1, file);
            fwrite(&trash_off, sizeof(offset), 1, file);
            fclose(file);
        }

        void clear_file() {
            file = fopen(filename, "wb+");
            root_off = trash_off = invalid_off;
            end_off = bptree_byte;

            fwrite(&root_off, sizeof(offset), 1, file);
            fwrite(&end_off, sizeof(offset), 1, file);
            fwrite(&trash_off, sizeof(offset), 1, file);
        }

        value_t find(const key_t &Key) {
            Node &t = pool[cnt++];
            find_leaf(Key, t);

            Rank idx = t.search_leaf(Key);

            cnt--;
            if (idx == -1)
                return value_t();
            else
                return *(t.value(idx));
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
            cnt--;

            if (idx == -1) {
                return false;
            }
            else {
                *(t.value(idx)) = Value;
                write_block(t);
                return true;
            }
        }

        bool insert(const key_t &Key, const value_t &Value) {
            Node &root = pool[cnt++];

            if (root_off == invalid_off) {
                append_block(root, true);

                root.push_back_key_leaf(Key);
                root.push_back_value(Value);

                root_off = root.addr;
                write_block(root);
                cnt--;

                return true;
            }

            get_root(root);
            bool flag = insert_node(root, Key, Value);

            if (!flag) {
                cnt--;
                return false;
            }
            else {
                if (root.isLeaf) {
                    if (root.keySize > leaf_max) {
//#ifdef DEBUG
//                        static int dbg1 = 0;
//                        ++dbg1;
//                        puts("new root(leaf)");
//                        cout << dbg1 << endl;
//#endif
                        Node &newLeaf = pool[cnt++];
                        append_block(newLeaf, true);

                        split_leaf(root, newLeaf);
#ifdef DEBUG
                        puts("view root and new leaf node");
                        root.view();
                        newLeaf.view();
#endif
                        newLeaf.next = root.next;
                        root.next = newLeaf.addr;

                        Node &newRoot = pool[cnt++];
                        append_block(newRoot, false);

                        newRoot.push_back_key_inner(*(newLeaf.key_leaf(0)));
                        newRoot.push_back_child(root.addr);
                        newRoot.push_back_child(newLeaf.addr);
                        root_off = newRoot.addr;
#ifdef DEBUG
                        puts("view new root");
                        newRoot.view();
#endif

                        write_block(newRoot);
                        write_block(root);
                        write_block(newLeaf);
                        cnt -= 3;

                        return true;
                    }
                    else {
                        write_block(root);
                        cnt--;
                        return true;
                    }
                }
                else {
                    if (root.pointSize > nonleaf_max) {
//#ifdef DEBUG
//                        static int dbg2 = 0;
//                        ++dbg2;
//                        puts("new root(inner)");
//                        cout << dbg2 << endl;
//#endif

                        Node &newInner = pool[cnt++];
                        append_block(newInner, false);


                        Node &newRoot = pool[cnt++];
                        append_block(newRoot, false);

                        key_t midKey = split_nonleaf(root, newInner);

                        newRoot.push_back_key_inner(midKey);
                        newRoot.push_back_child(root.addr);
                        newRoot.push_back_child(newInner.addr);
                        root_off = newRoot.addr;

                        write_block(newRoot);
                        write_block(root);
                        write_block(newInner);
                        cnt -= 3;

                        return true;
                    }
                    else {
                        write_block(root);
                        cnt--;
                        return true;
                    }
                }
            }
        }

        bool erase(const key_t &Key) {
            if (root_off == invalid_off)
                return false;

            Node &root = pool[cnt++];
            get_root(root);

            bool flag = erase_node(root, Key);

            if (!flag) {
                cnt--;
                return false;
            }

            if (root.isLeaf) {
                if (root.keySize == 0) {
                    root_off = invalid_off;

                    root.next = trash_off;
                    trash_off = root.addr;

                    write_block(root);
                    cnt--;
                    return true;
                }
                else {
                    write_block(root);
                    cnt--;
                    return true;
                }
            }
            else {
                if (root.pointSize == 1) {
                    root_off = *(root.child(0));

                    root.next = trash_off;
                    trash_off = root.addr;

                    write_block(root);
                    cnt--;
                    return true;
                }
                else {
                    write_block(root);
                    cnt--;
                    return true;
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

            bool check() {
                if (addr == invalid_off)
                    return false;
                else
                    return true;
            }
        };

        void next_iterator(iterator &it) {
            fseek(file, it.addr + 1, SEEK_SET);
            fread(buf, sizeof(offset), 1, file);
            offset nxt = *(offset *)(buf);
            fread(buf, sizeof(Rank), 1, file);
            Rank key_size = *(Rank *)(buf);

            if (it.idx == key_size - 1) {
                it.addr = nxt;
                it.idx = 0;
            }
            else {
                it.idx++;
            }
        }

        key_t retKey(iterator it) {
            fseek(file, it.addr + node_byte + sizeof(value_t) * (it.idx + 1) + sizeof(key_t) * it.idx, SEEK_SET);
            fread(buf, sizeof(key_t), 1, file);

            return *(key_t *)(buf);
        }

        value_t retValue(iterator it) {
            fseek(file, it.addr + node_byte + sizeof(value_t) * it.idx + sizeof(key_t) * it.idx, SEEK_SET);
            fread(buf, sizeof(value_t), 1, file);

            return *(value_t *)(buf);
        }

        iterator lower_bound(const key_t &Key) {
            Node &tmp = pool[cnt++];
            find_leaf(Key, tmp);

            offset addr;
            Rank idx = tmp.search_lower_leaf(Key);
            if (idx == -1) {
                idx = 0;
                addr = tmp.next;
            }
            else {
                addr = tmp.addr;
            }

            cnt--;
            iterator ret(addr, idx);
            return ret;
        }

#ifdef VIEW
        void tree_info() {
            cout << "blockSize = " << blockSize << endl;
            cout << "leaf_max = " << leaf_max << endl;
            cout << "nonleaf_max = " << nonleaf_max << endl;
        }

        void view_root() {
            Node &root = pool[cnt];
            get_root(root);
            puts("root");
            cout << "keysize = " << root.keySize << " pointsize = " << root.pointSize << endl;
            root.view();
//            Node &ch = pool[cnt++];
//            if (!root.isLeaf) {
//                for (Rank i = 0; i < root.pointSize; ++i) {
//                    get_block(*(root.child(i)), ch);
//                    cout << "child:" << i << endl;
//                    ch.view();
//                }
//            }

        }

        void view(offset addr) {
            if (addr == invalid_off)
                return;

            Node &t = pool[cnt++];
            get_block(addr, t);
            cout << "keysize = " << t.keySize << " pointsize = " << t.pointSize << endl;
            t.view();
            if (!t.isLeaf) {
                cout << "t is inner node" << endl;
                for (Rank i = 0; i < t.pointSize; ++i) {
                    view(*(t.child(i)));
                }
            }
            cnt--;
        }
#endif

    };
}

#endif
