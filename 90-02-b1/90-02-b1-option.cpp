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

void NonAnimeOption(int n, int m, int optChoose) 
{
	int map[MAP_SIZE][MAP_SIZE] = { 0 }, sta[MAP_SIZE][MAP_SIZE] = { 0 };
	generate(n, m, map, COLOR_CATES);
	baseDrawing(n, m, map, sta, optChoose);
	waitLine(400, "\n\n��С�������������End����...", "�����������������", "End", 1);
}

void AnimeOption(int n, int m, int optChoose)
{
	int map[MAP_SIZE][MAP_SIZE] = { 0 }, sta[MAP_SIZE][MAP_SIZE] = { 0 };
	generate(n, m, map, COLOR_CATES);
	StyleCSS style;
	style.setHead((const char**)SINGLE_HEAD);
	style.setTail((const char**)SINGLE_TAIL);
	style.setTran((const char**)SINGLE_TRAN);
	style.setLine(SINGLE_LINE);
	style.setVert(SINGLE_VERT);
	int a, b;
	cout << "here!!";
	Sleep(1000);
	drawBackground(n, m, 1, &a, &b, 3, 3, style, 10);
	exit(0);
	//graphDrawing(n, m, map, sta, optChoose);
	waitLine(400, "\n\n��С�������������End����...", "�����������������", "End", 1);
}