#ifndef LCK_CONSTANT_HPP
#define LCK_CONSTANT_HPP

namespace sjtu {
    typedef short Rank;
    typedef int offset;

    const int blockSize = 8;
    const offset invalid_off = -1;

    //root_off + append_off + trash_off = 12 bytes
    const offset bptree_byte = 12;
    //isLeaf + next + size = 1 + 4 + 2 = 7
    const offset node_byte = 7;
}
#endif //LCK_CONSTANT_HPP
