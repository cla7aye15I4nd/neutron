#ifndef LCK_CONSTANT_HPP
#define LCK_CONSTANT_HPP

namespace sjtu {
    typedef short Rank;
    typedef int offset;

    const int blockSize = 8;
    const offset invalid_off = -1;

    const offset bptree_byte = 12;
    const offset node_byte = 15;
}
#endif //LCK_CONSTANT_HPP
