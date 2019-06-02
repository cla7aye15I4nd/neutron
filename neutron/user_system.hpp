#ifndef USER_SYSTEM_HPP
#define USER_SYSTEM_HPP

#include "unistd.h"

#include <cctype>
#include <cstdio>
#include <cstring>
#include <cassert>

#include <fcntl.h>
//#define PERFECT

#ifdef __linux__
#define putchar putchar_unlocked
#define getchar getchar_unlocked
#endif

#define sp() putchar(' ')
#define ln() putchar('\n')

#define BUF_COUNT 64
#define USER_INFO_SIZE 64
#define BLOCK_SIZE (BUF_COUNT * USER_INFO_SIZE)
#define PRIV_BLOCK_SIZE (125000)

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

const char* USER_DB = "user.db";

class UserSystem {
public:
	UserSystem() {
		fd = open(USER_DB, O_RDWR);
		if (fd == -1) {
			fclose(fopen(USER_DB, "a+"));
			memset(priviege, 0, sizeof priviege);
			priviege[0] = 1;
			fd = open(USER_DB, O_RDWR);
			write(fd, priviege, PRIV_BLOCK_SIZE);
		}
		else {
			read(fd, priviege, PRIV_BLOCK_SIZE);
		}

		dirty = 0; info = pool;
		userID = (lseek(fd, 0, SEEK_END) - PRIV_BLOCK_SIZE) / USER_INFO_SIZE + 2019;
	}

	void refresh() {
		lseek(fd, 0, SEEK_END);
		write(fd, pool, info - pool);
		info = pool;
	}

	~UserSystem() {
		lseek(fd, 0, SEEK_SET);

		for (int i = 0; dirty; i += BLOCK_SIZE, dirty >>= 1) {
			if (dirty & 1)
				write(fd, priviege + i, BLOCK_SIZE);
			else
				lseek(fd, BLOCK_SIZE, SEEK_CUR);
		}

		refresh();
		close(fd);
	}

	void append() {
		char *ptr = info;
		for (*ptr = getchar(); isspace(*ptr); *ptr = getchar());
		for (*++ptr = getchar(); *ptr != '\n'; *++ptr = getchar());
		*ptr = 0;

#ifdef PERFECT
		lseek(fd, 0, SEEK_END);
#endif

		info += USER_INFO_SIZE;
		if (info == pool + BLOCK_SIZE) {
			write(fd, pool, BLOCK_SIZE);
			info = pool;
		}
		printf("%d\n", userID++);
	}

	void modify_profile() {
		int id;
		scanf("%d", &id);

		char *ptr = info;
		for (*ptr = getchar(); isspace(*ptr); *ptr = getchar());
		for (*++ptr = getchar(); *ptr != '\n'; *++ptr = getchar());
		*ptr = 0;

		if (id < 2019 || id >= userID) puts("0");
		else {
			if (pool != info) refresh();
			lseek(fd, OFFSET(id), SEEK_SET);
			write(fd, info, USER_INFO_SIZE);
			puts("1");
		}
	}

	void login() {
		int id;

		scanf("%d%s", &id, passwd);

		if (id < 2019 || id >= userID) puts("0");
		else {
			if (pool != info) refresh();
			lseek(fd, OFFSET(id), SEEK_SET);
			read(fd, info, USER_INFO_SIZE);

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
		dirty |= 1 << (id >> 15);
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
		int id; scanf("%d", &id);
		if (id < 2019 || id >= userID) puts("0");
		else {
			if (pool != info) refresh();
			lseek(fd, OFFSET(id), SEEK_SET);
			read(fd, info, USER_INFO_SIZE);

			for (int i = 0, cnt = 0; i < USER_INFO_SIZE && info[i]; ++i) {
				cnt += info[i] == ' ';
				if (cnt != 1) putchar(info[i]);
			}

			printf(" %d\n", get_privilege(id) + 1);
		}
	}

	void clear() {
		close(fd);
		fclose(fopen(USER_DB, "wb+"));
		fd = open(USER_DB, O_RDWR);
		memset(priviege, 0, sizeof priviege);
		*priviege = 1;
		write(fd, priviege, PRIV_BLOCK_SIZE);
	}

private:
	int userID, fd;

	unsigned int dirty;
	char passwd[32];
	char pool[BLOCK_SIZE], *info;
	char priviege[PRIV_BLOCK_SIZE];
};

#endif