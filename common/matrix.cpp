/* 2351871 郎若谷 计科 */
#include <iostream>
#include <iomanip>
#include <cstring>
#include <cmath>
#include <ctime>
#include <windows.h>
#include <conio.h>
#include "../include/cmd_console_tools.h"
#include "../include/cmd_gmw_tools.h"
#include "../include/io_tools.h"
#include "../include/matrix.h"
using namespace std;

StyleCSS::StyleCSS() {
	for (int i = 0; i < 3; i++) {
		this->head[i] = DOUBLE_HEAD[i];
		this->tran[i] = DOUBLE_TRAN[i];
		this->tail[i] = DOUBLE_TAIL[i];
	}
	line = DOUBLE_LINE;
	vert = DOUBLE_VERT;
	now = 0;
}

void StyleCSS::setHead(const char* const news[])
{
	for (int i = 0; i < 3; i++)
		this->head[i] = news[i];
}
void StyleCSS::setTail(const char* const news[])
{
	for (int i = 0; i < 3; i++)
		this->tail[i] = news[i];
}
void StyleCSS::setTran(const char* const news[])
{
	for (int i = 0; i < 3; i++)
		this->tran[i] = news[i];
}
void StyleCSS::setLine(const char* const news)
{
	this->line = news;
}
void StyleCSS::setVert(const char* const news)
{
	this->vert = news;
}
void StyleCSS::setOutput(int kind)
{
	this->now = kind;
}
const char* StyleCSS::getHead()
{
	return this->head[this->now];
}
const char* StyleCSS::getTail()
{
	return this->tail[this->now];
}
const char* StyleCSS::getTran()
{
	return this->tran[this->now];
}
const char* StyleCSS::getLine()
{
	return this->line;
}
const char* StyleCSS::getVert()
{
	return this->vert;
}

void rgetpos(int i, int j, int* x, int* y, int addx, int addy, int cn, int cm)
{
	*x = (j - 2) / (cm + addx) + 1;
	*y = (i - 2) / (2 * (cn + addy)) + 1;
}

void getpos(int i, int j, int* x, int* y, int addx, int addy, int cn, int cm)
{
	// y 是列数，x 是行数
	*x = 2 + (cm + addx) * (j - 1) * 2;
	*y = 2 + (cn + addy) * (i - 1);
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

void TransferStatus(int n, int m, int sta[][MAP_SIZE], int ori, int trans)
{
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= m; j++)
			if (sta[i][j] == ori)
				sta[i][j] = trans;
}

void dfsFindBlock(int x, int y, int n, int m, int showBorder, const int map[][MAP_SIZE], int sta[][MAP_SIZE], void (*delBall)(int, int, int, int))
{
	static const int FX[] = { 0, 0, -1, 1 };
	static const int FY[] = { 1, -1, 0, 0 };
	sta[x][y] = STA_NOW_DEL;
	delBall(x, y, map[x][y], showBorder);
	for (int i = 0; i < 4; i++) {
		int nextx = x + FX[i];
		int nexty = y + FY[i];
		if (nextx >= 1 && nexty >= 1 && nextx <= n && nexty <= m)
			if (sta[nextx][nexty] == STA_NEED_DEL && map[nextx][nexty] == map[x][y])
				dfsFindBlock(nextx, nexty, n, m, showBorder, map, sta, delBall);
	}
}

void deleteBall(int n, int m, int showBorder, int map[][MAP_SIZE], int sta[][MAP_SIZE], bool showGraph, void (*delBall)(int, int, int, int))
{
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			if (sta[i][j] == STA_NEED_DEL) {
				sta[i][j] = STA_NOW_DEL;
				if (showGraph)
					dfsFindBlock(i, j, n, m, showBorder, map, sta, delBall);
				map[i][j] = 0;
			}
		}
	}
}

void dfsFindBlock(int x, int y, CONSOLE_GRAPHICS_INFO* const pCGI, const int map[][MAP_SIZE], int sta[][MAP_SIZE], void (*delBall)(CONSOLE_GRAPHICS_INFO* const, int, int, int))
{
	static const int FX[] = { 0, 0, -1, 1 };
	static const int FY[] = { 1, -1, 0, 0 };
	sta[x][y] = STA_NOW_DEL;
	delBall(pCGI, x, y, map[x][y]);
	for (int i = 0; i < 4; i++) {
		int nextx = x + FX[i];
		int nexty = y + FY[i];
		if (nextx >= 1 && nexty >= 1 && nextx <= pCGI->row_num && nexty <= pCGI->col_num)
			if (sta[nextx][nexty] == STA_NEED_DEL && map[nextx][nexty] == map[x][y])
				dfsFindBlock(nextx, nexty, pCGI, map, sta, delBall);
	}
}

