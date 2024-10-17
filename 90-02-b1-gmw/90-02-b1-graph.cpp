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
#include "90-02-b1-head.h"
using namespace std;

static void updateScore(CONSOLE_GRAPHICS_INFO* const pCGI, int nows, int& tots)
{
	nows = nows * nows * 5;
	tots += nows;
	char s[30];
	sprintf(s, "本次得分：%d，总得分：%d", nows, tots);
	gmw_status_line(pCGI, TOP_STATUS_LINE, s);
}

#define UPDATE_LIGHT	1
#define UPDATE_DARK		0

// 格子坐标绘球
static void updateBall(CONSOLE_GRAPHICS_INFO* const pCGI, int x, int y, int map[][MAP_SIZE], int dl)
{
	if (x < 1 || y < 1 || x > pCGI->row_num || y > pCGI->col_num)
		return;
	if (dl == UPDATE_LIGHT)
		gmw_draw_block(pCGI, x - 1, y - 1, map[x][y], BDI_SELECTED);
	else 
		gmw_draw_block(pCGI, x - 1, y - 1, map[x][y], BDI_NORMAL);
}

static void dfsDisplay(CONSOLE_GRAPHICS_INFO* const pCGI, int x, int y, int map[][MAP_SIZE], int sta[][MAP_SIZE], int dl)
{
	int sta_being = (dl == UPDATE_LIGHT ? STA_NEED_DEL : STA_NORMAL);
	sta[x][y] = sta_being;
	updateBall(pCGI, x, y, map, dl);
	for (int i = 0; i < 4; i++) {
		int nextx = x + forwardx[i];
		int nexty = y + forwardy[i];
		if (nextx < 1 || nextx > pCGI->row_num || nexty < 1 || nexty > pCGI->col_num)
			continue;
		if (sta[nextx][nexty] == sta_being)
			continue;
		if (map[nextx][nexty] == map[x][y])
			dfsDisplay(pCGI, nextx, nexty, map, sta, dl);
	}
}

static void drawFrontBall(CONSOLE_GRAPHICS_INFO* const pCGI, int map[][MAP_SIZE], int sta[][MAP_SIZE])
{
	for (int i = 1; i <= pCGI->row_num; i++)
		for (int j = 1; j <= pCGI->col_num; j++)
			gmw_draw_block(pCGI, i - 1, j - 1, map[i][j], BDI_NORMAL);
}

// delBall(x, y, col) : 坐标(x, y)的颜色为col的球的删除动画
static void eliminateBall(CONSOLE_GRAPHICS_INFO* const pCGI, int x, int y, int col)
{
	gmw_draw_block(pCGI, x - 1, y - 1, BDI_VALUE_BLANK, BDI_NORMAL);
}

// slideDownBall(pCGI, x, y, col) : 二三参数描述被操作元素的坐标(x, y)
//									col 为该元素的颜色。函数效果是显示将这个位置的元素向下移动一格的动画
static void slideDownBall(CONSOLE_GRAPHICS_INFO* const pCGI, int x, int y, int col)
{
	if (col == 0)
		return;
	gmw_move_block(pCGI, x - 1, y - 1, col, 0, BDI_NORMAL, UP_TO_DOWN, 1);
}

static void slideLeftBall(CONSOLE_GRAPHICS_INFO* const pCGI, int x, int y, int col)
{
	gmw_move_block(pCGI, x - 1, y - 1, col, 0, BDI_NORMAL, RIGHT_TO_LEFT, 1);
}

static bool checkAbleToBe(CONSOLE_GRAPHICS_INFO* const pCGI, int x, int y, int sta[MAP_SIZE][MAP_SIZE])
{
	return x >= 0 && y >= 0 && x < pCGI->row_num && y < pCGI->col_num && sta[x + 1][y + 1] != STA_VOID;
}

/*
 * 当前坐标 chsx, chsy 有变化时的处理
 */
static void coordinateMoved(CONSOLE_GRAPHICS_INFO* const pCGI, int map[][MAP_SIZE], int sta[][MAP_SIZE], int chsx, int chsy, int& lstx, int& lsty, int& clicktimes)
{
	if (clicktimes == 1)
		dfsDisplay(pCGI, lstx + 1, lsty + 1, map, sta, UPDATE_DARK);
	updateBall(pCGI, lstx + 1, lsty + 1, map, UPDATE_DARK);
	updateBall(pCGI, chsx + 1, chsy + 1, map, UPDATE_LIGHT);
	clicktimes = 0;
	lstx = chsx;
	lsty = chsy;
}

