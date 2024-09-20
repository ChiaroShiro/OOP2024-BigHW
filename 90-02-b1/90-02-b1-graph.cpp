/* 2351871 ������ �ƿ� */

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

#define __DEBUG 0

static void __debugPrint(const char* s, int x = 0, int y = 0)
{
	int orx, ory;
	cct_getxy(orx, ory);
	cct_gotoxy(x, y);
	std::cout << s << '\n';
	cct_gotoxy(orx, ory);
}

static void updateScore(int nows, int& tots)
{
	nows = nows * nows * 5;
	tots += nows;
	cct_gotoxy(0, 0);
	cct_setcolor();
	cout << "���ε÷�: " << nows << "���ܷ�: " << tots;
}

// �����������
static void drawBallAtXY(int posx, int posy, int bgcolor, int frcolor)
{
	cct_setcolor(bgcolor, frcolor);
	for (int p = 0; p < 3; p++) {
		cct_gotoxy(posx, posy + p);
		cout << GRAPH[p];
	}
	cct_setcolor();
}

// �����������
static void updateBall(int x, int y, int n, int m, int showBorder, int showFrame, int coren, int corem, int colbg, int colfr = COLOR_WHITE)
{
	if (x < 1 || y < 1 || x > n || y > m)
		return;
	int posx, posy;
	getpos(x, y, &posx, &posy, showBorder, showBorder, corem, corem);
	posx += 2 * showFrame;
	posy += 1 * showFrame;
	drawBallAtXY(posx, posy, colbg, colfr);
}

static void drawFrontBall(int n, int m, int map[][MAP_SIZE], int sta[][MAP_SIZE], int showBorder, int showFrame, int gap = 0, int times = 1)
{
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			updateBall(i, j, n, m, showBorder, !!showFrame, 3, 3, map[i][j], COLOR_BLACK);
			if (j % times == 0)
				wait(gap);
		}
	}
}

#define RET_QUIT_GAME			1
#define RET_CHOOSE_POSITION		2
#define RET_CHANGE_CELL			3

/*
* 
 * ����ֵ��	QUIT_GAME(1)		ѡ���˳���Ϸ
 *			CHOOOSE_POSITION(2)	ѡ���˵�ǰλ��
 *			CHANGE_CELL(3)		ָ���˱��λ��
 * 
 * ������	n, m, map, sta, showBorder ��
 *			chsx, chsy			��ָ��Ļ���ѡ��ĸ�������
 *			cor					ѡ���λ���Ƿ�Ϸ�
 */

static int getOneChange(int n, int m, int map[][MAP_SIZE], int sta[][MAP_SIZE], int showBorder, int& chsx, int& chsy, int &cor)
{
	int x, y, mevent, keya, keyb;
	while (1) {
		cct_enable_mouse();
		int korm = cct_read_keyboard_and_mouse(x, y, mevent, keya, keyb);
		if (korm == CCT_KEYBOARD_EVENT) { // ���̲���
			if (keya == 'Q' || keya == 'q')
				return RET_QUIT_GAME;
			if (keya == '\n' || keya == '\r')
				return RET_CHOOSE_POSITION;
			if (keya == 0xe0) { // ��ͷ����
				cor = 1;
				int nxtx = chsx, nxty = chsy;
				if (keyb == KB_ARROW_UP)
					nxtx = max(chsx - 1, 1);
				if (keyb == KB_ARROW_DOWN)
					nxtx = min(chsx + 1, n);
				if (keyb == KB_ARROW_LEFT)
					nxty = max(chsy - 1, 1);
				if (keyb == KB_ARROW_RIGHT)
					nxty = min(chsy + 1, m);
				if (sta[nxtx][nxty] == STA_NORMAL) {
					chsx = nxtx;
					chsy = nxty;
				}
				return RET_CHANGE_CELL;
			}
		}
		else {								// ������
			if (mevent == MOUSE_LEFT_BUTTON_CLICK || mevent == MOUSE_LEFT_BUTTON_DOUBLE_CLICK)
				return RET_CHOOSE_POSITION;
			if (mevent == MOUSE_RIGHT_BUTTON_CLICK)
				return RET_QUIT_GAME;
			int newx, newy, orix, oriy;
			rgetpos(x - 2, y - 1, &newx, &newy, showBorder, showBorder, 3, 3);
			getpos(newx, newy, &orix, &oriy, showBorder, showBorder, 3, 3);
			oriy += 1;
			orix += 2;
			if (x >= orix && y >= oriy && x < orix + 6 && y < oriy + 3) {
				if (chsx != newx || chsy != newy || !cor) { // �Ƶ��¸���
					cor = 1;
					chsx = newx;
					chsy = newy;
					return RET_CHANGE_CELL;
				}
			}
			else {
				if (chsx != newx || chsy != newy || cor) { // �Ƶ��±߽�
					cor = 0;
					chsx = newx;
					chsy = newy;
					return RET_CHANGE_CELL;
				}
			}
		}
	} // while true
}

