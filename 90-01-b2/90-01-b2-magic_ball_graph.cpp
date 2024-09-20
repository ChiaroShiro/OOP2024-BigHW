/*2351871 郎若谷 计科*/
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

static int getcol(int x)
{
	return x + (x >= 7) + (x >= 8) * 3 + (x == 9);
}

static bool isNeighbor(Pair a, Pair b)
{
	return (((a.x == b.x) + (a.y == b.y))
							== 1 &&
			((abs(a.x - b.x) == 1) + (abs(a.y - b.y) == 1))
							== 1);
}

static int getGraph(int x)
{
	if (x == STA_NORMAL)
		return CHOLLOW;
	if (x == STA_NEED_DEL)
		return CSOLID;
	if (x == STA_SWAP)
		return CDOUBLE;
	return CHOLLOW;
}

static void endHint(int y_size)
{
	cct_gotoxy(0, y_size - 3);
	waitLine(0, "本小题结束，请输入End继续...", "输入错误", "End");
}

static int checkClickable(int x, int y, int n, int m, int sta[][MAP_SIZE])
{
	int mapx, mapy;
	rgetpos(x, y, &mapx, &mapy, 1, 1, 1, 1);
	if (mapx < 1 || mapy < 1 || mapx > n || mapy > m)
		return CLICK_UNABLE;
	if ((y % 2) || (x % 4 < 2))
		return CLICK_UNABLE;
	return sta[mapx][mapy] == STA_NORMAL ? CLICK_SINGLE : CLICK_DOUBLE;
}

static void updateScore(int s)
{
	cct_gotoxy(20, 0);
	cct_setcolor();
	cout << "当前分数: " << s;
}

static void updateBall(int x, int y, int map[][MAP_SIZE], int sta[][MAP_SIZE])
{
	if (sta[x][y] == STA_CHOOSE)
		cct_setcolor(getcol(map[x][y]), COLOR_WHITE);
	else
		cct_setcolor(getcol(map[x][y]), COLOR_BLACK);
	getpos(x, y, &x, &y, 1, 1, 1, 1);
	cct_gotoxy(x, y);
	cout << CORE[CDOUBLE];
	cct_setcolor();
}

static void drawFrontBall(int n, int m, int map[][MAP_SIZE], int sta[][MAP_SIZE], bool showBorder, int gap = 0)
{
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			int x, y;
			getpos(i, j, &x, &y, showBorder, showBorder, 1, 1);
			cct_gotoxy(x, y);
			cct_setcolor(getcol(map[i][j]), COLOR_BLACK);
			cout << CORE[getGraph(sta[i][j])];
			cct_setcolor();
			wait(gap);
		}
	}
}

void eliminateBall(int x, int y, int val, int showBorder = 1)
{
	int posx, posy;
	getpos(x, y, &posx, &posy, 1, 1, 1, 1);
	cct_setcolor(getcol(val), COLOR_BLACK);
	for (int i = 0; i < 6; i++) {
		cct_gotoxy(posx, posy);
		cout << CORE[CFOUR];
		wait(MID_GAP);
		cct_gotoxy(posx, posy);
		cout << CORE[CHOLLOW];
		wait(MID_GAP);
	}
	cct_gotoxy(posx, posy);
	cct_setcolor(COLOR_WHITE, COLOR_BLACK);
	cout << CORE[CVOID];
	cct_setcolor();
}

void slideDownBall(int n, int m, int x, int y, int val, int showBorder = 1)
{
	int posx, posy;
	getpos(x, y, &posx, &posy, 1, 1, 1, 1);
	if (x != 0) {
		cct_gotoxy(posx, posy);
		cct_setcolor(COLOR_WHITE, COLOR_BLACK);
		cout << CORE[CVOID];
	}
	cct_gotoxy(posx, posy + 1);
	cct_setcolor(getcol(val), COLOR_BLACK);
	cout << CORE[CHOLLOW];
	wait(MID_GAP);
	cct_gotoxy(posx, posy + 1);
	cct_setcolor(COLOR_WHITE, COLOR_BLACK);
	cout << DOUBLE_LINE;
	cct_gotoxy(posx, posy + 2);
	cct_setcolor(getcol(val), COLOR_BLACK);
	cout << CORE[CHOLLOW];
	wait(MID_GAP);
	cct_setcolor();
}

int finishDrawing(int n, int m, int x_size, int y_size, int map[][MAP_SIZE], int sta[][MAP_SIZE],
	int coren, int corem, bool isGap)
{
	int ret = 0;
	while (findBlock(n, m, map, sta)) {
		ret += oneDrawing(n, m, y_size, 1, map, sta, isGap, BALL_CATEGORY_SIZE, slideDownBall, eliminateBall, drawFrontBall);
	}
	clearStatus(n, m, sta, STA_NORMAL);
	findAvailable(n, m, map, sta);
	drawFrontBall(n, m, map, sta, 1);
	return ret;
}

void graphOptionInitBorder(int n, int m, bool showBorder)
{
	int map[MAP_SIZE][MAP_SIZE] = { 0 }, sta[MAP_SIZE][MAP_SIZE] = { 0 };
	int x_size, y_size;
	generate(n, m, map, BALL_CATEGORY_SIZE);
	drawCanvas(n, m, map, sta, "初始数组:");
	waitLine(0, "按回车键显示图形", '\n');
	cct_cls();
	drawBackground(n, m, showBorder, 0, &x_size, &y_size, 1, 1, StyleCSS());
	drawFrontBall(n, m, map, sta, showBorder, SMALL_GAP);
	endHint(y_size);
}

