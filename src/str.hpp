#ifndef STR_HPP
#define STR_HPP

#include <iostream>
#include <sstream>
#include <cstring>
#include "tools.hpp"

bool equals(int *a, int *b, int length) {
    for (int i = 0; i < length; ++i, ++a, ++b)
        if (*a != *b) return false;
    return true;
}

template <size_t length>
class str {
public:
    char ch[length+1];
    
    str(){ memset(ch, 0, sizeof(ch)); }
    
    char operator[] (int k) const{ return ch[k]; }
    
    bool operator== (const str &rhs) {
        return equals((int*) ch, (int*) rhs.ch, length >> 2);
    }
    bool operator!= (const str &rhs) {
        return !(*this == rhs);
    }
    bool operator <(const str &rhs) {
        for (size_t i = 0; i < length; ++i) {
            if (ch[i] < rhs[i]) return true;
            if (ch[i] > rhs[i]) return false;
        }
        return false;
    }
    
    str& operator =(const std::string &s) {
        for (size_t i = 0; i < s.length(); ++i)
            ch[i] = s[i];
        return *this;
    }
    
    void read() {
        memset(ch, 0, sizeof ch);
        scanf("%s", ch);
    }
};

#endif
