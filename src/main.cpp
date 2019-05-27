#include "saleSystem.hpp"

#define DEBUG

#ifdef DEBUG
#endif
bool isLinux = true;
bool debug = true;
hashCity hashC;
hashTrain hashT;
bitManager bitMgr;
int bridgeN = 0;
ticketData bridge[100];

saleSystem sys;
int main() {
    double begin = clock();
    sys.processing();
    fprintf(stderr, "%f\n", (clock() - begin) / CLOCKS_PER_SEC);
    return 0;
}

/*
  TODO
  1. 不要使用bool debug 全局定义一个defs.h 使用#define DEBUG (*)
  2. 最后加上读入优化，bitset加上循环展开 (*)
  3. 尝试使用系统原生的方式读写文件 (*)
  4. str.hpp
     为什么str的==， <= 会需要多个if语句？ 宁可每次使用前都memset一次
     每个str的大小开成四的倍数
     已经记了used为什么后面还需要0作为结束符
     理论上不需要double
     str比较可以四位进行一次比较和赋值。 (*)
  5. hash.hpp
     用模板再精简一下 (*)
  6. 每次读入的那个while循环是在干什么。。。


*/
