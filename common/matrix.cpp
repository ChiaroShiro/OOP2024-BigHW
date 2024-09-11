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
using namespace std;
 
void getpos(int i, int j, int* x, int* y, int showBorder, int cn, int cm)
{
	// y ��������x ������
	*x = 2 + (cm * 2 + 2 * showBorder) * (j - 1);
	*y = 2 + (cn + 1 * showBorder) * (i - 1);
}

void generate(int n, int m, int map[][MAP_SIZE], int cates)
{
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= m; j++)
			map[i][j] = rand() % cates + 1;
}

void clearStatus(int n, int m, int sta[][MAP_SIZE], int x)
{
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= m; j++)
			sta[i][j] = x;
}

void dfsFindBlock(int x, int y, int n, int m, const int map[][MAP_SIZE], int sta[][MAP_SIZE], void (*delBall)(int, int, int))
{
	static const int FX[] = { 0, 0, -1, 1 };
	static const int FY[] = { 1, -1, 0, 0 };
	sta[x][y] = STA_NOW_DEL;
	delBall(x, y, map[x][y]);
	for (int i = 0; i < 4; i++) {
		int nextx = x + FX[i];
		int nexty = y + FY[i];
		if (nextx >= 1 && nexty >= 1 && nextx <= n && nexty <= m)
			if (sta[nextx][nexty] == STA_NEED_DEL && map[nextx][nexty] == map[x][y])
				dfsFindBlock(nextx, nexty, n, m, map, sta, delBall);
	}
}

void fallBall(int n, int m, int map[][MAP_SIZE], int sta[][MAP_SIZE], bool showGraph, void (*delBall)(int, int, int), void (*slideDownBall)(int, int, int, int, int))
{
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			if (sta[i][j] == STA_NEED_DEL) {
				sta[i][j] = STA_NOW_DEL;
				if (showGraph)
					dfsFindBlock(i, j, n, m, map, sta, delBall);
			}
		}
	}
	for (int j = 1; j <= m; j++) {
		int downGap = 0, now = n;
		while (now) {
			if (sta[now][j] == STA_NOW_DEL) {
				++downGap;
				sta[now][j] = STA_VOID;
			}
			else {
				if (downGap) {
					for (int i = 0; showGraph && i < downGap; i++)
						slideDownBall(n, m, now + i, j, map[now][j]);
					sta[now + downGap][j] = sta[now][j];
					map[now + downGap][j] = map[now][j];
					sta[now][j] = STA_VOID;
				}
			}
			--now;
		}
	}
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= m; j++)
			if (sta[i][j] == STA_VOID)
				map[i][j] = 0;
}

static void newBall(int n, int m, int x, int y, int map[][MAP_SIZE], int sta[][MAP_SIZE], bool showGraph, int cates, void (*slideDownBall)(int, int, int, int, int))
{
	map[x][y] = rand() % cates + 1;
	sta[x][y] = STA_NEW;
	if (!showGraph)
		return;
	for (int i = 0; i < x; i++)
		slideDownBall(n, m, i, y, map[x][y]);
}

int fillVoidBall(int n, int m, int map[][MAP_SIZE], int sta[][MAP_SIZE], 
				 int cates, void (*slideDownBall)(int, int, int, int, int), bool showGraph)
{
	int ret = 0;
	for (int j = 1; j <= m; j++) {
		int now = 1;
		while (sta[now][j] == STA_VOID)
			++now, ++ret;
		--now;
		while (now) {
			newBall(n, m, now, j, map, sta, showGraph, cates, slideDownBall);
			--now;
		}
	}
	return ret;
}

/*
 * ��һʵ���У�m Ϊ���ȣ�kind Ϊ����Ԫ�ص���ʽ��showBorder Ϊ 1 ���б߿򣬷����ޱ߿�
 * ÿ��Ԫ�صĳ����� elelen ������gap Ϊ�滭ÿ��Ԫ�غ����ͣʱ��
 */
