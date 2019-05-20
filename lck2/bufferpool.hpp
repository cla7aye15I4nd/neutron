#ifndef LCK2_BUFFERPOOL_HPP
#define LCK2_BUFFERPOOL_HPP

#include "bpNode.hpp"
#include "BTree.hpp"
#include "constant.hpp"
#include "myVector.hpp"

namespace sjtu {
    template <class key_t, class value_t>
    class BufferManager {
        template <class key_type, class value_type>
        friend class bptree;

    private:
        typedef bpNode<key_t, value_t> Node;

        char filename[50];

        FILE *fp;

        bool isOpened;

        Node pool;

    public:
        offset root_off;
        offset head_off;
        offset tail_off;
        offset append_off;

        offset trash_off;       // get head block of trash bin

        int theSize;
        int allocSize;

    private:
        bool exist() {
            fp = fopen(filename, "rb");
            if (fp) {
                fclose(fp);
                return true;
            }
            else {
                return false;
            }
        }

        void createFile() {
            fp = fopen(filename, "wb");
            root_off = head_off = tail_off = trash_off = invalid_off;
            append_off = 0;
            theSize = allocSize = 0;

            fwrite(&root_off, sizeof(int), 1, fp);
            fwrite(&head_off, sizeof(int), 1, fp);
            fwrite(&tail_off, sizeof(int), 1, fp);
            fwrite(&append_off, sizeof(int), 1, fp);
            fwrite(&trash_off, sizeof(int), 1, fp);
            fwrite(&theSize, sizeof(int), 1, fp);
            fwrite(&allocSize, sizeof(int), 1, fp);

            fclose(fp);
        }

        void init() {
            if(!exist()) {
                createFile();
                fp = fopen(filename, "rb");
            }
            else {
                fp = fopen(filename, "rb");
                fread(&root_off, sizeof(int), 1, fp);
                fread(&head_off, sizeof(int), 1, fp);
                fread(&tail_off, sizeof(int), 1, fp);
                fread(&append_off, sizeof(int), 1, fp);
                fread(&trash_off, sizeof(int), 1, fp);
                fread(&theSize, sizeof(int), 1, fp);
                fread(&allocSize, sizeof(int), 1, fp);
            }
        }



    public:
        BufferManager() {
            filename[0] = '\0';
            isOpened = false;
            head_off = tail_off = root_off = trash_off = invalid_off;
            append_off = -1;  //-1 represents closed file
            theSize = allocSize = -1;   //-1 represents closed file

        }

        ~BufferManager() {
            if (isOpened) {
                closeFile();
            }
        }

        void setFilename(const char *fname) {
            strcpy(filename, fname);
        }

        void clearFilename() {
            filename[0] = '\0';
        }

        void record() {
            fseek(fp, 0, SEEK_SET);
            fwrite(&root_off, sizeof(int), 1, fp);
            fwrite(&head_off, sizeof(int), 1, fp);
            fwrite(&tail_off, sizeof(int), 1, fp);
            fwrite(&append_off, sizeof(int), 1, fp);
            fwrite(&trash_off, sizeof(int), 1, fp);
            fwrite(&theSize, sizeof(int), 1, fp);
            fwrite(&allocSize, sizeof(int), 1, fp);
        }

        void openFile() {
            if (isOpened)
                return;
            else {
                init();
                isOpened = true;
                return;
            }
        }


        void closeFile() {
            if (!isOpened)
                return;
            else {
                record();
                fclose(fp);

                root_off = head_off = tail_off = invalid_off;
                append_off = -1;
                theSize = allocSize = -1;
                fp = nullptr;
                isOpened = false;

                return;
            }
        }

        bool is_opened() {
            return isOpened;
        }

        //将信息写入ret节点
        void get_node_by_offset(offset off, Node &ret) {
            ret.addr = off;

            fseek(fp, off, SEEK_SET);

            fread(&ret.isLeaf, sizeof(bool), 1, fp);

            size_t keySize, valueSize, childSize;
            fread(&keySize, sizeof(short), 1, fp);
            fread(&valueSize, sizeof(short), 1, fp);
            fread(&childSize, sizeof(short), 1, fp);

            ret.key.file_read(fp, keySize);
            ret.value.file_read(fp, valueSize);
            ret.child.file_read(fp, childSize);

            fread(&ret.next, sizeof(offset), 1, fp);
        }

        bool next_leaf(const Node &cur, Node &ret) {
            if (ret.next == invalid_off) {
                return false;
            }
            else {
                get_node_by_offset(cur.next, ret);
                return true;
            }
        }

        bool get_root(Node &ret) {
            if(root_off == invalid_off)
                return false;

            if(root_off == 0)
                get_node_by_offset(tree_byte, ret);
            else
                get_node_by_offset(root_off, ret);

            return true;
        }

        void append_node(Node &ret, bool is_leaf) {
            ret.clear();

            if(trash_off != invalid_off) {
                ret.addr = trash_off;
                get_node_by_offset(trash_off, pool);
                trash_off = pool.next;
            }
            else {
                if (append_off == 0) {
                    ret.addr = tree_byte;
                }
                else {
                    ret.addr = append_off;
                }

                append_off += blockSize;

                ++allocSize;
            }
            ret.isLeaf = is_leaf;
        }

        void write_node(Node &ret) {
            fseek(fp, ret.addr, SEEK_SET);

            fwrite(&ret.isLeaf, sizeof(bool), 1, fp);

            size_t tmp = ret.key.size();
            fwrite(&tmp, sizeof(size_t), 1, fp);
            tmp = ret.value.size();
            fwrite(&tmp, sizeof(size_t), 1, fp);
            tmp = ret.child.size();
            fwrite(&ret, sizeof(size_t), 1, fp);

            ret.key.file_write(fp);
            ret.value.file_write(fp);
            ret.child.file_write(fp);

            fwrite(&ret.next, sizeof(offset), 1, fp);
        }

        void traverse() {
            if(head_off == invalid_off) {
                printf("traverse empty tree\n");
                return;
            }

            Node cur;
            get_node_by_offset(head_off, cur);

            while(true) {
                for(int i = 0; i < cur.key.size(); ++i)
                    std::cout << cur.key[i] << ' ' << cur.value[i] << std::endl;

                if(cur.next == invalid_off)
                    break;
                get_node_by_offset(cur.next, cur);
            }
        }
    };
}

#endif //LCK2_BUFFERPOOL_HPP
