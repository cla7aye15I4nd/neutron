#ifndef BTREE_HPP
#define BTREE_HPP

#include <iostream>
#include <cstdio>
#include <cstring>

using std::cout;
using std::cin;
using std::endl;
#define DEBUG

namespace sjtu{
    typedef short Rank;
    typedef int offset;

    const int blockSize = 4096;
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
            char * info;

            Rank keySize;  //number of key
            Rank pointSize; //number of value or child

            bool isLeaf;

            offset addr;
            offset next;

            Node(bool leaf = true) {
                isLeaf = leaf;
                addr = invalid_off;
                next = invalid_off;
                keySize = 0;
                pointSize = 0;
                info = new char;
            }

            ~Node() {
                delete info;
            }

            void init(bool leaf = true) {
                isLeaf = leaf;
                addr = invalid_off;
                next = invalid_off;
                keySize = 0;
                pointSize = 0;
            }

            key_t *key_inner(Rank n) {
                return (key_t *)(info + sizeof(offset) * (n + 1) + sizeof(key_t) * n);
            }

            key_t *key_leaf(Rank n) {
                return (key_t *)(info + sizeof(value_t) * (n + 1) + sizeof(key_t) * n);
            }

            value_t *value(Rank n) {
                return (value_t *)(info + sizeof(value_t) * n + sizeof(key_t) * n);
            }

            offset *child(Rank n) {
                return (offset *)(info + sizeof(offset) * n + sizeof(key_t) * n);
            }

#ifdef DEBUG
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

            Rank search_leaf(const key_t &Key) {
                Rank idx;
                for (idx = 0; idx < keySize; ++idx) {
                    if (*key_leaf(idx) == Key)
                        break;
                }

                if (idx == keySize) {
                    return -1;
                }
                else {
                    return idx;
                }
            }

            Rank search_inner(const key_t &Key) {
                Rank idx;
                for (idx = 0; idx < keySize; ++idx) {
                    if (*key_inner(idx) == Key)
                        break;
                }

                if (idx == keySize)
                    return -1;
                else
                    return idx;
            }

            Rank search_upper_leaf(const key_t &Key) {
                Rank idx;
                for (idx = 0; idx < keySize; ++idx) {
                    if (*key_leaf(idx) > Key)
                        break;
                }

                if (idx == keySize) {
                    return -1;
                }
                else {
                    return idx;
                }
            }

            Rank search_upper_inner(const key_t &Key) {
                Rank idx;
                for (idx = 0; idx < keySize; ++idx) {
                    if (*key_inner(idx) > Key)
                        break;
                }

                if (idx == keySize) {
                    return -1;
                }
                else {
                    return idx;
                }
            }

