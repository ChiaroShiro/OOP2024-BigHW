/* 2351871 郎若谷 计科 */

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
#include "../include/menu.h"
#include "90-02-b2-head.h"
using namespace std;

static int getrand(int p)
{ // 越小的数出现的概率越大，最大的上界数k出现的概率只有 2^{-k}
	int val = 1ll * rand() * rand() % (p - 1), mod = p - 1;
	int ret = 2;
	while (mod) {
		if (val <= mod / 2)
			return ret;
		mod /= 2;
		val -= mod + 1;
		ret *= 2;
	}
	return 2;
}

static void initCGI(CONSOLE_GRAPHICS_INFO* const pCGI, int n, int m, int v)
{
	gmw_init(pCGI, n, m, COLOR_WHITE, COLOR_BLACK);
	gmw_set_block_default_linetype(pCGI, 2);			// 设置线型
	gmw_set_block_border_switch(pCGI, 1);				// 有色块边框边框
	gmw_set_colno_switch(pCGI, 1);						// 有列标
	gmw_set_color(pCGI, COLOR_BLACK, COLOR_WHITE, 0);	// 主体区域颜色后黑前白
	gmw_set_delay(pCGI, DELAY_OF_BLOCK_MOVED, v);		// 下落延时
	gmw_set_font(pCGI, "新宋体", 16);					// 设置字体
	gmw_set_ext_rowcol(pCGI, 1, 1, 4, 4);				// 设置附加区域
	gmw_set_frame_color(pCGI, COLOR_WHITE, COLOR_BLACK);// 游戏区颜色后白前黑
	gmw_set_frame_style(pCGI, 10, 5, 0);				// 色块大小 6*3，无分割线
	gmw_set_rowno_switch(pCGI, 1);						// 有行标
	gmw_set_frame_linetype(pCGI);						// 设置色块边框
	gmw_set_status_line_color(pCGI, TOP_STATUS_LINE);	// 有上状态栏
	gmw_set_status_line_color(pCGI, LOWER_STATUS_LINE); // 有下状态栏
}

static void slideBall(CONSOLE_GRAPHICS_INFO* const pCGI, int x, int y, int col, int forward)
{
	if (col == 0)
		return;
	gmw_move_block(pCGI, x, y, col, 0, BDI, forward, 1);
}

static bool newBlock(CONSOLE_GRAPHICS_INFO* const pCGI, int map[MAP_SIZE][MAP_SIZE], int sta[MAP_SIZE][MAP_SIZE], int p)
{
	if (checkFail(pCGI, map, sta))
		return 0;
	if (checkFull(pCGI, sta))
		return 1;
	while (1) {
		int x = rand() % pCGI->row_num;
		int y = rand() % pCGI->col_num;
		if (sta[x][y] == STA_VOID) {
			sta[x][y] = STA_NORMAL;
			map[x][y] = getrand(p); // 33% 概率随机到 4;  67% 概率随机到 2
			return 1;
		}
	}
}

static void showFrontBall (CONSOLE_GRAPHICS_INFO* const pCGI, int map[MAP_SIZE][MAP_SIZE], int sta[MAP_SIZE][MAP_SIZE])
{
	for (int i = 0; i < pCGI->row_num; i++) {
		for (int j = 0; j < pCGI->col_num; j++) {
			gmw_draw_block(pCGI, i, j, map[i][j], BDI);
		}
	}
	
}

static bool showScore(CONSOLE_GRAPHICS_INFO* const pCGI, int map[MAP_SIZE][MAP_SIZE])
{
	char str[50];
	int score = calcScore(pCGI, map);
	sprintf(str, "当前最大值：%d", score);
	gmw_status_line(pCGI, TOP_STATUS_LINE, str);
	return score == 131072;
}

static int readForward(CONSOLE_GRAPHICS_INFO* const pCGI)
{
	int ret, mac, kc1, kc2, mrow, mcol;
	while (1) {
		ret = gmw_read_keyboard_and_mouse(pCGI, mac, mrow, mcol, kc1, kc2, 0);
		if (ret == CCT_KEYBOARD_EVENT && kc1 == 224) // 键盘输入方向键
			return keyMapToForward(kc2);
	}
}

