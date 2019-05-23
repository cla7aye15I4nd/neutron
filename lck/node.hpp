#ifndef LCK2_NODE_HPP
#define LCK2_NODE_HPP

#include "bptree.hpp"
#include "constant.hpp"
using std::cout;
using std::endl;

namespace sjtu {
    template <class key_t, class value_t>
    struct node {
        char * info;

        Rank keySize;  //number of key
        Rank childSize;

        bool isLeaf;

        offset addr;
        offset next;

        node(bool is_leaf = true) {
            isLeaf = is_leaf;
            addr = invalid_off;
            next = invalid_off;
            keySize = 0;
            childSize = 0;
        }

        ~node() {

        }

        void init() {
            isLeaf = true;
            addr = invalid_off;
            next = invalid_off;
            keySize = 0;
            childSize = 0;
        }

        key_t *keyInner(Rank n) {
            return (key_t *)(info + sizeof(offset) * (n + 1) + sizeof(key_t) * n);
        }

        key_t *keyLeaf(Rank n) {
            return (key_t *)(info + sizeof(value_t) * (n + 1) + sizeof(key_t) * n);
        }

        value_t *value(Rank n) {
            return (value_t *)(info + sizeof(value_t) * n + sizeof(key_t) * n);
        }

        offset *child(Rank n) {
            return (offset *)(info + sizeof(offset) * n + sizeof(key_t) * n);
        }

        void view() {
            cout << "Nodesize = " << keySize << endl;
            if (isLeaf) {
                for (Rank i = 0; i < keySize; ++i) {
                    cout << *keyLeaf(i) << ':' << *value(i);
                    puts("");
                }
            }
            else {
                for (Rank i = 0; i < keySize; ++i) {
                    cout << *keyInner(i) << ' ';
                }
                puts("");
            }
        }

        Rank search_leaf(const key_t &Key) {
            Rank idx;
            for (idx = 0; idx < keySize; ++idx) {
                if (*keyLeaf(idx) == Key)
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
                if (*keyInner(idx) == Key)
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
                if (*keyLeaf(idx) > Key)
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
                if (*keyInner(idx) > Key)
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
            if (idx < keySize) {
                for (Rank i = keySize; i > idx; --i)
                    *keyLeaf(i) = *keyLeaf(i - 1);
            }

            *keyLeaf(idx) = Key;
        }


        void insert_value(Rank idx, const value_t &Value) {
            if (idx < keySize) {
                for (Rank i = keySize; i > idx; --i)
                    *value(i) = *value(i - 1);
            }

            *value(idx) = Value;
        }

        void insert_key_inner(Rank idx, const key_t &Key) {
            if (idx < keySize) {
                for (Rank i = keySize; i > idx; --i)
                    *keyInner(i) = *keyInner(i - 1);
            }

            *keyInner(idx) = Key;
        }

        void insert_child(Rank idx, const offset &off) {
            if (idx < childSize) {
                for (Rank i = childSize; i > idx; --i)
                    *child(i) = *child(i - 1);
            }
            *child(idx) = off;
        }

        void erase_key_leaf(Rank idx) {
            for (Rank i = idx + 1; i < keySize; ++i)
                *keyLeaf(i - 1) = *keyLeaf(i);
        }

        void erase_value(Rank idx) {
            for (Rank i = idx + 1; i < keySize; ++i)
                *value(i - 1) = *value(i);
        }

        void erase_key_inner(Rank idx) {
            for (Rank i = idx + 1; i < keySize; ++i)
                *keyInner(i - 1) = *keyInner(i);
        }

        void erase_child(Rank idx) {
            for (Rank i = idx + 1; i < childSize; ++i)
                *keyLeaf(i - 1) = *keyLeaf(i);
        }

    };
}
#endif //LCK2_NODE_HPP
