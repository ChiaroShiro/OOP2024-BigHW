/* 2351871 郎若谷 计科 */
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
#include "../include/menu.h"
#include "90-02-b1-head.h"
using namespace std;

// dfs连通块，内部格子变成 NEED_DEL
static int dfsArea(int x, int y, int col, int n, int m, const int map[][MAP_SIZE], int sta[][MAP_SIZE])
{
	sta[x][y] = STA_NEED_DEL;
	int size = 1;
	for (int i = 0; i < 4; i++) {
		int nextx = x + forwardx[i];
		int nexty = y + forwardy[i];
		if (nextx < 1 || nextx > n || nexty < 1 || nexty > m)
			continue;
		if (sta[nextx][nexty] == STA_NEED_DEL) 
			continue;
		if (map[nextx][nexty] == col)
			size += dfsArea(nextx, nexty, col, n, m, map, sta);
	}
	return size;
}

int isErasable(int n, int m, int x, int y, const int map[][MAP_SIZE], int sta[][MAP_SIZE])
{
	if (sta[x][y] != STA_NORMAL)
		return -1;
	int area = dfsArea(x, y, map[x][y], n, m, map, sta);
	if (area == 1) {
		sta[x][y] = STA_NORMAL; // 如果不可行则不会修改 sta
		return 0;
	}
	return area;
}

void drawStatus(int n, int m, int sta[][MAP_SIZE], const char* s)
{
	shows(s);
	showln();
	shows("  |");
	for (int i = 1; i <= m; i++) {
		cout << setw(3);
		showi(i);
	}
	shows("\n--+-");
	for (int i = 1; i <= m; i++)
		shows("---");
	showln();
	for (int i = 1; i <= n; i++) {
		showc('A' + i - 1);
		shows(" |");
		for (int j = 1; j <= m; j++) {
			shows("  ");
			if (sta[i][j] == STA_NEED_DEL)
				shows("*");
			else
				shows("0");
		}
		showln();
	}
}

bool isPossible(int n, int m, const int map[][MAP_SIZE], const int sta[][MAP_SIZE])
{
	int tmp[MAP_SIZE][MAP_SIZE] = { 0 };
	for (int i = 1; i <= n; i++) 
		for (int j = 1; j <= m; j++) 
			tmp[i][j] = sta[i][j];
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			if (isErasable(n, m, i, j, map, tmp) > 0)
				return 1;
		}
	}
	return 0;
}

static bool checkColumnVoid(int x, int n, const int sta[][MAP_SIZE])
{
	for (int i = 1; i <= n; i++) {
		if (sta[i][x] != STA_VOID)
			return 0;
	}
	return 1;
}

// 处理空列
void squeezeBall(int n, int m, int map[][MAP_SIZE], int sta[][MAP_SIZE], bool showGraph, void (*slideLeftBall)(int, int, int, int, int))
{
	int slidecnt = 0;
	for (int j = 1; j <= m; j++) {
		if (checkColumnVoid(j, n, sta)) {
			++slidecnt;
			continue;
		}
		if (slidecnt == 0) 
			continue;
		for (int i = 1; i <= n; i++) {
			if (sta[i][j] == STA_VOID)
				continue;
			if(showGraph)
				for (int p = 0; p < slidecnt; p++)
					slideLeftBall(n, m, i, j - p, map[i][j]);
			sta[i][j - slidecnt] = sta[i][j];
			map[i][j - slidecnt] = map[i][j];
			sta[i][j] = STA_VOID;
			map[i][j] = 0;
		}
	}
}