/*
 * DOWN_TO_UP		0
 * UP_TO_DOWN		1
 * RIGHT_TO_LEFT	2
 * LEFT_TO_RIGHT	3
 */

static void mergeBall(CONSOLE_GRAPHICS_INFO* const pCGI, int map[MAP_SIZE][MAP_SIZE], int sta[MAP_SIZE][MAP_SIZE], int fr, int fc, int fwd, int num)
{
	slideBall(pCGI, fr, fc, num, fwd);
	map[fr][fc] = 0;
	sta[fr][fc] = STA_VOID;

	int nr = fr + rowbias[fwd];
	int nc = fc + colbias[fwd];
	map[nr][nc] *= 2;
	sta[nr][nc] = STA_CHOOSE;
	gmw_draw_block(pCGI, nr, nc, map[nr][nc], BDI);
}

static bool moveAllBlock(CONSOLE_GRAPHICS_INFO* const pCGI, int map[MAP_SIZE][MAP_SIZE], int sta[MAP_SIZE][MAP_SIZE], int fwd)
{
	bool ret = 0;
	int colbg = colbegin[fwd] ? pCGI->col_num - 1 : 0;
	int coled = !colbegin[fwd] ? pCGI->col_num - 1 : 0;
	int colbs = colbegin[fwd] ? -1 : 1;
	int rowbg = rowbegin[fwd] ? pCGI->row_num - 1 : 0;
	int rowed = !rowbegin[fwd] ? pCGI->row_num - 1 : 0;
	int rowbs = rowbegin[fwd] ? -1 : 1;
	for (int r = rowbg; r != rowed + rowbs; r += rowbs) {
		for (int c = colbg; c != coled + colbs; c += colbs) {
			if (sta[r][c] == STA_VOID)
				continue;
			int nowr = r, nowc = c;
			while (1) {
				nowr += rowbias[fwd];
				nowc += colbias[fwd];
				if (inBorder(pCGI, nowr, nowc) == 0)
					break;
				if (sta[nowr][nowc] == STA_VOID)
					slideBall(pCGI, nowr - rowbias[fwd], nowc - colbias[fwd], map[r][c], fwd);
				else
					break;
				ret = 1;
			}
			int lstr = nowr - rowbias[fwd];
			int lstc = nowc - colbias[fwd];
			if (r != lstr || c != lstc) {
				map[lstr][lstc] = map[r][c];
				sta[lstr][lstc] = STA_NORMAL;
				map[r][c] = 0;
				sta[r][c] = STA_VOID;
			}
			if (inBorder(pCGI, nowr, nowc) && sta[nowr][nowc] == STA_NORMAL && map[nowr][nowc] == map[lstr][lstc]) {
				mergeBall(pCGI, map, sta, lstr, lstc, fwd, map[lstr][lstc]);
				ret = 1;
			}
		}
	}
	TransferStatus(pCGI->row_num, pCGI->col_num, sta, STA_CHOOSE, STA_NORMAL, 0);
	return ret;
}

void game2048(int n, int m, int v, int p)
{
	CONSOLE_GRAPHICS_INFO CGI;
	initCGI(&CGI, n, m, v);
	int map[MAP_SIZE][MAP_SIZE] = { 0 }, sta[MAP_SIZE][MAP_SIZE] = { 0 };
	bool flag = 1;
	clearStatus(n, m, sta, STA_VOID, 0);
	gmw_draw_frame(&CGI);
	newBlock(&CGI, map, sta, p);
	while (checkFail(&CGI, map, sta) == 0) {
		if (flag)
			newBlock(&CGI, map, sta, p);
		showFrontBall(&CGI, map, sta);
		if (showScore(&CGI, map))
			break;
		flag = moveAllBlock(&CGI, map, sta, readForward(&CGI));
	}
	showScore(&CGI, map);
	to_be_continued("游戏结束！", &CGI);
}