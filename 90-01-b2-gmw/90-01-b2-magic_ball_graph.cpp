/*2351871 ������ �ƿ�*/
#define _CRT_SECURE_NO_WARNINGS

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
	waitLine(0, "��С�������������End����...", "�������", "End");
}

static void updateScore(int score, CONSOLE_GRAPHICS_INFO* const pCGI)
{
	static char str[50];
	sprintf(str, "��ǰ������%d\0", score);
	gmw_status_line(pCGI, TOP_STATUS_LINE, str);
}

static void updateBall(CONSOLE_GRAPHICS_INFO* const pCGI, int x, int y, int map[][MAP_SIZE], int sta[][MAP_SIZE])
{
	gmw_draw_block(pCGI, x - 1, y - 1, map[x][y] + (sta[x][y] == STA_CHOOSE) * 10, BDI);
	cct_setcolor();
}

static void drawOneBlock(CONSOLE_GRAPHICS_INFO* const pCGI, int x, int y, int col, int sta)
{
	if (sta == STA_NORMAL)
		gmw_draw_block(pCGI, x - 1, y - 1, col, BDI_NORMAL);
	if (sta == STA_NEED_DEL)
		gmw_draw_block(pCGI, x - 1, y - 1, col, BDI_SELECTED);
	if (sta == STA_SWAP)
		gmw_draw_block(pCGI, x - 1, y - 1, col, BDI_PROMPT);
}

static void drawFrontBall(CONSOLE_GRAPHICS_INFO* const pCGI, int map[][MAP_SIZE], int sta[][MAP_SIZE])
{
	for (int i = 1; i <= pCGI->row_num; i++) {
		for (int j = 1; j <= pCGI->col_num; j++) {
			drawOneBlock(pCGI, i, j, map[i][j], sta[i][j]);
		}
	}
}

static void eliminateBall(CONSOLE_GRAPHICS_INFO* const pCGI, int x, int y, int val)
{
	cct_setcolor(getcol(val), COLOR_BLACK);
	for (int i = 0; i < 6; i++) {
		gmw_draw_block(pCGI, x - 1, y - 1, val, BDI_EXPLODED);
		wait(MID_GAP);
		gmw_draw_block(pCGI, x - 1, y - 1, val, BDI_NORMAL);
		wait(MID_GAP);
	}
	gmw_draw_block(pCGI, x - 1, y - 1, BDI_VALUE_BLANK, BDI_NORMAL);
	cct_setcolor();
}

static void slideDownBall(CONSOLE_GRAPHICS_INFO* const pCGI, int x, int y, int val)
{
	gmw_move_block(pCGI, x - 1, y - 1, val, 0, BDI_NORMAL, UP_TO_DOWN, 1);
}

static int finishDrawing(CONSOLE_GRAPHICS_INFO* const pCGI, int map[][MAP_SIZE], int sta[][MAP_SIZE])
{
	int ret = 0;
	while (findBlock(pCGI->row_num, pCGI->col_num, map, sta)) {
		drawFrontBall(pCGI, map, sta);
		wait(400);
		deleteBall(pCGI, map, sta, eliminateBall);
		fallBall(pCGI, map, sta, slideDownBall);
		ret += fillVoidBall(pCGI, map, sta, BALL_CATEGORY_SIZE, slideDownBall);
		wait(400);
	}
	clearStatus(pCGI->row_num, pCGI->col_num, sta, STA_NORMAL);
	findAvailable(pCGI->row_num, pCGI->col_num, map, sta);
	drawFrontBall(pCGI, map, sta);
	return ret;
}

static bool trySwap(CONSOLE_GRAPHICS_INFO* const pCGI, int map[][MAP_SIZE], int sta[][MAP_SIZE], Pair tpos[])
{
	swap(map[tpos[1].x][tpos[1].y], map[tpos[2].x][tpos[2].y]);
	if (tryFindBlock(pCGI->row_num, pCGI->col_num, map))
		return 1;
	swap(map[tpos[1].x][tpos[1].y], map[tpos[2].x][tpos[2].y]);
	sta[tpos[1].x][tpos[1].y] = sta[tpos[2].x][tpos[2].y] = STA_SWAP;
	updateBall(pCGI, tpos[1].x, tpos[1].y, map, sta);
	updateBall(pCGI, tpos[2].x, tpos[2].y, map, sta);
	return 0;
}