static void drawOneSolidLine(int m, int kind, bool showBorder, int elelen, int gap)
{
	cout << BHEAD[kind];
	wait(gap);
	for (int i = 1; i < m; i++) {
		for (int j = 0; j < elelen; j++)
			cout << BHENG;
		wait(gap);
		if (showBorder) {
			cout << BBODY[kind];
			wait(gap);
		}
	}
	for (int j = 0; j < elelen; j++)
		cout << BHENG;
	cout << BTAIL[kind] << '\n';
	wait(gap);
}


/*
 * ��һ�����У�m Ϊ���ȣ�showBorder Ϊ 1 ���б߿򣬷����ޱ߿�
 * ÿ��Ԫ�صĳ����� elelen ������gap Ϊ�滭ÿ��Ԫ�غ����ͣʱ��
 */
static void drawOneHollowLine(int m, bool showBorder, int elelen, int gap)
{
	cout << BSHU;
	wait(gap);
	for (int i = 1; i < m; i++) {
		for (int j = 0; j < elelen; j++)
			cout << "  ";
		wait(gap);
		if (showBorder) {
			cout << BSHU;
			wait(gap);
		}
	}
	for (int j = 0; j < elelen; j++)
		cout << "  ";
	cout << BSHU << '\n';
	wait(gap);
}

/*
 * ���� n �� m �и�Ԫ�صı�����
 * showBorder Ϊ 1 ����ʾ�߽磬������ʾ
 * totx �� toty ��洢���ĺ����Ļ�ĳ���
 * coren Ϊÿ��Ԫ�ص�������corem Ϊÿ��Ԫ�صĿ�ȣ������ַ���ȣ�
 * gap Ϊ����ÿ��Ԫ�غ����ͣʱ��
 */
void drawBackground(int n, int m, bool showBorder, int* totx, int* toty,
						   int coren, int corem, int gap)
{
	getpos(n, m, totx, toty, showBorder, coren, corem);
	*totx = max(*totx + 12, 40);
	*toty = max(*toty + 7, 14);
	cct_setconsoleborder(*totx, *toty);
	cct_gotoxy(0, 0);
	cout << "��Ļ��" << *totx << "��" << *toty << "��\n";
	cct_setcolor(COLOR_WHITE, COLOR_BLACK);
	drawOneSolidLine(m, BUP, showBorder, corem, gap);
	for (int i = 1; i < n; i++) {
		for (int j = 0; j < coren; j++)
			drawOneHollowLine(m, showBorder, corem, gap);
		if (showBorder)
			drawOneSolidLine(m, BMID, showBorder, corem, gap);
	}
	for (int j = 0; j < coren; j++)
		drawOneHollowLine(m, showBorder, corem, gap);
	drawOneSolidLine(m, BDOWN, showBorder, corem, gap);
	cct_setcolor();
}

int oneDrawing(int n, int m, int y_size, int map[][MAP_SIZE], int sta[][MAP_SIZE], bool isGap, int cates, 
			   void (*slideDownBall)(int, int, int, int, int), 
			   void (*delBall)(int, int, int), 
			   void (*drawFrontBall)(int, int, int[][MAP_SIZE], int[][MAP_SIZE], bool, int),
			   void (*extraMoving)(int, int, int[][MAP_SIZE], int[][MAP_SIZE], int, void(*)(int, int, int), void (*)(int, int, int, int, int)))
{
	int ret = 0;
	drawFrontBall(n, m, map, sta, 1, 0);
	if (isGap)
		wait(400);
	else {
		cct_gotoxy(0, y_size - 3);
		waitLine(0, "���س������������������0����", '\n');
	}
	fallBall(n, m, map, sta, 1, delBall, slideDownBall);
	if (extraMoving != NULL)
		extraMoving(n, m, map, sta, 1, delBall, slideDownBall);
	if (!isGap) {
		cct_gotoxy(0, y_size - 3);
		waitLine(0, "���س���������ֵ���", '\n');
	}
	ret += fillVoidBall(n, m, map, sta, cates, slideDownBall);
	if (isGap)
		wait(400);
	return ret;
}