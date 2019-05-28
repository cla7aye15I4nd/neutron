#ifndef STR_HPP
#define STR_HPP

#include <iostream>
#include <sstream>
#include <cstring>

//string with fixed length
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
	/*str(std::string &s) {
		for (int i = 0; i < s.length(); i++)
			ch[i] = s[i];
		ch[s.length()] = '\0';
	}*/
	char& operator [] (int k) {
		return ch[k];
	}
	bool operator ==(const str &other) {
		for (int i = 0; i < len; i++)
			if (ch[i] != other.ch[i]) return false;
		return true;
	}
	/*
	bool operator ==(const str &other) {
	    unsigned int *p = ch, *q = other.ch;
		for (int i = 0; i < len / 4; i++)
			if (p[i] != q[i]) return false;
		return true;
	}
	 * */
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
	    memset(ch, 0, sizeof(ch));
	    scanf("%s", ch);
	}
	/*template <size_t Len>
	void append(const str<Len> &other) {
		while (ch[used] != '\0') used++;
		ch[used++] = ' ';
		for (int i = 0; i < other.used; i++)
			ch[used + i] = other.ch[i];
		used += other.used;
	}*/
};
/*template <size_t len>
std::istream& operator >> (std::istream &is, str<len> &obj) {
	std::string s;
	is >> s;
	obj = s;
	return is;
}

//something to string
template <typename T>
std::string toString(T &x) {
	std::stringstream ss;
	std::string s;
	ss << x;
	ss >> s;
	return s;
}
*/

#endif // !STR_HPP
