#ifndef LCK2_NODE_HPP
#define LCK2_NODE_HPP

#include "myVector.hpp"
#include "constant.hpp"
using std::cout;
using std::endl;

namespace sjtu {
    template <class key_t, class value_t>
    struct node {
        char * info;
        vector <node *> child;
        vector <key_t> key;
        vector <value_t> value;

        bool isLeaf;

        node *next;

        node(bool is_leaf = true) {
            isLeaf = is_leaf;
            next = NULL;
        }

        ~node() {

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
}
#endif //LCK2_NODE_HPP
