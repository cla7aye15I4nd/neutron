#ifndef BTREE_HPP
#define BTREE_HPP

#include <iostream>
#include <cstdio>
#include <cstring>
#include <queue>

using std::cout;
using std::cin;
using std::endl;
using std::cerr;
using std::queue;

//#define DEBUG
//#define DEBUG2
#define VIEW

namespace sjtu{
    typedef short Rank;
    typedef int offset;

    const int blockSize = 64;
    const offset invalid_off = -1;

    //root_off + end_off + trash_off = 12 bytes
    const int bptree_byte = 12;
    //isLeaf + next + keySize + pointSize = 1 + 4 + 2 + 2 = 9 bytes
    const int node_byte = 9;

    const int info_byte = blockSize - node_byte;

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

            char info[info_byte + sizeof(key_t) + sizeof(value_t) + 4];

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
                keySize = 0;
                pointSize = 0;
            }


#define next_key_inner(ptr) ((key_t*) ((char *) ptr + sizeof(offset) + sizeof(key_t)))
#define next_key_leaf(ptr)  ((key_t*) ((char *) ptr + sizeof(value_t) + sizeof(key_t)))

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
                for (Rank i = keySize; i > idx; --i)
                    *key_leaf(i) = *key_leaf(i - 1);
                *key_leaf(idx) = Key;
                keySize++;
            }

            void insert_value(Rank idx, const value_t &Value) {
                for (Rank i = pointSize; i > idx; --i)
                    *value(i) = *value(i - 1);

                *value(idx) = Value;
                pointSize++;
            }

            void insert_key_inner(Rank idx, const key_t &Key) {
                for (Rank i = keySize; i > idx; --i)
                    *key_inner(i) = *key_inner(i - 1);

                *key_inner(idx) = Key;
                keySize++;
            }

            void insert_child(Rank idx, const offset &off) {
                for (Rank i = pointSize; i > idx; --i)
                    *child(i) = *child(i - 1);

                *child(idx) = off;
                pointSize++;
            }

            void erase_key_leaf(Rank idx) {
                for (Rank i = idx + 1; i < keySize; ++i)
                    *key_leaf(i - 1) = *key_leaf(i);

                keySize--;
            }

            void erase_value(Rank idx) {
                for (Rank i = idx + 1; i < pointSize; ++i)
                    *value(i - 1) = *value(i);

                pointSize--;
            }

            void erase_key_inner(Rank idx) {
                for (Rank i = idx + 1; i < keySize; ++i)
                    *key_inner(i - 1) = *key_inner(i);

                keySize--;
            }

            void erase_child(Rank idx) {
                for (Rank i = idx + 1; i < pointSize; ++i)
                    *child(i - 1) = *child(i);

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

        struct pair {
            bool first;
            bool second;

            pair(bool a = true, bool b = true) : first(a), second(b) {}
        };

        char buf[blockSize];
//        offset buf_addr;

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
//            if (ret.addr == off)
//                return;
            ret.addr = off;
            fseek(file, off, SEEK_SET);

            fread(&ret.next, sizeof(offset), 1, file);
            fread(&ret.keySize, sizeof(Rank), 1, file);
            fread(&ret.pointSize, sizeof(Rank), 1, file);
            fread(&ret.isLeaf, sizeof(bool), 1, file);

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

            fwrite(&cur.next, sizeof(offset), 1, file);
            fwrite(&cur.keySize, sizeof(Rank), 1, file);
            fwrite(&cur.pointSize, sizeof(Rank), 1, file);
            fwrite(&cur.isLeaf, sizeof(bool), 1, file);

            if (cur.isLeaf) {
                fwrite(cur.info, 1, sizeof(key_t) * cur.keySize + sizeof(value_t) * cur.pointSize, file);
            }
            else {
                fwrite(cur.info, 1, sizeof(key_t) * cur.keySize + sizeof(offset) * cur.pointSize, file);
            }
        }

        void write_trash(Node &cur) {
            fseek(file, cur.addr, SEEK_SET);
            fwrite(&cur.next, sizeof(offset), 1, file);
        }

        void get_trash(offset off, Node &ret) {
            ret.addr = off;
            fseek(file, off, SEEK_SET);

            fread(&ret.next, sizeof(offset), 1, file);
        }

        void append_block(Node &ret, bool leaf) {
            if (trash_off == invalid_off) {
                ret.addr = end_off;
                end_off += blockSize;
                ret.isLeaf = leaf;
            }
            else {
                ret.addr = trash_off;
                get_trash(trash_off, trashNode);
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

            key_t midKey = *(n.key_leaf(mid - 1));

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

        pair insert_node(Node &cur, const key_t &Key, const value_t &Value) {
            if (cur.isLeaf) {
                Rank idx = cur.search_upper_leaf(Key);

                if (idx == -1) {
                    //fail
                    if (*(cur.key_leaf(cur.keySize - 1)) == Key)
                        return pair(false, false);
                    //success
                    else {
                        cur.push_back_key_leaf(Key);
                        cur.push_back_value(Value);
                        return pair(true, true);
                    }
                }
                else {
                    //fail
                    if (idx != 0 && *(cur.key_leaf(idx - 1)) == Key)
                        return pair(false, false);
                    //success
                    else {
                        cur.insert_key_leaf(idx, Key);
                        cur.insert_value(idx, Value);
                        return pair(true, true);
                    }
                }
            }
            else {
                Node ch;
                Rank chPos = find_child(Key, cur, ch);

                pair flag = insert_node(ch, Key, Value);

                if (!flag.first) {
                    return pair(false, false);
                }

                if (ch.isLeaf) {
                    if (ch.keySize <= leaf_max) {
                        write_block(ch);
                        return pair(true, false);
                    }
                    else {
                        Node newLeaf;
                        append_block(newLeaf, true);

                        newLeaf.next = ch.next;
                        ch.next = newLeaf.addr;

                        split_leaf(ch, newLeaf);

                        cur.insert_key_inner(chPos, *(newLeaf.key_leaf(0)));
                        cur.insert_child(chPos + 1, newLeaf.addr);

                        write_block(ch);
                        write_block(newLeaf);

                        return pair(true, true);
                    }
                }
                    // child is inner node
                else {
                    if (ch.pointSize <= nonleaf_max) {
                        if (flag.second) {
                            write_block(ch);
                        }
                        return pair(true, false);
                    }
                    else {
                        key_t midKey;

                        Node newInner;
                        append_block(newInner, false);

                        midKey = split_nonleaf(ch, newInner);

                        cur.insert_key_inner(chPos, midKey);
                        cur.insert_child(chPos + 1, newInner.addr);

                        write_block(ch);
                        write_block(newInner);

                        return pair(true, true);
                    }
                }
            }
        }

        pair erase_node(Node &cur, const key_t &Key) {
            if (cur.isLeaf) {
                Rank delPos = cur.search_leaf(Key);

                if (delPos == -1)
                    return pair(false, false);
                else {
                    cur.erase_key_leaf(delPos);
                    cur.erase_value(delPos);
//#ifdef DEBUG
//                    puts("current node is a leaf node");
//                    cur.view();
//#endif
                    return pair(true, true);
                }
            }
            else {
                Node ch;

                Rank chPos = find_child(Key, cur, ch);

                pair flag = erase_node(ch, Key);

                if (!flag.first) {
                    return pair(false, false);
                }

                if (ch.isLeaf) {
                    if (ch.keySize >= (leaf_max + 1) / 2) {
                        write_block(ch);
                        return pair(true, false);
                    }
                    else {
                        Node sbl;
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
                            write_trash(*rightNode);

                            return pair(true, true);
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

                                return pair(true, true);
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

                                return pair(true, true);
                            }
                        }
                    }
                }
                    // ch is inner node
                else {
                    if (ch.pointSize >= (nonleaf_max + 1) / 2) {
                        if (flag.second) {
                            write_block(ch);
                        }
                        return pair(true, false);
                    }
                    else {
                        Node sbl;
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
                            write_trash(*rightNode);

                            return pair(true, true);
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

                                return pair(true, true);
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

                                return pair(true, true);
                            }
                        }
                    }
                }
            }
        }

    public:
        bptree(const char *fname) {
            strcpy(filename, fname);
            leaf_max = (blockSize -  node_byte) / (sizeof(key_t) + sizeof(value_t)) - 1;
            nonleaf_max = (blockSize - node_byte + sizeof(key_t)) / (sizeof(key_t) + sizeof(offset)) - 1;

            file = fopen(filename, "rb+");
            if (!file) {
                file = fopen(filename, "wb+");
                root_off = trash_off = invalid_off;
                end_off = bptree_byte;
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
#ifdef DEBUG2
        void close_file() {
            fseek(file, 0, SEEK_SET);
            fwrite(&root_off, sizeof(offset), 1, file);
            fwrite(&end_off, sizeof(offset), 1, file);
            fwrite(&trash_off, sizeof(offset), 1, file);
            fclose(file);
        }

        void open_file() {
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

#endif
        void clear_file() {
            file = fopen(filename, "wb+");
            root_off = trash_off = invalid_off;
            end_off = bptree_byte;
        }

        value_t find(const key_t &Key) {
            Node t;
            find_leaf(Key, t);

            Rank idx = t.search_leaf(Key);

            if (idx == -1)
                return value_t();
            else
                return *(t.value(idx));
        }

        int count(const key_t &Key) {
            Node t;
            find_leaf(Key, t);

            Rank idx = t.search_leaf(Key);

            if (idx == -1)
                return 0;
            else
                return 1;
        }

        bool modify(const key_t &Key, const value_t &Value) {
            Node t;
            find_leaf(Key, t);

            Rank idx = t.search_leaf(Key);

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
            Node root;

            if (root_off == invalid_off) {
                append_block(root, true);

                root.push_back_key_leaf(Key);
                root.push_back_value(Value);

                root_off = root.addr;
                write_block(root);

                return true;
            }

            get_root(root);
            pair flag = insert_node(root, Key, Value);

            if (!flag.first) {
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
                        Node newLeaf;
                        append_block(newLeaf, true);

                        split_leaf(root, newLeaf);
#ifdef DEBUG
                        puts("view root and new leaf node");
                        root.view();
                        newLeaf.view();
#endif
                        newLeaf.next = root.next;
                        root.next = newLeaf.addr;

                        Node newRoot;
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

                        return true;
                    }
                    else {
                        if (flag.second) {
                            write_block(root);
                        }
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

                        Node newInner;
                        append_block(newInner, false);


                        Node newRoot;
                        append_block(newRoot, false);

                        key_t midKey = split_nonleaf(root, newInner);

                        newRoot.push_back_key_inner(midKey);
                        newRoot.push_back_child(root.addr);
                        newRoot.push_back_child(newInner.addr);
                        root_off = newRoot.addr;

                        write_block(newRoot);
                        write_block(root);
                        write_block(newInner);
                        return true;
                    }
                    else {
                        if (flag.second) {
                            write_block(root);
                        }
                        return true;
                    }
                }
            }
        }

        bool erase(const key_t &Key) {
            if (root_off == invalid_off)
                return false;

            Node root;
            get_root(root);

            pair flag = erase_node(root, Key);

            if (!flag.first) {
                return false;
            }

            if (root.isLeaf) {
                if (root.keySize == 0) {
                    root_off = invalid_off;

                    root.next = trash_off;
                    trash_off = root.addr;

                    write_trash(root);
                    return true;
                }
                else {
                    if (flag.second) {
                        write_block(root);
                    }
                    return true;
                }
            }
            else {
                if (root.pointSize == 1) {
                    root_off = *(root.child(0));

                    root.next = trash_off;
                    trash_off = root.addr;

                    write_trash(root);
                    return true;
                }
                else {
                    if (flag.second) {
                        write_block(root);
                    }
                    return true;
                }
            }
        }

        void build(key_t keys[], value_t values[], int len) {
            int newLen = 0;
            int rank = 0;

            key_t newKeys[len / leaf_max + 1];
            offset newChilds[len / leaf_max + 1];

            Node ret;
            while (len > 2 * leaf_max) {
                ret.init();
                append_block(ret, true);
                newKeys[newLen] = keys[rank];
                newChilds[newLen++] = ret.addr;

                while (ret.keySize <= leaf_max) {
                    ret.push_back_key_leaf(keys[rank]);
                    ret.push_back_value(values[rank++]);
                    len--;
                }
                ret.next = end_off;
//                ret.view();
                write_block(ret);
            }

            Rank mid = (len + 1) / 2;
            ret.init();
            append_block(ret, true);
            newKeys[newLen] = keys[rank];
            newChilds[newLen++] = ret.addr;
            for (Rank i = 0; i < mid; ++i) {
                ret.push_back_key_leaf(keys[rank]);
                ret.push_back_value(values[rank++]);
            }
            //ret keysize is (len + 1) / 2 >= leaf_max
            ret.next = end_off;
            write_block(ret);

            ret.init();
            append_block(ret, true);
            newKeys[newLen] = keys[rank];
            newChilds[newLen++] = ret.addr;

            for (Rank i = mid; i < len; ++i) {
                ret.push_back_key_leaf(keys[rank]);
                ret.push_back_value(values[rank++]);
            }

            ret.next = invalid_off;
            write_block(ret);

//#define DEBUG3
#ifdef DEBUG3
            offset beg = 12;
            while (beg != invalid_off) {
                get_block(beg, ret);
                cout << '(' << *(ret.key_leaf(0)) << ',' << beg << ',' <<*(ret.value(0)) << ')' << endl;
                beg = ret.next;
//                if (ret.next == invalid_off)
                    ret.view();
            }

            for (int i = 0; i < newLen; ++i) {
                cout << '(' << newKeys[i] << ',' << newChilds[i] << ')' << endl;
            }
#endif

            build_parent(newKeys, newChilds, newLen);
        }

    private:

        void build_parent(key_t keys[], offset childs[], int len) {
            Node ret;
            int rank = 0;

            //if parent is root
            if (len <= nonleaf_max) {
                append_block(ret, false);
                ret.push_back_child(childs[rank++]);
                while (ret.pointSize < len) {
                    ret.push_back_key_inner(keys[rank]);
                    ret.push_back_child(childs[rank++]);
                }
                root_off = ret.addr;
                write_block(ret);
                return;
            }

            int newLen = 0;
            key_t newKeys[len / nonleaf_max + 1];
            offset newChilds[len / nonleaf_max + 1];

            while (len > 2 * nonleaf_max) {
                ret.init();
                append_block(ret, false);

                newKeys[newLen] = keys[rank];
                newChilds[newLen++] = ret.addr;

                ret.push_back_child(childs[rank++]);
                len--;
                while (ret.pointSize <= nonleaf_max) {
                    ret.push_back_key_inner(keys[rank]);
                    ret.push_back_child(childs[rank++]);
                    len--;
                }
                write_block(ret);
            }

            Rank mid = (len + 1) / 2;
            ret.init();
            append_block(ret, false);
            newKeys[newLen] = keys[rank];
            newChilds[newLen++] = ret.addr;

            ret.push_back_child(childs[rank++]);
            for (Rank i = 1; i < mid; ++i) {
                ret.push_back_key_inner(keys[rank]);
                ret.push_back_child(childs[rank++]);
            }
            write_block(ret);

            ret.init();
            append_block(ret, false);
            newKeys[newLen] = keys[rank];
            newChilds[newLen++] = ret.addr;

            ret.push_back_child(childs[rank++]);
            for (Rank i = mid + 1; i < len; ++i) {
                ret.push_back_key_inner(keys[rank]);
                ret.push_back_child(childs[rank++]);
            }

            write_block(ret);

            build_parent(newKeys, newChilds, newLen);
        }

    public:
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
            if (it.addr == invalid_off)
                return;

            if (it.idx == *(Rank *)(buf + 4) - 1) {
                offset nxt = *(offset *)(buf);
                it.addr = nxt;
                it.idx = 0;
                if (nxt != invalid_off) {
                    fseek(file, nxt, SEEK_SET);
                    fread(buf, 1, blockSize, file);
                }
            }
            else {
                it.idx++;
            }
        }

        key_t retKey(iterator it) {
//            fseek(file, it.addr + node_byte + sizeof(value_t) * (it.idx + 1) + sizeof(key_t) * it.idx, SEEK_SET);
//            fread(buf, sizeof(key_t), 1, file);

            return *(key_t *)(buf + node_byte + (sizeof(value_t) + sizeof(key_t)) * it.idx + sizeof(value_t));
        }

        value_t retValue(iterator it) {
//            fseek(file, it.addr + node_byte + sizeof(value_t) * it.idx + sizeof(key_t) * it.idx, SEEK_SET);
//            fread(buf, sizeof(value_t), 1, file);

            return *(value_t *)(buf + node_byte + (sizeof(value_t) + sizeof(key_t)) * it.idx);
        }

        iterator lower_bound(const key_t &Key) {
            Node tmp;
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

            iterator ret(addr, idx);
            if (addr != invalid_off) {
                fseek(file, addr, SEEK_SET);
                fread(buf, 1, blockSize, file);
            }
//            buf_addr = addr;
            return ret;
        }

