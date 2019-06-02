#ifndef USER_SYSTEM_HPP
#define USER_SYSTEM_HPP

#include <cctype>
#include <cstdio>
#include <cstring>
#include <sys/stat.h>

#ifdef __linux__
#define putchar putchar_unlocked
#define getchar getchar_unlocked
#endif

#define sp() putchar(' ')
#define ln() putchar('\n')

#define PRIV_BLOCK_SIZE (131072)
#define USER_INFO_SIZE 128
#define USER_DB "user.db"
#define OFFSET(ID) (PRIV_BLOCK_SIZE + (ID - 2019) * USER_INFO_SIZE)

template<class K> inline void read(K& x) {
	register char c = getchar();
	for (x = 0; !isdigit(c); c = getchar());
	for (; isdigit(c); c = getchar())
		x = x * 10 + c - '0';
}

template<class K> inline void write(K x) {
	static int fout[20], top;
	do fout[top++] = x % 10, x /= 10; while (x);
	while (top) putchar(fout[--top] + '0');
}

class UserSystem {
public:
	UserSystem() {
		file = fopen(USER_DB, "rb+");
		if (!file) {
			file = fopen(USER_DB, "wb+");
			memset(priviege, 0, sizeof priviege);
			xor_privilege(2019);
			fwrite(priviege, PRIV_BLOCK_SIZE, 1, file);
		}
		else {
			fread(priviege, PRIV_BLOCK_SIZE, 1, file);
		}

		fseek(file, 0, SEEK_END);
		userID = (ftell(file) - PRIV_BLOCK_SIZE) / USER_INFO_SIZE + 2019;
	}

	void write_back() {
		fseek(file, 0, SEEK_SET);
		fwrite(priviege, PRIV_BLOCK_SIZE, 1, file);
		fclose(file);
	}

	void append() {
		char *ptr = info;
		for (*ptr = getchar(); isspace(*ptr); *ptr = getchar());
		for (*++ptr = getchar(); *ptr != '\n'; *++ptr = getchar());
		*ptr = 0;

		fseek(file, 0, SEEK_END);
		fwrite(info, 1, USER_INFO_SIZE, file);
		printf("%d\n", userID++);
	}

	void modify_profile() {
		int id;
		read(id);

		char *ptr = info;
		for (*ptr = getchar(); isspace(*ptr); *ptr = getchar());
		for (*++ptr = getchar(); *ptr != '\n'; *++ptr = getchar());
		*ptr = 0;

		if (id < 2019 || id >= userID) puts("0");
		else {
			fseek(file, OFFSET(id), SEEK_SET);
			fwrite(info, ptr - info + 1, 1, file);
			puts("1");
		}
	}

	void login() {
		int id;

		scanf("%d%s", &id, passwd);

		if (id < 2019 || id >= userID) puts("0");
		else {
			fseek(file, OFFSET(id), SEEK_SET);
			fread(info, USER_INFO_SIZE, 1, file);

			for (int i = 0, cnt = 0; i < USER_INFO_SIZE; ++i) {
				if (cnt == 1) {
					int n = strlen(passwd);
					for (int j = 0; j < n; ++j, ++i)
						if (info[i] != passwd[j]) {
							puts("0");
							return;
						}
					break;
				}
				cnt += info[i] == ' ';
			}
			puts("1");
		}
	}

	bool get_privilege(int id) {
		id = id - 2019;
		return priviege[id >> 3] & (1 << (id & 7));
	}

	void xor_privilege(int id) {
		id = id - 2019;
		priviege[id >> 3] ^= (1 << (id & 7));
	}

	void modify_privilege() {
		int id1, id2, priv;
		scanf("%d%d%d", &id1, &id2, &priv);
		if (id1 < 2019 || id1 >= userID || id2 < 2019 || id2 >= userID || !get_privilege(id1) || priv == 0) puts("0");
		else {
			int priv2 = get_privilege(id2) + 1;
			if (priv2 > priv) puts("0");
			else {
				if (priv2 != priv)
					xor_privilege(id2);
				puts("1");
			}
		}
	}

	void query_profile() {
		int id; read(id);
		if (id < 2019 || id >= userID) puts("0");
		else {
			fseek(file, OFFSET(id), SEEK_SET);
			fread(info, USER_INFO_SIZE, 1, file);

			for (int i = 0, cnt = 0; i < USER_INFO_SIZE && info[i]; ++i) {
				cnt += info[i] == ' ';
				if (cnt != 1) putchar(info[i]);
			}

			printf(" %d\n", get_privilege(id) + 1);
		}
	}

	void clear() {
		fclose(file);
		file = fopen(USER_DB, "wb+");
		memset(priviege, 0, sizeof priviege);
		xor_privilege(2019);
		fwrite(priviege, PRIV_BLOCK_SIZE, 1, file);
	}

private:
	int userID;
	FILE* file;

	char passwd[25];
	char info[USER_INFO_SIZE];
	char priviege[PRIV_BLOCK_SIZE];
};

#endif