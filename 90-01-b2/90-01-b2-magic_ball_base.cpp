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

void drawCanvas(int n, int m, int map[][MAP_SIZE], int sta[][MAP_SIZE], const char* s)
{
	cout << s << '\n';
	cout << "  |";
	for (int i = 1; i <= m; i++)
		cout << setw(3) << i;
	cout << "\n--+-";
	for (int i = 1; i <= m; i++)
		cout << "---";
	cout << '\n';
	for (int i = 1; i <= n; i++) {
		cout << char('A' + i - 1) << " |";
		for (int j = 1; j <= m; j++) {
			cout << "  ";
			cct_setcolor(COLOR_RED * !!sta[i][j]);
			cout << map[i][j];
			cct_setcolor();
		}
		cout << '\n';
	}
}

static bool showCanvasAndTag(int n, int m, int map[][MAP_SIZE], int sta[][MAP_SIZE])
{
	drawCanvas(n, m, map, sta, "初始数组");
	waitLine(0, "\n按回车键进行寻找初始可消除项的操作...\n", '\n');
	int ret = findBlock(n, m, map, sta);
	if (!ret)
		cout << "初始已无可消除项\n";
	else
		drawCanvas(n, m, map, sta, "初始可消除项（不同色标识）");
	return ret;
}

static void initClearBlockLoop(int n, int m, int map[][MAP_SIZE], int sta[][MAP_SIZE])
{
	while (showCanvasAndTag(n, m, map, sta)) {
		waitLine(0, "按回车键进行数组下落除0操作...", '\n');
		fallBall(n, m, map, sta);
		drawCanvas(n, m, map, sta, "\n下落除0后的数组:");
		waitLine(0, "按回车键进行新值填充", '\n');
		fillVoidBall(n, m, map, sta, BALL_CATEGORY_SIZE, slideDownBall, 0);
		drawCanvas(n, m, map, sta, "\n新值填充后的数组（不同色标识）");
		waitLine(0, "按回车键继续", '\n');
		clearStatus(n, m, sta, STA_NORMAL);
	}
}

void baseOptionInit(int n, int m)
{
	int map[MAP_SIZE][MAP_SIZE] = { 0 }, sta[MAP_SIZE][MAP_SIZE] = { 0 };
	generate(n, m, map, BALL_CATEGORY_SIZE);
	showCanvasAndTag(n, m, map, sta);
	waitLine(0, "本小题结束，请输入End继续...", "输入错误", "End");
}

void baseOptionDelete(int n, int m)
{
	int map[MAP_SIZE][MAP_SIZE] = { 0 }, sta[MAP_SIZE][MAP_SIZE] = { 0 };
	generate(n, m, map, BALL_CATEGORY_SIZE);
	initClearBlockLoop(n, m, map, sta);
	waitLine(0, "本小题结束，请输入End继续...", "输入错误", "End");
}

void baseOptionDeleteHint(int n, int m)
{
	int map[MAP_SIZE][MAP_SIZE] = { 0 }, sta[MAP_SIZE][MAP_SIZE] = { 0 };
	generate(n, m, map, BALL_CATEGORY_SIZE);
	initClearBlockLoop(n, m, map, sta);
	findAvailable(n, m, map, sta);
	drawCanvas(n, m, map, sta, "可选择的消除提示（不同色标识）:");
	waitLine(0, "本小题结束，请输入End继续...", "输入错误", "End");
}