#ifndef STR_HPP
#define STR_HPP

#include <iostream>
#include <sstream>

//TODO 读入也加入循环展开

//string with fixed length
template <size_t len>
class str {
public:
    char ch[len + 1];
    
    int used = 0;
    str(){
        ch[0] = '\0';
    }
    str(int k) {
        sprintf(ch, "%d", k);
    }
    str(double k) {
        sprintf(ch, "%.5f", k);
    }
    /*str(std::string &s) {
      for (int i = 0; i < s.length(); i++)
      ch[i] = s[i];
      ch[s.length()] = '\0';
      }*/
    char& operator [] (int k) {
        return ch[k];
    }
    /*operator const char* (){
      return ch;
      }*/
    bool operator ==(const str &other) {
        for (int i = 0; i < len; i++) {
            if (ch[i] == '\0' && other.ch[i] == '\0') return true;
            if (ch[i] != other.ch[i]) return false;
        }
    }
    bool operator !=(const str &other) {
        return !(*this == other);
    }
    bool operator <(const str &other) {
        for (int i = 0; i <= len; i++) {
            if (ch[i] == '\0' && other.ch[i] == '\0') return false;
            if (ch[i] == '\0') return true;
            if (other.ch[i] == '\0') return false;
            if (ch[i] < other.ch[i]) return true;
            if (ch[i] > other.ch[i]) return false;
        }
    }
    str& operator =(const std::string &s) {
        for (int i = 0; i < s.length(); i++)
            ch[i] = s[i];
        ch[s.length()] = '\0';
        return *this;
    }
    template <size_t Len>
    str& operator =(const str<Len> &other) {
        if (Len > len) throw int();
        for (int i = 0; i < Len; i++)
            ch[i] = other.ch[i];
        return *this;
    }
    template <size_t Len>
    void append(const str<Len> &other) {
        while (ch[used] != '\0') used++;
        ch[used++] = ' ';
        for (int i = 0; i < other.used; i++)
            ch[used + i] = other.ch[i];
        used += other.used;
    }
};

#endif // !STR_HPP
