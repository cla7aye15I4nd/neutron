#ifndef STR_HPP
#define STR_HPP

#include <iostream>
#include <sstream>
#include <cstring>
#include "tools.hpp"

bool equals(int *a, int *b, int len) {
	for (int i = 0; i < len; ++i, ++a, ++b)
		if (*a != *b) return false;
	return true;
}

template <size_t len>
class str {
public:
	char ch[len + 1];
	int used = 0;
	str(){
		memset(ch, 0, sizeof(ch));
	}
	str(int k) {
		sprintf(ch, "%d", k);
	}
	str(double k) {
		sprintf(ch, "%.3f", k);
	}
	char& operator [] (int k) {
		return ch[k];
	}
	bool operator ==(const str &other) {
		return equals((int*) ch, (int*) other.ch, len >> 2);
	}
	bool operator !=(const str &other) {
		return !(*this == other);
	}
	bool operator <(const str &other) {
		for (int i = 0; i <= len; i++) {
            if (ch[i] < other.ch[i]) return true;
            if (ch[i] > other.ch[i]) return false;
        }
		return false;
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
	void clear() {
        memset(ch, 0, sizeof(ch));
	}
	void read() {
	    memset(ch, 0, sizeof ch);
		scanf("%s", ch);
	}
};

#endif // !STR_HPP
