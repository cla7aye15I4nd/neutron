
#ifndef LCK2_BPNODE_HPP
#define LCK2_BPNODE_HPP

#include "constant.hpp"
#include "myVector.hpp"

namespace sjtu {
    template <class key_t, class value_t>
    struct bpNode {
        offset addr;

        vector<offset> child;
        vector<key_t> key;
        vector<value_t> value;

        offset next;

        bool isLeaf;

        bpNode(offset address = invalid_off, bool is_leaf = true) {
            addr = address;
            next = invalid_off;
            isLeaf = is_leaf;
        }

        bpNode(const bpNode &other) {
            addr = other.addr;
            next = other.next;
            isLeaf = other.isLeaf;

            key = other.key;

            if (isLeaf) {
                value = other.value;
            }
            else {
                child = other.child;
            }

        }

        ~bpNode() {

        }

        void clear() {
            addr = next = invalid_off;
            isLeaf = true;

            key.clear();
            value.clear();
            child.clear();
        }

        int search(const key_t &Key) {
            size_t idx;
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

        int search_upper(const key_t &Key) {
            int idx;
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

        int search_child(offset Child) {
            int idx;
            for (idx = 0; idx < child.size(); ++idx) {
                if (child[idx] == Child)
                    break;
            }

            if (idx == child.size()) {
                return -1;
            }
            else {
                return idx;
            }
        }

    };
}
#endif //LCK2_BPNODE_HPP
