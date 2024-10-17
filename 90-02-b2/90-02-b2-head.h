/* 2351871 郎若谷 计科 */

#pragma once


#include "../include/cmd_console_tools.h"
#include "../include/cmd_gmw_tools.h"

const BLOCK_DISPLAY_INFO BDI[] = {
		{BDI_VALUE_BLANK, -1, -1, NULL},  //0不显示，用空格填充即可
		{2, COLOR_HYELLOW, COLOR_BLACK, NULL},
		{4, COLOR_HRED, COLOR_BLACK, NULL},
		{8, COLOR_GREEN, COLOR_BLACK, NULL},
		{16, COLOR_RED, COLOR_BLACK, NULL},
		{32, COLOR_HGREEN, COLOR_BLACK, NULL},
		{64, COLOR_HYELLOW, COLOR_BLACK, NULL},
		{128, COLOR_HRED, COLOR_BLACK, NULL},
		{256, COLOR_HWHITE, COLOR_BLACK, NULL},
		{512, COLOR_HBLACK, COLOR_BLACK, NULL},
		{1024, COLOR_HPINK, COLOR_BLACK, NULL},
		{2048, COLOR_WHITE, COLOR_BLACK, NULL},
		{4096, COLOR_YELLOW, COLOR_BLACK, NULL},
		{8192, COLOR_PINK, COLOR_BLACK, NULL},
		{16384, COLOR_HBLUE, COLOR_BLACK, NULL},
		{32768, COLOR_HCYAN, COLOR_BLACK, NULL},
		{65536, COLOR_HGREEN, COLOR_BLACK, NULL},
		{131072, COLOR_HPINK, COLOR_BLACK, NULL},  //如果开心，还可以继续加
		{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为-999
};

const int NUM2K[] = { 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048 };

const int colbias[] = { 0, 0, -1, 1 };
const int colbegin[] = { 0, 0, 0, 1 };
const int rowbias[] = { -1, 1, 0, 0 };
const int rowbegin[] = { 0, 1, 0, 0 };

int calcRestSize(CONSOLE_GRAPHICS_INFO* const pCGI, int sta[MAP_SIZE][MAP_SIZE]);
bool checkFull(CONSOLE_GRAPHICS_INFO* const pCGI, int sta[MAP_SIZE][MAP_SIZE]);
int calcScore(CONSOLE_GRAPHICS_INFO* const pCGI, int map[MAP_SIZE][MAP_SIZE]);
bool checkFail(CONSOLE_GRAPHICS_INFO* const pCGI, int map[MAP_SIZE][MAP_SIZE], int sta[MAP_SIZE][MAP_SIZE]);
bool inBorder(CONSOLE_GRAPHICS_INFO* const pCGI, int x, int y);

void game2048(int n, int m, int v, int p);