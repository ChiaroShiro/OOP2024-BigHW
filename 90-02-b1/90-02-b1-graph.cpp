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

static void updateBall(int x, int y, int showBorder, int showFrame, int coren, int corem, int colbg, int colfr = COLOR_WHITE)
{
	int posx, posy;
	getpos(x, y, &posx, &posy, showBorder, showBorder, corem, corem);
	posx += 2 * showFrame;
	posy += 1 * showFrame;
	cct_setcolor(colbg, colfr);
	for (int p = 0; p < 3; p++) {
		cct_gotoxy(posx, posy + p);
		cout << GRAPH[p];
	}
	cct_setcolor();
}

static void drawFrontBall(int n, int m, int map[][MAP_SIZE], int sta[][MAP_SIZE], int showBorder, int showFrame, int gap = 0)
{
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			updateBall(i, j, showBorder, !!showFrame, 3, 3, map[i][j], COLOR_BLACK);
			wait(gap);
		}
	}
}

#define RET_QUIT_GAME			1
#define RET_CHOOSE_POSITION		2
#define RET_CHANGE_CELL			3


static int getOneChange(int n, int m, int map[][MAP_SIZE], int sta[][MAP_SIZE], int showBorder, int& chsx, int& chsy, int &cor)
{
	int x, y, mevent, keya, keyb;
	while (1) {
		cct_enable_mouse();
		int korm = cct_read_keyboard_and_mouse(x, y, mevent, keya, keyb);
		if (korm == CCT_KEYBOARD_EVENT) {
			if (keya == 'Q' || keya == 'q')
				return RET_QUIT_GAME;
			if (keya == '\n' || keya == '\r')
				return RET_CHOOSE_POSITION;
			if (keya == 0xe0) {
				cor = 1;
				if (keyb == KB_ARROW_UP)
					chsx = max(chsx - 1, 1);
				if (keyb == KB_ARROW_DOWN)
					chsx = min(chsx + 1, n);
				if (keyb == KB_ARROW_LEFT)
					chsy = max(chsy - 1, 1);
				if (keyb == KB_ARROW_RIGHT)
					chsy = min(chsy + 1, m);
				return RET_CHANGE_CELL;
			}
		}
		else {
			if (mevent == MOUSE_LEFT_BUTTON_CLICK || mevent == MOUSE_LEFT_BUTTON_DOUBLE_CLICK)
				return RET_CHOOSE_POSITION;
			int newx, newy, orix, oriy;
			rgetpos(x, y, &newx, &newy, showBorder, showBorder, 3, 3);
			getpos(newx, newy, &orix, &oriy, showBorder, showBorder, 3, 3);
			if (x >= orix && y >= oriy && x < orix + 3 && y < oriy + 3) {
				if (chsx != newx || chsy != newy || !cor) {
					cor = 1;
					chsx = newx;
					chsy = newy;
					return RET_CHANGE_CELL;
				}
			}
			else {
				if (chsx != newx || chsy != newy || cor) {
					cor = 0;
					chsx = newx;
					chsy = newy;
					return RET_CHANGE_CELL;
				}
			}
		}
	}
}

void eliminateBall(int x, int y, int col)
{

}

void slideDownBall(int n, int m, int x, int y, int col)
{

}

void slideLeftBall(int n, int m, int x, int y, int col)
{

}

bool gaming(int n, int m, int map[][MAP_SIZE], int sta[][MAP_SIZE], int optChoose, StyleCSS style)
{  
	optChoose %= 2;
	int nowScore = 0, totScore = 0, xborder, yborder;
	clearStatus(n, m, sta, STA_NORMAL);
	cct_cls();
	drawBackground(n, m, optChoose, ('A' << 8) + '1', &xborder, &yborder, 3, 3, style, 1);
	drawFrontBall(n, m, map, sta, optChoose, 1, 1);
	while (isPossible(n, m, map, sta)) {
		int chsx = 1, chsy = 1, cor = 1, rechoose = 0;
		while (1) {
			if (cor)
				updateBall(chsx, chsy, 1, 1, 3, 3, map[chsx][chsy]);
			int ret = getOneChange(n, m, map, sta, optChoose, chsx, chsy, cor);
			if (ret == RET_QUIT_GAME)
				return 1;
			if (++rechoose == 2) {
				if(cor)
					break;
				rechoose = 0;
			}
			if (ret == RET_CHANGE_CELL)
				rechoose = 0;
		}
		nowScore = isErasable(n, m, chsx, chsy, map, sta);
		if (nowScore <= 0) {
			continue;
		}
		deleteBall(n, m, map, sta, 1, eliminateBall);
		fallBall(n, m, map, sta, 1, slideDownBall);
		squeezeBall(n, m, map, sta, 1, slideLeftBall);
	}
	return 0;
}