void deleteBall(CONSOLE_GRAPHICS_INFO* const pCGI, int map[][MAP_SIZE], int sta[][MAP_SIZE], void (*delBall)(CONSOLE_GRAPHICS_INFO* const, int, int, int))
{
	for (int i = 1; i <= pCGI->row_num; i++) {
		for (int j = 1; j <= pCGI->col_num; j++) {
			if (sta[i][j] == STA_NEED_DEL) {
				sta[i][j] = STA_NOW_DEL;
				dfsFindBlock(i, j, pCGI, map, sta, delBall);
				map[i][j] = 0;
			}
		}
	}
}

void fallBall(int n, int m, int map[][MAP_SIZE], int sta[][MAP_SIZE], bool showGraph, int showBorder, void (*slideDownBall)(int, int, int, int, int, int))
{
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
						slideDownBall(n, m, now + i, j, map[now][j], showBorder);
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

void fallBall(CONSOLE_GRAPHICS_INFO* const pCGI, int map[][MAP_SIZE], int sta[][MAP_SIZE], void (*slideDownBall)(CONSOLE_GRAPHICS_INFO* const, int, int, int))
{
	for (int j = 1; j <= pCGI->col_num; j++) {
		int downGap = 0, now = pCGI->row_num;
		while (now) {
			if (sta[now][j] == STA_NOW_DEL) {
				++downGap;
				sta[now][j] = STA_VOID;
			}
			else {
				if (downGap) {
					for (int i = 0; i < downGap; i++)
						slideDownBall(pCGI, now + i, j, map[now][j]);
					sta[now + downGap][j] = sta[now][j];
					map[now + downGap][j] = map[now][j];
					sta[now][j] = STA_VOID;
				}
			}
			--now;
		}
	}
	for (int i = 1; i <= pCGI->row_num; i++)
		for (int j = 1; j <= pCGI->col_num; j++)
			if (sta[i][j] == STA_VOID)
				map[i][j] = 0;
}

static void newBall(int n, int m, int x, int y, int map[][MAP_SIZE], int sta[][MAP_SIZE], bool showGraph, int showBorder, int cates, void (*slideDownBall)(int, int, int, int, int, int))
{
	map[x][y] = rand() % cates + 1;
	sta[x][y] = STA_NEW;
	if (!showGraph)
		return;
	for (int i = 0; i < x; i++)
		slideDownBall(n, m, i, y, map[x][y], showBorder);
}

int fillVoidBall(int n, int m, int map[][MAP_SIZE], int sta[][MAP_SIZE], 
				 int cates, void (*slideDownBall)(int, int, int, int, int, int), bool showGraph, int showBorder)
{
	int ret = 0;
	for (int j = 1; j <= m; j++) {
		int now = 1;
		while (sta[now][j] == STA_VOID)
			++now, ++ret;
		--now;
		while (now) {
			newBall(n, m, now, j, map, sta, showBorder, showGraph, cates, slideDownBall);
			--now;
		}
	}
	return ret;
}

static void newBall(CONSOLE_GRAPHICS_INFO* const pCGI, int x, int y, int map[][MAP_SIZE], int sta[][MAP_SIZE], int cates, void (*slideDownBall)(CONSOLE_GRAPHICS_INFO* const, int, int, int))
{
	map[x][y] = rand() % cates + 1;
	sta[x][y] = STA_NEW;
	for (int i = 0; i < x; i++)
		slideDownBall(pCGI, i, y, map[x][y]);
}

int fillVoidBall(CONSOLE_GRAPHICS_INFO* const pCGI, int map[][MAP_SIZE], int sta[][MAP_SIZE],
	int cates, void (*slideDownBall)(CONSOLE_GRAPHICS_INFO* const, int, int, int))
{
	int ret = 0;
	for (int j = 1; j <= pCGI->col_num; j++) {
		int now = 1;
		while (sta[now][j] == STA_VOID)
			++now, ++ret;
		--now;
		while (now) {
			newBall(pCGI, now, j, map, sta, cates, slideDownBall);
			--now;
		}
	}
	return ret;
}

/*
 * 画一实心行，m 为长度，kind 为行首元素的样式，showBorder 为 1 则有边框，否则无边框
 * 每个元素的长度由 elelen 决定，gap 为绘画每个元素后的暂停时长
 * 样式：
 */
static void drawOneSolidLine(int m, int kind, bool showBorder, int elelen, int gap, int times, StyleCSS style)
{
	style.setOutput(kind);
	shows(style.getHead());
	wait(gap);
	for (int i = 1; i < m; i++) {
		for (int j = 0; j < elelen; j++)
			shows(style.getLine());
		if (showBorder) {
			shows(style.getTran());
		}
		if (i % times == 0)
			wait(gap);
	}
	for (int j = 0; j < elelen; j++)
		shows(style.getLine());
	shows(style.getTail());
	showln();
	wait(gap);
}


/*
 * 画一空心行，m 为长度，showBorder 为 1 则有边框，否则无边框
 * 每个元素的长度由 elelen 决定，gap 为绘画每个元素后的暂停时长
 */
static void drawOneHollowLine(int m, bool showBorder, int elelen, int gap, int times, StyleCSS style)
{
	shows(style.getVert());
	wait(gap);
	for (int i = 1; i < m; i++) {
		for (int j = 0; j < elelen; j++)
			shows("  ");
		if (showBorder)
			shows(style.getVert());
		if (i % times == 0)
			wait(gap);
	}
	for (int j = 0; j < elelen; j++)
		shows("  ");
	shows(style.getVert());
	showln();
	wait(gap);
}

void drawBackground(int n, int m, bool showBorder, int showFrame, int* totx, int* toty,
						   int coren, int corem, StyleCSS style, int gap, int times)
{
	int addxy = showBorder;
	getpos(n, m, totx, toty, addxy, addxy, coren, corem);
	*totx = max(*totx + 12, 40) + !!showFrame * 2;
	*toty = max(*toty + 7, 14) + !!showFrame;
	cct_setconsoleborder(*totx, *toty);
	cct_gotoxy(0, 0);
	shows("屏幕：");
	showi(*totx);
	shows("行");
	showi(*toty);
	shows("列\n");
	if (showFrame) {
		for (int i = 0; i < m; i++) {
			int xx, yy;
			getpos(1, i + 1, &xx, &yy, addxy, addxy, coren, corem);
			yy -= 1;
			cct_gotoxy(xx + 2, yy);
			if ((showFrame & 0xFF) == '1' && i == 9) {
				cout << setw(corem + 1);
				showi(10);
			}
			else {
				cout << setw(corem + 1);
				showc((showFrame & 0xFF) + i);
			}
		}
		showln();
	}
	if (showFrame)
		shows("  ");
	cct_setcolor(COLOR_WHITE, COLOR_BLACK);
	drawOneSolidLine(m, BUP, showBorder, corem, gap, times, style);
	for (int i = 1; i < n; i++) {
		for (int j = 0; j < coren; j++) {
			if (showFrame) {
				cct_setcolor(COLOR_BLACK, COLOR_WHITE);
				if (j == coren / 2) {
					showc(((showFrame >> 8) & 0xFF) + i - 1);
					shows(" ");
				}
				else
					shows("  ");
				cct_setcolor(COLOR_WHITE, COLOR_BLACK);
			}
			drawOneHollowLine(m, showBorder, corem, gap, times, style);
		}
		if (showBorder) {
			if (showFrame) {
				cct_setcolor(COLOR_BLACK, COLOR_WHITE);
				shows("  ");
				cct_setcolor(COLOR_WHITE, COLOR_BLACK);
			}
			drawOneSolidLine(m, BMID, showBorder, corem, gap, times, style);
		}
	}
	for (int j = 0; j < coren; j++) {
		if (showFrame) {
			cct_setcolor(COLOR_BLACK, COLOR_WHITE);
			if (j == coren / 2) {
				showc(((showFrame >> 8) & 0xFF) + n - 1);
				shows(" ");
			}
			else
				shows("  ");
			cct_setcolor(COLOR_WHITE, COLOR_BLACK);
		}
		drawOneHollowLine(m, showBorder, corem, gap, times, style);
	}
	if (showFrame) {
		cct_setcolor(COLOR_BLACK, COLOR_WHITE);
		shows("  ");
		cct_setcolor(COLOR_WHITE, COLOR_BLACK);
	}
	drawOneSolidLine(m, BDOWN, showBorder, corem, gap, times, style);
	cct_setcolor();
}

void drawCanvas(int n, int m, const int map[][MAP_SIZE], const int sta[][MAP_SIZE], const char* s, int colorTag)
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
			if(colorTag == -1 || colorTag == sta[i][j])
				cct_setcolor(COLOR_RED * !!sta[i][j]);
			showi(map[i][j]);
			cct_setcolor();
		}
		showln();
	}
}