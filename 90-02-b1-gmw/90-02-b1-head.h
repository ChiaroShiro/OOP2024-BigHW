/* 2351871 郎若谷 计科 */

#pragma once

#define COLOR_CATES			5

const BLOCK_DISPLAY_INFO BDI_NORMAL[] = {
		{0, -1, -1, "  "},  //0不显示，用空格填充即可
		{1, COLOR_HBLUE, COLOR_BLACK, "★"},
		{2, COLOR_HGREEN, COLOR_BLACK, "★"},
		{3, COLOR_HCYAN, COLOR_BLACK, "★"},
		{4, COLOR_HRED, COLOR_BLACK, "★"},
		{5, COLOR_HPINK, COLOR_BLACK, "★"},
		{-999, -1, -1, NULL} //判断结束条件为-999
};
/* 定义1-5的数字用何种形式显示在界面上（当前选择项状态+选中后关联项状态） */
const BLOCK_DISPLAY_INFO BDI_SELECTED[] = {
	{BDI_VALUE_BLANK, -1, -1, "  "},  //空白
	{1, COLOR_HBLUE, COLOR_WHITE, "★"},
	{2, COLOR_HGREEN, COLOR_WHITE, "★"},
	{3, COLOR_HCYAN, COLOR_WHITE, "★"},
	{4, COLOR_HRED, COLOR_WHITE, "★"},
	{5, COLOR_HPINK, COLOR_WHITE, "★"},
	{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为-999
};

int isErasable(int n, int m, int x, int y, const int map[][MAP_SIZE], int sta[][MAP_SIZE]);
void drawStatus(int n, int m, int sta[][MAP_SIZE], const char* s);
bool isPossible(int n, int m, const int map[][MAP_SIZE], const int sta[][MAP_SIZE]);
void squeezeBall(CONSOLE_GRAPHICS_INFO* const pCGI, int map[][MAP_SIZE], int sta[][MAP_SIZE], void (*slideLeftBall)(CONSOLE_GRAPHICS_INFO* const pCGI, int, int, int));

void AnimeOption(int n, int m);