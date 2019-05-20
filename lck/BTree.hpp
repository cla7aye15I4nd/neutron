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
    template <class key_t, class value_t>
    class bptree {
        friend class iterator;

        typedef char buffer_t[blockSize];
        typedef char * buffer_p;

    private:
        FILE *pfile;
        char *filename;
        const size_t nonleafMax;
        const size_t leafMax;

        struct Node {
//            char* buf;
//            key_t key;
//            *((key_t*)(buf + 5)) = key;
            key_t key;
            off_t pos;

            off_t parent;
            off_t next;

            size_t _size;

            bool isLeaf;

            Node(bool is_leaf = true) {
                pos = invalid_off;
                isLeaf = is_leaf;
                next = invalid_off;
                parent = invalid_off;
                _size = 0;
            }

            ~Node() {

            }

            void view() {
                cout << "Nodesize = " << key.size() << endl;
                if (isLeaf) {
                    for (int i = 0; i < key.size(); ++i) {
                        cout << key[i] << ":" << value[i] << ' ';
                    }
                    puts("");
                }
                else {
                    for (int i = 0; i < key.size(); ++i) {
                        cout << key[i] << ' ';
                    }
                    puts("");
                }
            }
        };

        char buffer[blockSize];

        inline void file_reopen() {
            if (pfile)  fflush(pfile);
        }

        inline void move_to_data(const Node &p) {
            fseek(pfile, p.pos + sizeof(Node), SEEK_SET);
        }

        inline void load_nonleaf(char *b, Node p) {
            move_to_data(p);

            if (p._size == 0)
                return;

            fread(b, 1, (sizeof(off_t) + sizeof(key_t) * p._size), pfile);
        }

        inline void save_nonleaf(char *b, Node p) {
            move_to_data(p);
            fwrite(b, 1, (sizeof(off_t) + sizeof(key_t)) * p.sz, pfile);
            file_reopen();
        }

        inline void loaf_leaf(char *b, Node p) {
            move_to_data(p);

            if (p._size == 0)
                return;

            fread(b, 1, (sizeof(value_t) + sizeof(key_t)) * p._size, pfile);
        }

        inline void save_leaf(char *b, Node p) {
            move_to_data(p);
            fwrite(b, 1, (sizeof(value_t) + sizeof(key_t)) * p._size, pfile);
            file_reopen();
        }

        inline void save_node(const Node &p) {
            fseek(pfile, p.pos, SEEK_SET);
            fwrite(&p, sizeof(Node), 1, pfile);
            file_reopen();
        }

        //4 functions to get key, value or child
        key_t *nth_nonleaf_key(buffer_p b, int n) {
            return (key_t *)(b + sizeof(off_t) + (sizeof(off_t) + sizeof(key_t)) * n);
        }

        off_t *nth_nonleaf_child(buffer_p b, int n) {
            return (off_t *)(b + (sizeof(off_t) + sizeof(key_t)) * n);
        }

        key_t *nth_leaf_key(buffer_p b, int n) {
            return (key_t *)(b + sizeof(value_t) + (sizeof(value_t) + sizeof(key_t) * n));
        }

        value_t *nth_leaf_value(buffer_p b, int n) {
            return (value_t *)(b + (sizeof(value_t) + sizeof(key_t)) * n);
        }

        size_t search_nonleaf(buffer_p b, const key_t &Key, const size_t &size) {
            int idx;
            for (idx = 0; idx < size; ++idx) {
                if (Key < *nth_nonleaf_key(idx))
                    break;
            }

            return idx;
        }

        void insert_nonleaf(buffer_p b, const key_t &Key, const off_t &child, Node &p) {
            int idx = search_nonleaf(b, Key, p._size);

            if (idx < p._size && Key == nth_nonleaf_key(b, idx))
                return;

            //todo unsure the number of child
            for (int i = p._size; i > idx; --i) {
                *nth_nonleaf_key(b, i) = *nth_nonleaf_key(b, i - 1);
                *nth_nonleaf_child(b, i) = *nth_nonleaf_child(b, i - 1);
            }

            p._size++;
            *nth_nonleaf_key(b, idx) = Key;
            *nth_nonleaf_child(b, idx) = child;

            p.key = *nth_nonleaf_key(b, 0);
        }

        size_t search_leaf(buffer_p b, const key_t &Key, const size_t &size) {
            int idx;
            for (idx = 0; idx < size; ++idx) {
                if (Key < *nth_leaf_key(idx))
                    break;
            }

            return idx;
        }

        void insert_leaf() {

        }

        Node read_node(off_t pos) {
            Node r;
            fseek(pfile, pos, SEEK_SET);
            fread(&r, sizeof(Node), 1, pfile);
            return r;
        }



    public:
        off_t root;

    private:
        Node *find_leaf(const key_t &Key) {
            Node *t = root;
            Node *tmp;
            while (!t->isLeaf) {
                int idx;
                for (idx = 0; idx < t->key.size(); ++idx) {
                    if (Key < t->key[idx])
                        break;
                }
                tmp = t;
                t = t->child[idx];
                t->parent = tmp;
            }
            return t;
        }

        void insert_in_leaf(Node *lf, const key_t &Key, const value_t &Value) {
            int idx;
            for (idx = 0; idx < lf->key.size(); ++idx) {
                if (Key < lf->key[idx])
                    break;
            }
            lf->key.insert(idx, Key);
            lf->value.insert(idx, Value);

            return;
        }

        void insert_in_parent(Node *n, const key_t &Key, Node *n2) {
            if (n == root) {
                Node *newRoot = new Node();
                newRoot->key.push_back(Key);
                newRoot->child.push_back(n);
                newRoot->child.push_back(n2);
                root = newRoot;
                root->isLeaf = false;
                return;
            }

            Node *p = n->parent;
            // p is an inner Node
            if (p->child.size() < blockSize) {
                int idx;
                for (idx = 0; idx < p->child.size(); ++idx) {
                    if (p->child[idx] == n)
                        break;
                }
                p->key.insert(idx, Key);
                p->child.insert(idx + 1, n2);
            }
            else {
                /* Split P */
                Node *tmp = new Node;
                for (int i = 0; i < blockSize - 1; ++i) {
                    tmp->key.push_back(p->key[i]);
                    tmp->child.push_back(p->child[i]);
                }
                tmp->child.push_back(p->child[blockSize - 1]);

                tmp->key.push_back(Key);
                tmp->child.push_back(n2);

                p->key.clear();
                p->child.clear();
                Node *p2 = new Node();

                for (int i = 0; i <= (blockSize + 1) / 2 - 1; ++i) {
                    p->key.push_back(tmp->key[i]);
                    p->child.push_back(tmp->child[i]);
                }
                key_t k2 = tmp->key[(blockSize + 1) / 2];
                for (int i = (blockSize + 1) / 2 + 1; i <= blockSize; ++i) {
                    p->key.push_back(tmp->key[i]);
                    p->child.push_back(tmp->child[i]);
                }
                delete tmp;

                insert_in_parent(p, k2, p2);
            }

        }

        void erase_entry(Node *n, const key_t &Key, Node *delNode) {

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

//            if (N is the root and N has only one remaining child)
            if (n == root && n->child.size() == 1) {
                root = n->child[0];
                delete n;
                return;
            }
            else if (n == root) {
                return;
            }
            else if ((n->isLeaf && n->key.size() < blockSize / 2)
                     ||
                     (!n->isLeaf && n->child.size() < (blockSize + 1) / 2)) {
                Node *p = n->parent;
                Node *n2;
                key_t k2;
                //prev为true时表示n2是n的前一个节点
//                Let n2 be the previous or next child of parent(n)
//                Let k2 be the value between pointers n and n2 in parent(n)
                bool prev = false;
                int idx;
                for (idx = 0; idx < p->child.size(); ++idx) {
                    if (p->child[idx] == n)
                        break;
                }
                if (idx > 0) {
                    n2 = p->child[idx - 1];
                    k2 = p->key[idx - 1];
                    prev = true;
                } else {
                    n2 = p->child[idx + 1];
                    k2 = p->key[idx];
                }

//                if (entries in N and N′ can fit in a single node)
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
                    n2->next = n->next;
                    erase_entry(n->parent, k2, n);
                    delete n;
                }
                else {
                    /* Redistribution: borrow an entry from n2 */

//                    if (N′ is a predecessor of N)
                    if (prev) {
                        // n2是n的前一个节点

//                        if (N is a nonleaf node)
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
                                    p->key[i] = n2->key.front();
                                    break;
                                }
                            }

                            n2->key.erase(0);
                            n2->value.erase(0);
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

        bptree(const char * fname) {
            root = invalid_off;

            pfile = fopen(fname, "rb+");
            filename = new char[strlen(fname) + 1];
            strcpy(filename, fname);

            if (!pfile) {
                pfile = fopen(fname, "wb+");
            }
            else {
                read_info();
            }
        }

        ~bptree() {
            clear(root);

            if (pfile) {
                fclose(pfile);
            }

            delete filename;
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
            //now t is a leaf node
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
            Node *lf;
            if (root == NULL) {
                root = new Node();
                lf = root;
            }
            else {
                lf = find_leaf(Key);
            }

            //lf is a leaf Node
            if (lf->key.size() < blockSize - 1) {
                insert_in_leaf(lf, Key, Value);
            }
            else {
                /* Leaf has n − 1 key values already, split it */
                Node *lf2 = new Node();
                Node *tmp = new Node();

                for (int i = 0; i < lf->key.size(); ++i) {
                    tmp->key.push_back(lf->key[i]);
                    tmp->value.push_back(lf->value[i]);
                }
                insert_in_leaf(tmp, Key, Value);

                lf2->next = lf->next;
                lf->next = lf2;

//                Erase L.P1 through L.Kn−1 from L
//                Copy T.P1 through T.K⌈n/2⌉ from T into L starting at L.P1 Copy T.P⌈n/2⌉+1 through T.Kn from T into L′ starting at L′.P1 Let K′ be the smallest key-value in L′
//                insert in parent(L, K′, L′)
                lf->key.clear();
                lf->value.clear();

                for (int i = 0; i <= (blockSize + 1) / 2 - 1; ++i) {
                    lf->key.push_back(tmp->key[i]);
                    lf->value.push_back(tmp->value[i]);
                }

                for (int i = (blockSize + 1) / 2; i <= blockSize - 1; ++i) {
                    lf2->key.push_back(tmp->key[i]);
                    lf2->value.push_back(tmp->value[i]);
                }

                delete tmp;

                insert_in_parent(lf, lf2->key[0], lf2);
            }
        }

        void erase(const key_t &Key) {
            Node *lf = find_leaf(Key);
            erase_entry(lf, Key, NULL);
        }

        class iterator {
            friend class bptree;
        private:
            Node *ptr;
            int idx;

        public:
            iterator(Node *p = NULL, int i = 0) : ptr(p), idx(i) {}

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
            int idx;
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