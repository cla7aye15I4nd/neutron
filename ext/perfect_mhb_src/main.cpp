#include "term.hpp"

term sys;
int main() {
    setvbuf(stdout, NULL, _IONBF, 0);
    sys.run();
    return 0;
}
