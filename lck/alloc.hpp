#ifndef LCK2_ALLOC_HPP
#define LCK2_ALLOC_HPP

#include "bptree.hpp"
#include "constant.hpp"
#include "myVector.hpp"

namespace sjtu {
    class file_alloc {
    private:
        int nodeSize = blockSize;

        struct Node {
            offset start, end;

            Node *prev, *next;

            Node(offset start = 0, offset end = 0, Node *prev = NULL, Node *next = NULL) {
                start = start;
                end = end;
                prev = prev;
                next = next;
            }

            inline int size() {
                return end - start;
            }

            inline void print() {
                printf("[block start:%d end:%d]]", start, end);
            }
        };

        Node *head, *tail;

        offset end_off;

        inline void delNode(Node *p) {
            if (p->prev)
                p->prev->next = p->next;
            else
                head = p->next;

            if (p->next)
                p->next->prev = p->prev;
            else
                tail = p->prev;

            delete p;
        }

        inline Node *insert_before(Node *p, offset start, offset end) {
            Node *q = new Node(start, end, p->prev, p);

            if (p->prev)
                p->prev->next = q;
            else
                head = q;

            return q;
        }

        inline Node *push_back(offset start, offset end) {
            if (empty()) {
                head = tail = new Node(start, end);
                return head;
            }

            Node *q = new Node(start, end, tail, NULL);
            tail->next = q;
            tail = q;

            return q;
        }

        inline void merge(Node *p) {
            Node *q;

            if (p->next && p->next->start == p->end) {
                q = p->next;
                p->end = q->end;

                delNode(q);
            }

            if (p->prev && p->prev->end == p->start) {
                q = p->prev;
                q->end = p->end;

                delNode(p);
            }
        }

    public:
        file_alloc(const char *filename = "") {
            head = tail = NULL;

            end_off = 0;
        }

        ~file_alloc() {
            clear();
        }

        void clear() {
            end_off = 0;
            Node *p = head, *q;
            while (p) {
                q = p->next;
                delete p;
                p = q;
            }

            head = tail = NULL;
        }

        inline bool empty() {
            return head = NULL;
        }

        offset alloc(int len) {
            Node *p = head;
            offset addr;

            while (p && p->size() < len) {
                p = p->next;
            }

            if (p) {
                addr = p->start;
                p->start += len;
                if (p->start == p->end)
                    delNode(p);

                return addr;
            }
            else {
                addr = end_off;
                end_off += len;

                return addr;
            }
        }

        void free(offset addr, offset len) {
            Node *p = head, *q;

            while (p && p->start < addr) {
                p = p->next;
            }

            if (p) {
                q = insert_before(p, addr, addr + len);
            }
            else {
                q = push_back(addr, addr + len);
            }

            merge(q);
        }

        void load(const char *filename = "") {
            clear();
            FILE *fp = fopen(filename, "rb");
            if (!fp)
                return;

            offset s[2];
            int read;
            fread(&end_off, sizeof(offset), 1, fp);

            while (!feof(fp)) {
                read = fread(s, sizeof(offset), 2, fp);
                if (read == 0)
                    break;

                push_back(s[0], s[1]);
            }

            fclose(fp);

        }

        void dump(const char *filename = "") {
            FILE *fp = fopen(filename, "wb");

            Node *p = head;

            offset s[2];
            fwrite(&end_off, sizeof(offset), 1, fp);

            while (p) {
                s[0] = p->start;
                s[1] = p->end;

                fwrite(s, sizeof(offset), 2, fp);

                p = p->next;
            }

            if (fp)
                fclose(fp);
        }


        void print() {
            Node *p = head;
            while (p) {
                p->print();
                puts("");
                p = p->next;
            }
        }

    };
}

#endif //LCK2_ALLOC_HPP