#ifdef VIEW
        void tree_info() {
            cout << "blockSize = " << blockSize << endl;
            cout << "leaf_max = " << leaf_max << endl;
            cout << "nonleaf_max = " << nonleaf_max << endl;
        }

        void view_root() {
            Node root;
            get_root(root);
            puts("root");
            cout << "keysize = " << root.keySize << " pointsize = " << root.pointSize << endl;
            root.view();
//            Node ch;
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

            Node t;
            get_block(addr, t);
            cout << "keysize = " << t.keySize << " pointsize = " << t.pointSize << endl;
            t.view();
            if (!t.isLeaf) {
                cout << "t is inner node" << endl;
                for (Rank i = 0; i < t.pointSize; ++i) {
                    view(*(t.child(i)));
                }
            }
        }

        struct node {
            offset addr;
            int height;

            node(offset a, int h) : addr(a), height(h) {}
        };


        queue<node> que;
        void traverse() {
            tree_info();
            if (root_off == invalid_off) {
                puts("empty");
                return;
            }
            que.push(node(root_off, 0));
            while (!que.empty()) {
                node temp = que.front();
                que.pop();

                Node tmp;
                get_block(temp.addr, tmp);
                if (tmp.isLeaf) {
                    cout << "height:" << temp.height << ", keySize:" << tmp.keySize << ", ";
                    for (Rank i = 0; i < tmp.keySize; ++i) {
                        cout << '(' << *tmp.key_leaf(i) << ',' << *tmp.value(i) << ") ";
                    }
                    puts("");
                }
                else {
                    cout << "height:" << temp.height << ", keySize:" << tmp.keySize <<", childSize:" << tmp.pointSize << endl;
                    for (Rank i = 0; i < tmp.keySize; ++i) {
                        cout << *tmp.key_inner(i) << ' ';
                    }
                    puts("");

                    for (Rank i = 0; i < tmp.pointSize; ++i)
                        que.push(node(*(tmp.child(i)), temp.height + 1));
                }
            }
        }

#endif

    };
}

#endif