static bool mouseLoop(CONSOLE_GRAPHICS_INFO* const pCGI, int map[][MAP_SIZE], int sta[][MAP_SIZE])
{
	int mx = -1, my = -1, mevent, keya, keyb;
	Pair tpos[3], * pt = tpos + 1, * p = tpos + 2;
	while (1) {
		gmw_read_keyboard_and_mouse(pCGI, mevent, mx, my, keya, keyb, 1);
		int x = mx + 1, y = my + 1;
		if (mevent == MOUSE_RIGHT_BUTTON_CLICK) {
			wait(150);
			cct_enable_mouse();
			return 1;
		}
		if (mevent != MOUSE_LEFT_BUTTON_CLICK)
			continue;
		if (sta[x][y] == STA_NORMAL) {
			gmw_status_line(pCGI, LOWER_STATUS_LINE, "����ѡ���λ��");
			if (p == pt) {
				++p;
				sta[p->x][p->y] = STA_SWAP;
				updateBall(pCGI, p->x, p->y, map, sta);
			}
			pt = tpos + 1;
			p = tpos + 2;
			wait(150);
			continue;
		}
		if (sta[x][y] == STA_CHOOSE) {
			sta[x][y] = STA_SWAP;
			updateBall(pCGI, x, y, map, sta);
			++p;
		}
		else {
			sta[x][y] = STA_CHOOSE;
			updateBall(pCGI, x, y, map, sta);
			if (p == pt) {
				if (!isNeighbor(*(p + 1), Pair{ x, y })) {
					++p;
					sta[p->x][p->y] = STA_SWAP;
					updateBall(pCGI, p->x, p->y, map, sta);
				}
			}
			p->x = x, p->y = y;
			if (--p < pt) {
				if (trySwap(pCGI, map, sta, tpos))
					return 0;
				p = tpos + 2;
				gmw_status_line(pCGI, LOWER_STATUS_LINE, "���ܽ���������λ��");
				wait(BIG_GAP);
				continue;
			}
			gmw_status_line(pCGI, LOWER_STATUS_LINE, "��λ�ñ�ѡ��");
			wait(BIG_GAP);
		}
	}
}


/*
 * ��ʼ�����
 */
static void initCGI(CONSOLE_GRAPHICS_INFO* const pCGI, int n ,int m) 
{
	gmw_init(pCGI, n, m, COLOR_WHITE, COLOR_BLACK);
	gmw_set_block_default_linetype(pCGI, 1);			// ��������
	gmw_set_block_border_switch(pCGI, 0);				// ��ɫ��߿�߿�
	gmw_set_colno_switch(pCGI, 0);						// ���б�
	gmw_set_color(pCGI, COLOR_BLACK, COLOR_WHITE, 0);	// ����������ɫ���ǰ��
	gmw_set_delay(pCGI, DELAY_OF_BLOCK_MOVED, 80);		// ������ʱ 50
	gmw_set_font(pCGI, "������", 32);					// ��������
	gmw_set_ext_rowcol(pCGI, 2, 2, 8, 8);				// ���ø�������
	gmw_set_frame_color(pCGI, COLOR_WHITE, COLOR_BLACK);// ��Ϸ����ɫ���ǰ��
	gmw_set_frame_style(pCGI, 2, 1, 0);					// ɫ���С 2*1���޷ָ���
	gmw_set_rowno_switch(pCGI, 0);						// ���б�
	gmw_set_status_line_color(pCGI, TOP_STATUS_LINE);	// ����״̬��
	gmw_set_status_line_color(pCGI, LOWER_STATUS_LINE); // ����״̬��
}

void mainWork(int n, int m)
{
	int map[MAP_SIZE][MAP_SIZE] = { 0 }, sta[MAP_SIZE][MAP_SIZE] = { 0 };
	int score = 0, flag = 0;
	CONSOLE_GRAPHICS_INFO CGI;
	generate(n, m, map, BALL_CATEGORY_SIZE);
	initCGI(&CGI, n, m);
	cct_cls();
	gmw_draw_frame(&CGI);
	updateScore(score, &CGI);
	while (1) {
		// ��״̬������
		if (flag)
			score += finishDrawing(&CGI, map, sta);
		else
			finishDrawing(&CGI, map, sta), flag = 1;
		updateScore(score, &CGI);
		if (!tryFindAvailable(n, m, map) || mouseLoop(&CGI, map, sta))
			break;
		clearStatus(n, m, sta, STA_NORMAL);
	}
	to_be_continued("��Ϸ����", &CGI);
}