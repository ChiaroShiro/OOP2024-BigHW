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
using namespace std;
 
void getpos(int i, int j, int* x, int* y, int showBorder, int cn, int cm)
{
	// y 是列数，x 是行数
	*x = 2 + (cm * 2 + 2 * showBorder) * (j - 1);
	*y = 2 + (cn + 1 * showBorder) * (i - 1);
}

void generate(int n, int m, int map[][MAP_SIZE], int cates)
{
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= m; j++)
			map[i][j] = rand() % cates + 1;
}

void clearStatus(int n, int m, int sta[][MAP_SIZE], int x)
{
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= m; j++)
			sta[i][j] = x;
}

void dfsFindBlock(int x, int y, int n, int m, const int map[][MAP_SIZE], int sta[][MAP_SIZE], void (*delBall)(int, int, int))
{
	static const int FX[] = { 0, 0, -1, 1 };
	static const int FY[] = { 1, -1, 0, 0 };
	sta[x][y] = STA_NOW_DEL;
	delBall(x, y, map[x][y]);
	for (int i = 0; i < 4; i++) {
		int nextx = x + FX[i];
		int nexty = y + FY[i];
		if (nextx >= 1 && nexty >= 1 && nextx <= n && nexty <= m)
			if (sta[nextx][nexty] == STA_NEED_DEL && map[nextx][nexty] == map[x][y])
				dfsFindBlock(nextx, nexty, n, m, map, sta, delBall);
	}
}

void fallBall(int n, int m, int map[][MAP_SIZE], int sta[][MAP_SIZE], bool showGraph, void (*delBall)(int, int, int), void (*slideDownBall)(int, int, int, int, int))
{
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			if (sta[i][j] == STA_NEED_DEL) {
				sta[i][j] = STA_NOW_DEL;
				if (showGraph)
					dfsFindBlock(i, j, n, m, map, sta, delBall);
			}
		}
	}
	for (int j = 1; j <= m; j++) {
		int downGap = 0, now = n;
		while (now) {
			if (sta[now][j] == STA_NOW_DEL) {
				++downGap;
				sta[now][j] = STA_VOID;
			}
			else {
				if (downGap) {
					for (int i = 0; showGraph && i < downGap; i++)
						slideDownBall(n, m, now + i, j, map[now][j]);
					sta[now + downGap][j] = sta[now][j];
					map[now + downGap][j] = map[now][j];
					sta[now][j] = STA_VOID;
				}
			}
			--now;
		}
	}
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= m; j++)
			if (sta[i][j] == STA_VOID)
				map[i][j] = 0;
}

static void newBall(int n, int m, int x, int y, int map[][MAP_SIZE], int sta[][MAP_SIZE], bool showGraph, int cates, void (*slideDownBall)(int, int, int, int, int))
{
	map[x][y] = rand() % cates + 1;
	sta[x][y] = STA_NEW;
	if (!showGraph)
		return;
	for (int i = 0; i < x; i++)
		slideDownBall(n, m, i, y, map[x][y]);
}

int fillVoidBall(int n, int m, int map[][MAP_SIZE], int sta[][MAP_SIZE], 
				 int cates, void (*slideDownBall)(int, int, int, int, int), bool showGraph)
{
	int ret = 0;
	for (int j = 1; j <= m; j++) {
		int now = 1;
		while (sta[now][j] == STA_VOID)
			++now, ++ret;
		--now;
		while (now) {
			newBall(n, m, now, j, map, sta, showGraph, cates, slideDownBall);
			--now;
		}
	}
	return ret;
}

/*
 * 画一实心行，m 为长度，kind 为行首元素的样式，showBorder 为 1 则有边框，否则无边框
 * 每个元素的长度由 elelen 决定，gap 为绘画每个元素后的暂停时长
 */
static void drawOneSolidLine(int m, int kind, bool showBorder, int elelen, int gap)
{
	cout << BHEAD[kind];
	wait(gap);
	for (int i = 1; i < m; i++) {
		for (int j = 0; j < elelen; j++)
			cout << BHENG;
		wait(gap);
		if (showBorder) {
			cout << BBODY[kind];
			wait(gap);
		}
	}
	for (int j = 0; j < elelen; j++)
		cout << BHENG;
	cout << BTAIL[kind] << '\n';
	wait(gap);
}


/*
 * 画一空心行，m 为长度，showBorder 为 1 则有边框，否则无边框
 * 每个元素的长度由 elelen 决定，gap 为绘画每个元素后的暂停时长
 */
static void drawOneHollowLine(int m, bool showBorder, int elelen, int gap)
{
	cout << BSHU;
	wait(gap);
	for (int i = 1; i < m; i++) {
		for (int j = 0; j < elelen; j++)
			cout << "  ";
		wait(gap);
		if (showBorder) {
			cout << BSHU;
			wait(gap);
		}
	}
	for (int j = 0; j < elelen; j++)
		cout << "  ";
	cout << BSHU << '\n';
	wait(gap);
}

/*
 * 绘制 n 行 m 列个元素的背景板
 * showBorder 为 1 则显示边界，否则不显示
 * totx 和 toty 会存储更改后的屏幕的长宽
 * coren 为每个元素的行数，corem 为每个元素的宽度（中文字符宽度）
 * gap 为绘制每个元素后的暂停时间
 */
void drawBackground(int n, int m, bool showBorder, int* totx, int* toty,
						   int coren, int corem, int gap)
{
	getpos(n, m, totx, toty, showBorder, coren, corem);
	*totx = max(*totx + 12, 40);
	*toty = max(*toty + 7, 14);
	cct_setconsoleborder(*totx, *toty);
	cct_gotoxy(0, 0);
	cout << "屏幕：" << *totx << "行" << *toty << "列\n";
	cct_setcolor(COLOR_WHITE, COLOR_BLACK);
	drawOneSolidLine(m, BUP, showBorder, corem, gap);
	for (int i = 1; i < n; i++) {
		for (int j = 0; j < coren; j++)
			drawOneHollowLine(m, showBorder, corem, gap);
		if (showBorder)
			drawOneSolidLine(m, BMID, showBorder, corem, gap);
	}
	for (int j = 0; j < coren; j++)
		drawOneHollowLine(m, showBorder, corem, gap);
	drawOneSolidLine(m, BDOWN, showBorder, corem, gap);
	cct_setcolor();
}

int oneDrawing(int n, int m, int y_size, int map[][MAP_SIZE], int sta[][MAP_SIZE], bool isGap, int cates, 
			   void (*slideDownBall)(int, int, int, int, int), 
			   void (*delBall)(int, int, int), 
			   void (*drawFrontBall)(int, int, int[][MAP_SIZE], int[][MAP_SIZE], bool, int),
			   void (*extraMoving)(int, int, int[][MAP_SIZE], int[][MAP_SIZE], int, void(*)(int, int, int), void (*)(int, int, int, int, int)))
{
	int ret = 0;
	drawFrontBall(n, m, map, sta, 1, 0);
	if (isGap)
		wait(400);
	else {
		cct_gotoxy(0, y_size - 3);
		waitLine(0, "按回车键进行消除和下落除0操作", '\n');
	}
	fallBall(n, m, map, sta, 1, delBall, slideDownBall);
	if (extraMoving != NULL)
		extraMoving(n, m, map, sta, 1, delBall, slideDownBall);
	if (!isGap) {
		cct_gotoxy(0, y_size - 3);
		waitLine(0, "按回车键进行新值填充", '\n');
	}
	ret += fillVoidBall(n, m, map, sta, cates, slideDownBall);
	if (isGap)
		wait(400);
	return ret;
}