void graphOptionDelete(int n, int m)
{
	int map[MAP_SIZE][MAP_SIZE] = { 0 }, sta[MAP_SIZE][MAP_SIZE] = { 0 };
	int x_size, y_size;
	generate(n, m, map, BALL_CATEGORY_SIZE);
	cct_cls();
	drawBackground(n, m, 0, 0, &x_size, &y_size, 1, 1, StyleCSS());
	findBlock(n, m, map, sta);
	drawFrontBall(n, m, map, sta, 0);
	endHint(y_size);
}

void graphOptionDeleteHint(int n, int m)
{
	int map[MAP_SIZE][MAP_SIZE] = { 0 }, sta[MAP_SIZE][MAP_SIZE] = { 0 };
	int x_size, y_size;
	generate(n, m, map, BALL_CATEGORY_SIZE);
	cct_cls();
	drawBackground(n, m, 1, 0, &x_size, &y_size, 1, 1, StyleCSS());
	finishDrawing(n, m, x_size, y_size, map, sta, 1, 1, 0);
	endHint(y_size);
}

static bool trySwap(int n, int m, int map[][MAP_SIZE], int sta[][MAP_SIZE], Pair tpos[])
{
	swap(map[tpos[1].x][tpos[1].y], map[tpos[2].x][tpos[2].y]);
	if (tryFindBlock(n, m, map))
		return 1;
	swap(map[tpos[1].x][tpos[1].y], map[tpos[2].x][tpos[2].y]);
	sta[tpos[1].x][tpos[1].y] = sta[tpos[2].x][tpos[2].y] = STA_SWAP;
	updateBall(tpos[1].x, tpos[1].y, map, sta);
	updateBall(tpos[2].x, tpos[2].y, map, sta);
	return 0;
}

static bool mouseLoop(int n, int m, int x_text, int y_text, int map[][MAP_SIZE], int sta[][MAP_SIZE], bool isLoop)
{
	int x, y, mevent, keya, keyb, tmpx, tmpy;
	Pair tpos[3], * pt = tpos + 1, * p = tpos + 2;
	while (1) {
		cct_enable_mouse();
		cct_read_keyboard_and_mouse(x, y, mevent, keya, keyb);
		if (mevent == MOUSE_RIGHT_BUTTON_CLICK) {
			wait(150);
			cct_enable_mouse();
			return 1;
		}
		int possta = checkClickable(x, y, n, m, sta);
		cct_gotoxy(x_text, y_text);
		if (possta == CLICK_UNABLE) {
			cout << "位置非法                      ";
			continue;
		}
		rgetpos(x, y, &x, &y, 1, 1, 1, 1);
		cout << char('A' + x - 1) << "行" << char(y + '0') << "列";
		cct_getxy(tmpx, tmpy);
		clearLine(tmpx, tmpy);
		if (mevent != MOUSE_LEFT_BUTTON_CLICK)
			continue;
		cct_gotoxy(tmpx, tmpy);
		if (possta == CLICK_SINGLE) {
			cout << ", 该位置不能被选择";
			wait(BIG_GAP);
			continue;
		}
		if (sta[x][y] == STA_CHOOSE) {
			sta[x][y] = STA_SWAP;
			updateBall(x, y, map, sta);
			++p;
		}
		else {
			sta[x][y] = STA_CHOOSE;
			updateBall(x, y, map, sta);
			if (!isLoop)
				return 1;
			if (p == pt) {
				if (!isNeighbor(*(p + 1), Pair{ x, y })) {
					++p;
					sta[p->x][p->y] = STA_SWAP;
					updateBall(p->x, p->y, map, sta);
				}
			}
			p->x = x, p->y = y;
			if (--p < pt) {
				if (trySwap(n, m, map, sta, tpos))
					return 0;
				p = tpos + 2;
				cct_gotoxy(x_text, y_text);
				cout << "不能交换这两个位置!";
				wait(BIG_GAP);
				continue;
			}
			cct_gotoxy(tmpx, tmpy);
			cout << ", 该位置被选择";
			wait(BIG_GAP);
		}
	}
}

void mainWork(int n, int m, bool isLoop)
{
	int map[MAP_SIZE][MAP_SIZE] = { 0 }, sta[MAP_SIZE][MAP_SIZE] = { 0 };
	int x_size, y_size, x_text, y_text, score = 0, flag = 0;
	generate(n, m, map, BALL_CATEGORY_SIZE);
	cct_cls();
	drawBackground(n, m, 1, 0, &x_size, &y_size, 1, 1, StyleCSS());
	updateScore(score);
	while (1) {
		cct_gotoxy(0, y_size - 5);
		cout << "[当前光标] ";
		cct_getxy(x_text, y_text);
		if (flag)
			score += finishDrawing(n, m, x_size, y_size, map, sta, 1, 1, 1);
		else
			finishDrawing(n, m, x_size, y_size, map, sta, 1, 1, 1), flag = 1;
		updateScore(score);
		if (!tryFindAvailable(n, m, map) || mouseLoop(n, m, x_text, y_text, map, sta, isLoop))
			break;
		clearStatus(n, m, sta, STA_NORMAL);
	}
	endHint(y_size);
}