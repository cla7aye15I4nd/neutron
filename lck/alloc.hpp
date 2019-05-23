#ifndef LCK2_ALLOC_HPP
#define LCK2_ALLOC_HPP

#include "bptree.hpp"
#include "constant.hpp"
#include "node.hpp"

namespace sjtu {
    template <class key_t, class value_t>
    class file_alloc {
        friend class bptree;
        typedef node<key_t, value_t> Node;

    private:
        char filename[50];

        FILE *file;

        bool open;
        Node trashNode;

    public:
        offset root_off;
        offset append_off;
        offset trash_off;

    private:
        void createFile() {
            file = fopen(filename, "wb+");

            root_off = trash_off = invalid_off;
            append_off = bptree_byte;

            fwrite(&root_off, sizeof(offset), 1, file);
            fwrite(&append_off, sizeof(offset), 1, file);
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
            fread(&append_off, sizeof(offset), 1, file);
            fread(&trash_off, sizeof(offset), 1, file);
        }

    public:
        file_alloc() {
            filename = "";
            open = false;
            file = NULL;
            root_off = append_off = trash_off = invalid_off;
        }

        ~file_alloc() {
            close_file();
        }

        void close_file() {
            if (open) {
                fseek(file, 0, SEEK_SET);
                fwrite(&root_off, sizeof(offset), 1, file);
                fwrite(&append_off, sizeof(offset), 1, file);
                fwrite(&trash_off, sizeof(offset), 1, file);

                fclose(file);
                file = NULL;
                open = false;
                root_off = append_off = trash_off = invalid_off;
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

        void get_block(offset off, Node &ret) {
            ret.addr = off;

            fseek(file, off, SEEK_SET);

            fread(&ret.isLeaf, sizeof(bool), 1, file);
            fread(&ret.next, sizeof(offset), 1, file);
            fread(&ret.size, sizeof(Rank), 1, file);

            if (ret.isLeaf) {
                fread(&ret.info, 1, sizeof(key_t) * ret.size + sizeof(value_t) * ret.size, file);
            }
            else {
                fread(&ret.info, 1, sizeof(key_t) * ret.size + sizeof(offset) * (ret.size + 1), file);
            }

        }

        bool get_next(const Node &cur, Node &ret) {
            if (cur.next == invalid_off) {
                return false;
            }
            else {
                get_block(cur.next, ret);
                return true;
            }
        }

        bool get_root(Node &ret) {
            if (root_off == invalid_off)
                return false;

            get_block(root_off, ret);
            return true;
        }

        void write_block(Node &cur) {
            fseek(file, cur.addr, SEEK_SET);

            fwrite(&cur.isLeaf, sizeof(bool), 1, file);
            fwrite(&cur.next, sizeof(offset), 1, file);
            fwrite(&cur.keySize, sizeof(Rank), 1, file);

            if (cur.isLeaf) {
                fwrite(&cur.info, 1, sizeof(key_t) * cur.keySize + sizeof(value_t) * cur.keySize, file);
            }
            else {
                fwrite(&cur.info, 1, sizeof(key_t) * cur.keySize + sizeof(offset) * (cur.keySize + 1), file);
            }
        }

        void append_block(Node &ret, bool leaf) {
            if (trash_off == invalid_off) {
                ret.addr = append_off;
                append_off += blockSize;
                ret.isLeaf = leaf;
            }
            else {
                ret.addr = trash_off;
                get_block(trash_off, trashNode);
                trash_off = trashNode.next;
                ret.isLeaf = leaf;
            }
        }


    };
}

#endif //LCK2_ALLOC_HPP