            Rank search_lower_leaf(const key_t &Key) {
                Rank idx;
                for (idx = 0; idx < keySize; ++idx) {
                    if (*key_inner(idx) >= Key)
                        break;
                }

                if (idx == keySize) {
                    return -1;
                }
                else {
                    return idx;
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
                    *key_leaf(i - 1) = *key_leaf(i);

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

        short cnt;
        Node pool[100];
        char buf[blockSize];

        Rank leaf_max;
        Rank nonleaf_max;

        char filename[50];
        FILE *file;

        bool open;
        Node trashNode;

        offset root_off;
        offset trash_off;
        offset end_off;

        void createFile() {
            file = fopen(filename, "wb+");

            root_off = trash_off = invalid_off;
            end_off = bptree_byte;

            fwrite(&root_off, sizeof(offset), 1, file);
            fwrite(&end_off, sizeof(offset), 1, file);
            fwrite(&trash_off, sizeof(offset), 1, file);

            fclose(file);
        }

        void init() {
            file = fopen(filename, "rb+");
            if (!file) {
                createFile();
                file = fopen(filename, "rb+");
            }

            fread(&root_off, sizeof(offset), 1, file);
            fread(&end_off, sizeof(offset), 1, file);
            fread(&trash_off, sizeof(offset), 1, file);
        }

        void get_block(offset off, Node &ret) {
            ret.addr = off;
            fseek(file, off, SEEK_SET);

            fread(&ret.isLeaf, sizeof(bool), 1, file);
            fread(&ret.next, sizeof(offset), 1, file);
            fread(&ret.keySize, sizeof(Rank), 1, file);
            fread(&ret.pointSize, sizeof(Rank), 1, file);

            if (ret.isLeaf) {
                fread(&ret.info, 1, sizeof(key_t) * ret.keySize + sizeof(value_t) * ret.pointSize, file);
            }
            else {
                fread(&ret.info, 1, sizeof(key_t) * ret.keySize + sizeof(offset) * ret.pointSize, file);
            }
        }

        bool get_next(const Node &cur, Node &ret) {
            if (cur.next == invalid_off)
                return false;
            else {
                get_block(cur.next, ret);
                return true;
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
                fwrite(&cur.info, 1, sizeof(key_t) * cur.keySize + sizeof(value_t) * cur.pointSize, file);
            }
            else {
                fwrite(&cur.info, 1, sizeof(key_t) * cur.keySize + sizeof(offset) * cur.pointSize, file);
            }
        }

        void append_block(Node &ret, bool leaf) {
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

    public:
        void close_file() {
            if (open) {
                fseek(file, 0, SEEK_SET);
                fread(&root_off, sizeof(offset), 1, file);
                fread(&end_off, sizeof(offset), 1, file);
                fread(&trash_off, sizeof(offset), 1, file);

                fclose(file);
                file = NULL;
                open = false;
                root_off = trash_off = end_off = invalid_off;
            }
        }

        void open_file() {
            if (!open) {
                init();
                open = true;
            }
        }

        void set_filename(const char *fname) {
            strcpy(filename, fname);
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
                return cur.pointSize;
            }
            else {
                get_block(*(cur.child(idx)), ch);
                return idx;
            }
        }

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
            n2.pointSize = n2.pointSize - mid;
            n.keySize = mid - 1;
            n.pointSize = mid;

            return midKey;
        }

        void split_leaf(Node &n, Node &n2) {
            Rank mid = (leaf_max + 1) / 2;

            key_t midKey = *(n.key_inner(mid - 1));

            for (Rank i = mid; i < n.keySize; ++i) {
                *(n2.key_inner(i - mid)) = *(n.key_inner(i));
            }

            for (Rank i = mid; i < n.pointSize; ++i) {
                *(n2.value(i - mid)) = *(n.value(i));
            }

            n2.keySize = n.keySize - mid;
            n2.pointSize = n2.pointSize - mid;
            n.keySize = mid;
            n.pointSize = mid;
        }

        //pass
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

        //我去到底哪里写错了，insert写的非常有道理啊
        bool insert_node(Node &cur, const key_t &Key, const value_t &Value) {
            if (cur.isLeaf) {
                bool flag = insert_in_leaf(cur, Key, Value);
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

                            cur.insert_key_inner(chPos, *(newLeaf.key_inner(0)));
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
                else {
                    if (ch.isLeaf) {
                        if (ch.keySize >= (leaf_max + 1)/ 2) {
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
                        //child is inner node
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
                                    rightNode->push_back_child(*(rightNode->child(i)));

                                cur.erase_key_inner(keyPos);
                                cur.erase_child(keyPos + 1);

                                rightNode->next = trash_off;
                                trash_off = rightNode->addr;

                                write_block(*leftNode);
                                write_block(*rightNode);
                                cnt -= 2;

                                return true;
                            }
                                //borrow from sibling
                            else {
                                if (keyPos < chPos) {
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
            filename[0] = '\0';
            //先开小一点，免得出问题
            leaf_max = (blockSize -  node_byte) / (sizeof(key_t) + sizeof(value_t)) - 1;
            nonleaf_max = (blockSize - node_byte + sizeof(key_t)) / (sizeof(key_t) + sizeof(offset)) - 1;

            open = false;
            file = NULL;
            cnt = 0;

            root_off = end_off = trash_off = invalid_off;
        }

        ~bptree() {
            close_file();
        }

        void clear() {
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
                return *(t.key_leaf(idx));
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
            if (idx == -1) {
                cnt--;
                return false;
            }
            else {
                *(t.value(idx)) = Value;
                write_block(t);
                cnt--;
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
                        Node &newLeaf = pool[cnt++];
                        append_block(newLeaf, true);

                        split_leaf(root, newLeaf);
                        newLeaf.next = root.next;
                        root.next = newLeaf.addr;

                        Node &newRoot = pool[cnt++];
                        append_block(newRoot, false);

                        newRoot.push_back_key_inner(*(newLeaf.key_leaf(0)));
                        newRoot.push_back_child(root.addr);
                        newRoot.push_back_child(newLeaf.addr);
                        root_off = newRoot.addr;

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
            if (root_off == invalid_off) {
                return false;
            }

            Node &root = pool[cnt++];
            get_root(root);

            bool flag = erase_node(root, Key);

            if (!flag) {
                cnt--;
                return false;
            }
            else {
                if (root.isLeaf) {
                    if (root.keySize == 0) {
                        root.next = trash_off;
                        trash_off = root.addr;
                        root_off = invalid_off;

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
            fseek(file, it.addr + bptree_byte + sizeof(value_t) * (it.idx + 1) + sizeof(key_t) * it.idx, SEEK_SET);
            fread(buf, sizeof(key_t), 1, file);

            return *(key_t *)(buf);
        }

        value_t retValue(iterator it) {
            fseek(file, it.addr + bptree_byte + sizeof(value_t) * it.idx + sizeof(key_t) * it.idx, SEEK_SET);
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

            iterator ret(addr, idx);
            return ret;
        }

#ifdef DEBUG
        void view_root() {
            Node &root = pool[cnt++];
            get_root(root);
            root.view();
            cnt--;
        }
#endif
    };
}

#endif