// 找到双击选择的坐标
static int findMouseClick(CONSOLE_GRAPHICS_INFO* const pCGI, int map[][MAP_SIZE], int sta[][MAP_SIZE], int& chsx, int& chsy)
{
	int ret, mac, kc1, kc2, clicktimes = 0, lstx = chsx, lsty = chsy;
	while (1) {
		int mx, my;
		ret = gmw_read_keyboard_and_mouse(pCGI, mac, mx, my, kc1, kc2, 1);
		if (ret == CCT_KEYBOARD_EVENT) {
			if (kc1 == 'q' || kc1 == 'Q') // 直接退出
				return 1;
			if (chsx != lstx || chsy != lsty) // 处理有移动的情况
				coordinateMoved(pCGI, map, sta, chsx, chsy, lstx, lsty, clicktimes);
			if (kc1 == '\n' || kc1 == '\r') {
				if (clicktimes) {
					dfsDisplay(pCGI, chsx + 1, chsy + 1, map, sta, UPDATE_DARK);
					return 0;
				}
				clicktimes = 1;
				dfsDisplay(pCGI, chsx + 1, chsy + 1, map, sta, UPDATE_LIGHT);
			}
			else if (kc1 == 224) {
				static const int fy[] = { 0, 0, -1, 1 };
				static const int fx[] = { -1, 1, 0, 0 };
				chsx = chsx + fx[keyMapToForward(kc2)];
				chsy = chsy + fy[keyMapToForward(kc2)];
				if (checkAbleToBe(pCGI, chsx, chsy, sta))
					coordinateMoved(pCGI, map, sta, chsx, chsy, lstx, lsty, clicktimes);
				else {
					chsx = chsx - fx[keyMapToForward(kc2)];
					chsy = chsy - fy[keyMapToForward(kc2)];
				}
			}
		}
		else {
			chsx = mx, chsy = my;
			if (mac == MOUSE_ONLY_MOVED) // 只有鼠标移动的情形
				coordinateMoved(pCGI, map, sta, chsx, chsy, lstx, lsty, clicktimes);
			else if (mac == MOUSE_RIGHT_BUTTON_CLICK) // 直接退出
				return 1;
			else { // 点击
				if (chsx != lstx || chsy != lsty) // 处理有移动的情况
					coordinateMoved(pCGI, map, sta, chsx, chsy, lstx, lsty, clicktimes);
				if (clicktimes) {
					dfsDisplay(pCGI, chsx + 1, chsy + 1, map, sta, UPDATE_DARK);
					return 0;
				}
				clicktimes = 1;
				dfsDisplay(pCGI, chsx + 1, chsy + 1, map, sta, UPDATE_LIGHT);
			}
		}
	}
	return 0;
}

static void gaming(CONSOLE_GRAPHICS_INFO* const pCGI, int map[][MAP_SIZE], int sta[][MAP_SIZE])
{
	int nowScore = 0, totScore = 0;
	clearStatus(pCGI->row_num, pCGI->col_num, sta, STA_NORMAL);
	cct_cls();
	gmw_draw_frame(pCGI);
	drawFrontBall(pCGI, map, sta);
	while (isPossible(pCGI->row_num, pCGI->col_num, map, sta)) {
		int chsx = pCGI->row_num, chsy = 1;
		if (findMouseClick(pCGI, map, sta, chsx, chsy))
			return;
		nowScore = isErasable(pCGI->row_num, pCGI->col_num, chsx + 1, chsy + 1, map, sta);
		if (nowScore <= 0) {
			gmw_status_line(pCGI, LOWER_STATUS_LINE, NULL, "所选位置周围没有相同颜色块");
			Sleep(500);
			continue;
		}
		updateScore(pCGI, nowScore, totScore);
		deleteBall(pCGI, map, sta, eliminateBall);
		fallBall(pCGI, map, sta, slideDownBall);
		squeezeBall(pCGI, map, sta, slideLeftBall);
	}
	return;
}

static void initCGI(CONSOLE_GRAPHICS_INFO* const pCGI, int n, int m)
{
	gmw_init(pCGI, n, m, COLOR_WHITE, COLOR_BLACK);
	gmw_set_block_default_linetype(pCGI, 2);			// 设置线型
	gmw_set_block_border_switch(pCGI, 1);				// 有色块边框边框
	gmw_set_colno_switch(pCGI, 1);						// 有列标
	gmw_set_color(pCGI, COLOR_BLACK, COLOR_WHITE, 0);	// 主体区域颜色后黑前白
	gmw_set_delay(pCGI, DELAY_OF_BLOCK_MOVED, 12);		// 下落延时 12
	gmw_set_font(pCGI, "新宋体", 18);					// 设置字体
	gmw_set_ext_rowcol(pCGI, 1, 0, 4, 4);				// 设置附加区域
	gmw_set_frame_color(pCGI, COLOR_WHITE, COLOR_BLACK);// 游戏区颜色后白前黑
	gmw_set_frame_style(pCGI, 6, 3, 1);					// 色块大小 6*3，有分割线
	gmw_set_rowno_switch(pCGI, 1);						// 有行标
	gmw_set_frame_linetype(pCGI);						// 设置色块边框
	gmw_set_status_line_color(pCGI, TOP_STATUS_LINE);	// 有上状态栏
	gmw_set_status_line_color(pCGI, LOWER_STATUS_LINE); // 有下状态栏
}

void AnimeOption(int n, int m)
{
	int map[MAP_SIZE][MAP_SIZE] = { 0 }, sta[MAP_SIZE][MAP_SIZE] = { 0 };
	CONSOLE_GRAPHICS_INFO CGI;
	initCGI(&CGI, n, m);
	generate(n, m, map, COLOR_CATES);
	gaming(&CGI, map, sta);
	to_be_continued("本小题结束", &CGI);
}