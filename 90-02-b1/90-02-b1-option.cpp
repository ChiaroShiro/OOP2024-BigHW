/* 2351871 郎若谷 计科 */
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

void NonAnimeOption(int n, int m, int optChoose) 
{
	int map[MAP_SIZE][MAP_SIZE] = { 0 }, sta[MAP_SIZE][MAP_SIZE] = { 0 };
	generate(n, m, map, COLOR_CATES);
	baseDrawing(n, m, map, sta, optChoose);
	waitLine(400, "\n\n本小题结束，请输入End继续...", "输入错误！请重新输入", "End", 1);
}

void AnimeOption(int n, int m, int optChoose)
{
	int map[MAP_SIZE][MAP_SIZE] = { 0 }, sta[MAP_SIZE][MAP_SIZE] = { 0 };
	int xborder = 0, yborder = 0;
	generate(n, m, map, COLOR_CATES);
	StyleCSS style;
	style.setHead(SINGLE_HEAD);
	style.setTail(SINGLE_TAIL);
	style.setTran(SINGLE_TRAN);
	style.setLine(SINGLE_LINE); 
	style.setVert(SINGLE_VERT);
	//style.setVert("||");
	gaming(n, m, map, sta, optChoose, style, xborder, yborder);
	cct_gotoxy(0, yborder - 4);
	cct_setcolor();
	waitLine(400, "\n\n本小题结束，请输入End继续...", "输入错误！请重新输入", "End", 1);
}