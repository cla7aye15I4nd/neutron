#ifndef BTREE_HPP
#define BTREE_HPP

#include <iostream>
#include <cstdio>
#include <cstring>

const int Order = 1000;

namespace sjtu {
    template <class Key, class Value>
    class BTree {
    private:
        struct node {
            Key key;
            int pos;    //索引

            node(const Key &k) : key(k) {

            }

            ~node() {

            }
        };

        struct BTNode {
            BTNode *father;
            BTNode *child[Order + 1];   //child_size <= Order

            node *data[Order];  //data_size <= Order - 1 允许数量大一个，方便上溢或下溢
            size_t nodeSize;   // the number of key, 同时也是data中元素的个数

            BTNode() {
                father = NULL;
                nodeSize = 0;
                for (int i = 0; i < Order - 1; ++i)
                    data[i] = NULL;
                for (int i = 0; i < Order; ++i)
                    child[i] = NULL;
            }

            ~BTNode() {
                for (int i = 0; i < Order - 1; ++i)
                    if (data[i])
                        delete data[i];
            }
        };

        BTNode *root;
        BTNode *hot;    //find()最后访问的非空的节点位置
        size_t _size;   // the total number of key

        void destory(BTNode *t) {
            if (t == NULL)
                return;

            for (int i = 0; i < Order; ++i) {
                destory(t->child[i]);
            }

            delete t;
            t = NULL;
        }

        //todo maybe wrong
        node *fprev(BTNode *t, int rank) const {
            Key key = t->data[rank]->value->first;

            if (t->child[rank]) {
                t = t->child[rank];
                while (t->child[t->nodeSize]) {
                    t = t->child[t->nodeSize];
                }
                return t->data[t->nodeSize - 1];
            }
            else if (rank > 0) {
                return t->data[rank - 1];
            }
            else {
                while (t->father) {
                    t = t->father;
                    int i;
                    for (i = t->nodeSize - 1; i >= 0; --i) {
                        if (cmp(t->data[i]->value->first, key))
                            break;
                    }
                    if (i >= 0)
                        return t->data[i];
                }
                return NULL;
            }
        }

        //todo maybe wrong
        node *fnext(BTNode *t, int rank) const {
            Key key = t->data[rank]->value->first;

            if (t->child[rank + 1]) {
                t = t->child[rank + 1];
                while (t->child[0]) {
                    t = t->child[0];
                }
                return t->data[0];
            }
            else if (rank < t->nodeSize - 1) {
                return t->data[rank + 1];
            }
            else {
                while (t->father) {
                    t = t->father;
                    int i;
                    for (i = 0; i < t->nodeSize; ++i) {
                        if (cmp(key, t->data[i]->value->first))
                            break;
                    }
                    if (i < t->nodeSize)
                        return t->data[i];
                }
                return NULL;
            }
        }

        //todo unsure
        //找出不大于key的关键码,返回秩r
        int array_find(BTNode *Bnd, const Key &kk) {
            int i;
            for (i = 0; i < Bnd->nodeSize; ++i) {
                //找到第一个关键码大于key的
                if (kk < Bnd->data[i]->key)
                    break;
            }
            return i - 1;
        }

        int array_insert(BTNode *Bnd, int r, const Key &kk) {
            for (int i = Bnd->nodeSize; i > r; --i)
                Bnd->data[i] = Bnd->data[i - 1];    //传址

            Bnd->data[r] = new node(kk);
            Bnd->nodeSize++;

            return r;
        }

        void array_remove(BTNode *Bnd, int r) {
            if (Bnd->data[r])
                delete Bnd->data[r];
            for (int i = r; i < Bnd->nodeSize - 1; ++i) {
                Bnd->data[i] = Bnd->data[i + 1];
            }
            Bnd->data[Bnd->nodeSize - 1] = NULL;

            Bnd->nodeSize--;
        }

        void child_insert(BTNode *Bnd, int r, BTNode *ch) {
            for (int i = Bnd->nodeSize; i > r; --i)
                Bnd->child[i] = Bnd->child[i - 1];

            Bnd->child[r] = ch;
        }

        void child_remove(BTNode *Bnd, int r) {
            for (int i = 0; i < Bnd->nodeSize; ++i)
                Bnd->child[i] = Bnd->child[i + 1];

            Bnd->child[Bnd->nodeSize] = NULL;
        }

        void solveOverflow(BTNode *t) {
            if (t->nodeSize <= Order - 1) {
                return;
            }

            int mid = Order / 2;
            BTNode *tmp = new BTNode();
            for (int i = 0; i < Order - mid -1; ++i) {
                tmp->child[i] = t->child[mid + 1 + i];
                t->child[mid + 1 + i] = NULL;

                array_insert(tmp, i, t->data[mid + 1]->key);
                array_remove(t, mid + 1);
            }

            tmp->child[Order - mid - 1] = t->child[Order];

            if (tmp->child[0]) {
                for (int i = 0; i < Order - mid; ++i) {
                    tmp->child[i]->father = tmp;
                }
            }

            BTNode *fa = t->father;
            if (fa == NULL) {
                root = fa = new BTNode();
                fa->child[0] = t;
                t->father = fa;
            }

            int r = 1 + array_find(t, t->data[0]->key);
            array_insert(fa, r, t->data[mid]->key);
            array_remove(t, mid);
            fa->child[r + 1] = tmp;
            tmp->father = fa;

            solveOverflow(fa);
        }

