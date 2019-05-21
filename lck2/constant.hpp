#ifndef LCK_CONSTANT_HPP
#define LCK_CONSTANT_HPP

namespace sjtu {
    /**
     * in addition to content information, B+ tree has to store some configuration information.
     *
     * 1. (utility_offset) tree config: root_offset, head_offset, tail_offset, append_offset.
     *      in total 4+4+4+4 = 16 bytes.
     *      in addition, we add logicSize and allocSize, as well as trash_off.
     *      there are 8 bytes extra.(12 bytes)
     *
     * 2. (node_spare) node config: address, isLeaf, K_size, V_size, Ch_size,  next.
     *      in total 4+1+2+2+2+4+4 = 19 bytes.
     *      note: now father is gone, new node_utility_byte is 15. */
     //4+1+4+4+4+4
    typedef int offset;
    const offset invalid_off = -1;

    const offset tree_byte = 28;
    const offset node_byte = 21;

    const int blockSize = 8;
}
#endif //LCK_CONSTANT_HPP
