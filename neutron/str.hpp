#ifndef STR_HPP
#define STR_HPP

#include <iostream>
#include <sstream>

//string with fixed length
template <size_t len>
class str {
	char ch[len + 1];
public:
	str() {
		ch[0] = '\0';
	}
	str(std::string &s) {
		for (int i = 0; i < s.length(); i++)
			ch[i] = s[i];
		ch[s.length()] = '\0';
	}
	bool operator ==(const str &other) {
		for (int i = 0; i < len; i++) {
			if (ch[i] == '\0' && other.ch[i] == '\0') return true;
			if (ch[i] != other.ch[i]) return false;
		}
		return true;
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
	operator std::string() const {
		return this->toString();
	}
	std::string toString() const {
		std::stringstream ss;
		std::string s;
		ss << ch;
		ss >> s;
		return s;
	}
};
template <size_t len>
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


#endif // !STR_HPP