static int dfsDisplay(int x, int y, int col, int n, int m, const int map[][MAP_SIZE], int sta[][MAP_SIZE], int showBorder, int showFrame, int frontColor)
{
	sta[x][y] = STA_NEED_DEL;
	int size = 1;
	updateBall(x, y, n, m, showBorder, showFrame, 3, 3, col, frontColor);
	for (int i = 0; i < 4; i++) {
		int nextx = x + forwardx[i];
		int nexty = y + forwardy[i];
		if (nextx < 1 || nextx > n || nexty < 1 || nexty > m)
			continue;
		if (sta[nextx][nexty] == STA_NEED_DEL)
			continue;
		if (map[nextx][nexty] == col)
			size += dfsDisplay(nextx, nexty, col, n, m, map, sta, showBorder, showFrame, frontColor);
	}
	return size;
}

// �ҵ�˫��ѡ�������
static int findMouseClick(int n, int m, int map[][MAP_SIZE], int sta[][MAP_SIZE], int showBorder, int& chsx, int& chsy, int opt)
{
	int tmp[MAP_SIZE][MAP_SIZE] = { 0 };
	int cor = 1, rechoose = 0, lstx = chsx, lsty = chsy;
	while (1) {
		cct_setcolor();
		gotoBottom(n, showBorder);
		if (chsx >= 1 && chsx <= n && chsy >= 1 && chsy <= m && cor)
			cout << "[��ǰ����]: " << char(chsx + 'A' - 1) << "��" << chsy << "��              ";
		else
			cout << "[��ǰ����]: λ�÷Ƿ�           ";
		if (cor) {
			updateBall(lstx, lsty, n, m, showBorder, 1, 3, 3, map[lstx][lsty], COLOR_BLACK);
			updateBall(chsx, chsy, n, m, showBorder, 1, 3, 3, map[chsx][chsy]);
		}
		if (chsx >= 1 && chsx <= n && chsy >= 1 && chsy <= m && cor) {
			lstx = chsx;
			lsty = chsy;
		}
		int ret = getOneChange(n, m, map, sta, showBorder, chsx, chsy, cor);
		if (sta[chsx][chsy] != STA_NORMAL) {
			cor = 0;
			continue;
		}
		if (ret == RET_QUIT_GAME)
			return 1;

		if (ret == RET_CHANGE_CELL || chsx < 1 || chsy < 1 || chsx > n || chsy > m) {
			if (rechoose == 1) {
				memset(tmp, 0, sizeof(tmp));
				dfsDisplay(lstx, lsty, map[lstx][lsty], n, m, map, tmp, showBorder, 1, COLOR_BLACK);
			}
			rechoose = 0;
		}
		if (++rechoose == 2) { // ��һ������˫��
			if (cor)
				break;
			rechoose = 0;
		}
		if (ret == RET_CHANGE_CELL || chsx < 1 || chsy < 1 || chsx > n || chsy > m || !cor) {
			if (rechoose == 1) {
				memset(tmp, 0, sizeof(tmp));
				dfsDisplay(lstx, lsty, map[lstx][lsty], n, m, map, tmp, showBorder, 1, COLOR_BLACK);
			}
			rechoose = 0;
		}
		if (rechoose == 1) { // ����Ѱ����ͨ��
			if (opt == 4 || opt == 5)	 // ���� D/E ����
				return 1;
			memset(tmp, 0, sizeof(tmp));
			dfsDisplay(chsx, chsy, map[chsx][chsy], n, m, map, tmp, showBorder, 1, COLOR_WHITE);
		}
	}
	updateBall(lstx, lsty, n, m, showBorder, 1, 3, 3, map[lstx][lsty], COLOR_BLACK);
	return 0;
}

