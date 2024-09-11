/*2351871 ¿…»Ùπ» º∆ø∆*/
#include <iostream>
#include <iomanip>
#include <cstring>
#include <cmath>
#include <ctime>
#include <windows.h>
#include <conio.h>
#include "../include/cmd_console_tools.h"
#include "../include/io_tools.h"
#include "../include/matrix.h"
#include "90-01-b2-magic_ball.h"
using namespace std;

bool findBlock(int n, int m, int map[][MAP_SIZE], int res[][MAP_SIZE])
{
	bool ret = 0;
	clearStatus(n, m, res, STA_NORMAL);
	for (int i = 1; i <= n; i++) {
		int lst = 0;
		for (int j = 1; j <= m; j++) {
			if (map[i][j] == map[i][lst]) {
				if (j - lst >= 2) {
					res[i][j] = res[i][j - 1] = res[i][j - 2] = STA_NEED_DEL;
					ret = 1;
				}
			}
			else
				lst = j;
		}
		if (m - lst >= 2) {
			res[i][m] = res[i][m - 1] = res[i][m - 2] = STA_NEED_DEL;
			ret = 1;
		}
	}
	for (int j = 1; j <= m; j++) {
		int lst = 0;
		for (int i = 1; i <= n; i++) {
			if (map[i][j] == map[lst][j]) {
				if (i - lst >= 2) {
					res[i][j] = res[i - 1][j] = res[i - 2][j] = STA_NEED_DEL;
					ret = 1;
				}
			}
			else
				lst = i;
		}
		if (n - lst >= 2) {
			res[n][j] = res[n - 1][j] = res[n - 2][j] = STA_NEED_DEL;
			ret = 1;
		}
	}
	return ret;
}

bool tryFindBlock(int n, int m, int map[][MAP_SIZE])
{
	int tmp[MAP_SIZE][MAP_SIZE] = { 0 };
	return findBlock(n, m, map, tmp);
}

bool findAvailable(int n, int m, int map[][MAP_SIZE], int sta[][MAP_SIZE])
{
	int ret = 0;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j < m; j++) {
			swap(map[i][j], map[i][j + 1]);
			if (tryFindBlock(n, m, map))
				sta[i][j] = sta[i][j + 1] = STA_SWAP, ret = 1;
			swap(map[i][j], map[i][j + 1]);
		}
	}
	for (int i = 1; i < n; i++) {
		for (int j = 1; j <= m; j++) {
			swap(map[i][j], map[i + 1][j]);
			if (tryFindBlock(n, m, map))
				sta[i][j] = sta[i + 1][j] = STA_SWAP, ret = 1;
			swap(map[i][j], map[i + 1][j]);
		}
	}
	return ret;
}

bool tryFindAvailable(int n, int m, int map[][MAP_SIZE])
{
	int tmp[MAP_SIZE][MAP_SIZE] = { 0 };
	return findAvailable(n, m, map, tmp);
}

