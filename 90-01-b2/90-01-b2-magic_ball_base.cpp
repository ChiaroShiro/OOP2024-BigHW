/*2351871 ������ �ƿ�*/
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
	drawCanvas(n, m, map, sta, "��ʼ����");
	waitLine(0, "\n���س�������Ѱ�ҳ�ʼ��������Ĳ���...\n", '\n');
	int ret = findBlock(n, m, map, sta);
	if (!ret)
		cout << "��ʼ���޿�������\n";
	else
		drawCanvas(n, m, map, sta, "��ʼ���������ͬɫ��ʶ��");
	return ret;
}

static void initClearBlockLoop(int n, int m, int map[][MAP_SIZE], int sta[][MAP_SIZE])
{
	while (showCanvasAndTag(n, m, map, sta)) {
		waitLine(0, "���س����������������0����...", '\n');
		fallBall(n, m, map, sta);
		drawCanvas(n, m, map, sta, "\n�����0�������:");
		waitLine(0, "���س���������ֵ���", '\n');
		fillVoidBall(n, m, map, sta, BALL_CATEGORY_SIZE, slideDownBall, 0);
		drawCanvas(n, m, map, sta, "\n��ֵ��������飨��ͬɫ��ʶ��");
		waitLine(0, "���س�������", '\n');
		clearStatus(n, m, sta, STA_NORMAL);
	}
}

void baseOptionInit(int n, int m)
{
	int map[MAP_SIZE][MAP_SIZE] = { 0 }, sta[MAP_SIZE][MAP_SIZE] = { 0 };
	generate(n, m, map, BALL_CATEGORY_SIZE);
	showCanvasAndTag(n, m, map, sta);
	waitLine(0, "��С�������������End����...", "�������", "End");
}

void baseOptionDelete(int n, int m)
{
	int map[MAP_SIZE][MAP_SIZE] = { 0 }, sta[MAP_SIZE][MAP_SIZE] = { 0 };
	generate(n, m, map, BALL_CATEGORY_SIZE);
	initClearBlockLoop(n, m, map, sta);
	waitLine(0, "��С�������������End����...", "�������", "End");
}

void baseOptionDeleteHint(int n, int m)
{
	int map[MAP_SIZE][MAP_SIZE] = { 0 }, sta[MAP_SIZE][MAP_SIZE] = { 0 };
	generate(n, m, map, BALL_CATEGORY_SIZE);
	initClearBlockLoop(n, m, map, sta);
	findAvailable(n, m, map, sta);
	drawCanvas(n, m, map, sta, "��ѡ���������ʾ����ͬɫ��ʶ��:");
	waitLine(0, "��С�������������End����...", "�������", "End");
}