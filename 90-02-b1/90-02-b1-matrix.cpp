/* 2351871 ¿…»Ùπ» º∆ø∆ */
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

static const int forwardx[] = { 0, 0, 1, -1 };
static const int forwardy[] = { 1, -1, 0, 0 };

static int dfsArea(int x, int y, int col, int n, int m, int map[][MAP_SIZE], int sta[][MAP_SIZE])
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

bool isErasable(int n, int m, int x, int y, int map[][MAP_SIZE], int sta[][MAP_SIZE])
{
	int area = dfsArea(x, y, map[x][y], n, m, map, sta);
	if (area == 1) {
		map[x][y] = STA_NORMAL;
		return 0;
	}
	return area;
}