        void solveUnderflow(BTNode *t) {
            if ((Order + 1) / 2 <= t->nodeSize + 1) {
                return;
            }

            BTNode *fa = t->father;
            if (fa == NULL) {
                if (t->nodeSize == 0 && t->child[0]) {
                    root = t->child[0];
                    root->father = NULL;
                    t->child[0] = NULL;
                    delete t;
                }
                return;
            }

            int r = 0;
            while(fa->child[r] != t)    r++;

            //case 1: borrow from left sibling
            if (r > 0) {
                BTNode *ls = fa->child[r - 1];
                if ((Order + 1) / 2 < ls->nodeSize + 1) {
                    array_insert(t, 0, fa->data[r - 1]->key);
                    fa->data[r - 1]->key = ls->data[ls->nodeSize - 1]->key;
                    child_insert(t, 0, ls->child[ls->nodeSize]);
                    array_remove(ls, ls->nodeSize - 1);

                    if (t->child[0]) {
                        t->child[0]->father = t;
                    }
                    return;
                }
            }

            //case 2: borrow from right sibling
            if (fa->nodeSize > r) {
                BTNode *rs = fa->child[r + 1];
                if ((Order + 1) / 2 < rs->nodeSize + 1) {
                    array_insert(t, t->nodeSize, fa->data[r]->key);
                    fa->data[r]->key = rs->data[0]->key;
                    t->child[t->nodeSize] = rs->child[0];
                    child_remove(rs, 0);
                    array_remove(rs, 0);
                    if (t->child[t->nodeSize]) {
                        t->child[t->nodeSize]->father = fa;
                    }
                    return;
                }
            }

            //case 3: combination
            if (r > 0) {
                //combine with left sibling
                BTNode *ls = fa->child[r - 1];
                array_insert(ls, ls->nodeSize, fa->data[r - 1]);
                array_remove(fa, r - 1);
                child_remove(fa, r);

                ls->child[ls->nodeSize] = t->child[0];
                if (ls->child[ls->nodeSize]) {
                    ls->child[ls->nodeSize]->father = ls;
                }

                int rank = ls->nodeSize;
                while (t->nodeSize) {
                    array_insert(ls, rank, t->data[0]->key);
                    array_remove(t, 0);
                    ls->child[ls->nodeSize] = t->child[0];
                    if (ls->child[ls->nodeSize])
                        ls->child[ls->nodeSize]->father = ls;
                    child_remove(t, 0);
                }
                delete t;
            }
            else {
                //combine with right sibling
                BTNode *rs = fa->child[r + 1];
                array_insert(rs, 0, fa->data[r]->key);
                array_remove(fa, r);
                child_remove(fa, r);

                child_insert(rs, 0, t->child[t->nodeSize]);
                t->child[t->nodeSize] = NULL;
                if (rs->child[0]) {
                    rs->child[0]->father = rs;
                }

                int rank;
                while (t->nodeSize) {
                    array_insert(rs, 0, t->data[t->nodeSize - 1]);
                    child_insert(rs, 0, t->child[t->nodeSize]);
                    if (rs->child[0]) {
                        rs->child[0]->father = rs;
                    }
                    child_remove(t, t->nodeSize);
                    array_remove(t, t->nodeSize - 1);
                }
                delete t;
            }

            solveUnderflow(fa);
        }


    public:
        BTree() {
            _size = 0;
            root = NULL;
        }


        ~BTree() {
            clear();
        }

        void clear() {
            _size = 0;
            destory(root);
        }

        bool empty() const {
            return _size == 0;
        }

        size_t size() const {
            return _size;
        }

        //todo unsure
        //不确定return BTNode还是node
        BTNode *find(const Key &kk) {
            BTNode *t = root;
            hot = NULL;

            while (t) {
                int r = array_find(t, kk);
                if (r >= 0 && kk == t->data[r]->key) {
                    return t;
                }

                hot = t;
                t = t->child[r + 1];
            }
            return NULL;
        }

        bool insert(const Key &kk) {
            node *t = find(kk);
            if (t)  return false;

            int r = array_find(hot, kk);
            array_insert(hot, r + 1, kk);

            //todo unsure 是否需要创建一个空子树指针
            _size++;

            solveOverflow(hot);
            return true;
        }

        bool remove(const Key &kk) {
            BTNode *t = find(kk);
            if (!kk)    return false;

            int r = array_find(t, kk);
            if (t->child[0]) {
                BTNode *tmp = t->child[r + 1];
                while (tmp->child[0]) {
                    tmp = tmp->child[0];
                }
                t->data[r]->key = tmp->data[0]->key;
                t = tmp;
                r = 0;
            }
            array_remove(t, r);
            _size--;

            solveUnderflow(t);
            return true;
        }

    };
}

#endif