// delBall(x, y, col) : ����(x, y)����ɫΪcol�����ɾ������
void eliminateBall(int x, int y, int col, int showBorder)
{
	cct_setcolor(COLOR_WHITE, COLOR_BLACK);
	int posx, posy;
	getpos(x, y, &posx, &posy, showBorder, showBorder, 3, 3);
	posx += 2;
	posy += 1;
	for (int i = 0; i < 3; i++) {
		cct_gotoxy(posx, posy + i);
		cout << "      ";
	}
	cct_setcolor();
}

// slideDownBall(n, m, x, y, col) : ǰ��������n, m���������С�����Ĳ�������������Ԫ�ص�����(x, y)
//									col Ϊ��Ԫ�ص���ɫ������Ч������ʾ�����λ�õ�Ԫ�������ƶ�һ��Ķ���
void slideDownBall(int n, int m, int x, int y, int col, int showBorder)
{
	if (col == 0)
		return;
	int posx, posy;
	getpos(x, y, &posx, &posy, showBorder, showBorder, 3, 3);
	posx += 2;
	posy += 1;
	for (int i = 1; i <= 3 + showBorder; i++) {
		Sleep(10);
		cct_setcolor(COLOR_WHITE, COLOR_BLACK);
		cct_gotoxy(posx, posy + i - 1);
		cout << "      ";
		drawBallAtXY(posx, posy + i, col, COLOR_BLACK);
	}
	if (showBorder) {
		cct_setcolor(COLOR_WHITE, COLOR_BLACK);
		cct_gotoxy(posx, posy + 3);
		cout << SINGLE_LINE << SINGLE_LINE << SINGLE_LINE;
	}
}

void slideLeftBall(int n, int m, int x, int y, int col)
{
	if (col == 0)
		return;
	int posx, posy;
	getpos(x, y, &posx, &posy, 1, 1, 3, 3);
	posx += 2;
	posy += 1;
	for (int i = 1; i <= 8; i++) {
		Sleep(3);
		cct_setcolor(COLOR_WHITE, COLOR_BLACK);
		for (int j = 0; j < 3; j++) {
			cct_gotoxy(posx - i + 6, posy + j);
			cout << ' ';
		}
		drawBallAtXY(posx - i, posy, col, COLOR_BLACK);
	}
	cct_setcolor(COLOR_WHITE, COLOR_BLACK); 
	for (int j = 0; j < 3; j++) {
		cct_gotoxy(posx - 2, posy + j);
		cout << SINGLE_VERT;
	}
}

void gaming(int n, int m, int map[][MAP_SIZE], int sta[][MAP_SIZE], int optChoose, StyleCSS style, int &xborder, int &yborder)
{  
	int optc = optChoose;
	optChoose %= 2;
	int nowScore = 0, totScore = 0;
	clearStatus(n, m, sta, STA_NORMAL);
	cct_cls();
	drawBackground(n, m, optChoose, ('A' << 8) + '1', &xborder, &yborder, 3, 3, style, 1, 2);
	drawFrontBall(n, m, map, sta, optChoose, 1, 1, 2);
	while (isPossible(n, m, map, sta)) {
		int chsx = n, chsy = 1;
		if (findMouseClick(n, m, map, sta, optChoose, chsx, chsy, optc))
			return;
		nowScore = isErasable(n, m, chsx, chsy, map, sta);
		if (nowScore <= 0) {
			showBottom(n, "��ѡλ����Χû����ͬ��ɫ��", optChoose, COLOR_BLACK, COLOR_HYELLOW);
			Sleep(500);
			continue;
		}
		updateScore(nowScore, totScore);
		deleteBall(n, m, optChoose, map, sta, 1, eliminateBall);
		if (optc == 6) { // For task F
			showBottom(n, "���س����������       ", optChoose, COLOR_BLACK, COLOR_HYELLOW);
			waitForEndlineOrClick();
		}
		fallBall(n, m, map, sta, 1, optChoose, slideDownBall);
		if (optc == 6) // ���� F ����
			return;
		squeezeBall(n, m, map, sta, 1, slideLeftBall);
	}
	return;
}