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

StyleCSS::StyleCSS() {
	for (int i = 0; i < 3; i++) {
		this->head[i] = DOUBLE_HEAD[i];
		this->tran[i] = DOUBLE_TRAN[i];
		this->tail[i] = DOUBLE_TAIL[i];
	}
	line = DOUBLE_LINE;
	vert = DOUBLE_VERT;
	now = 0;
}

void StyleCSS::setHead(const char* news[])
{
	for (int i = 0; i < 3; i++)
		this->head[i] = news[i];
}
void StyleCSS::setTail(const char* news[])
{
	for (int i = 0; i < 3; i++)
		this->tail[i] = news[i];
}
void StyleCSS::setTran(const char* news[])
{
	for (int i = 0; i < 3; i++)
		this->tran[i] = news[i];
}
void StyleCSS::setLine(const char* news)
{
	this->line = news;
}
void StyleCSS::setVert(const char* news)
{
	this->vert = news;
}
void StyleCSS::setOutput(int kind)
{
	this->now = kind;
}
const char* StyleCSS::getHead()
{
	return this->head[this->now];
}
const char* StyleCSS::getTail()
{
	return this->tail[this->now];
}
const char* StyleCSS::getTran()
{
	return this->tran[this->now];
}
const char* StyleCSS::getLine()
{
	return this->line;
}
const char* StyleCSS::getVert()
{
	return this->vert;
}


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

void TransferStatus(int n, int m, int sta[][MAP_SIZE], int ori, int trans)
{
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= m; j++)
			if (sta[i][j] == ori)
				sta[i][j] = trans;
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

void deleteBall(int n, int m, int map[][MAP_SIZE], int sta[][MAP_SIZE], bool showGraph, void (*delBall)(int, int, int))
{
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			if (sta[i][j] == STA_NEED_DEL) {
				sta[i][j] = STA_NOW_DEL;
				if (showGraph)
					dfsFindBlock(i, j, n, m, map, sta, delBall);
				map[i][j] = 0;
			}
		}
	}
}

void fallBall(int n, int m, int map[][MAP_SIZE], int sta[][MAP_SIZE], bool showGraph, void (*slideDownBall)(int, int, int, int, int))
{
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
 * ��ʽ��
 */
static void drawOneSolidLine(int m, int kind, bool showBorder, int elelen, int gap, StyleCSS style)
{
	style.setOutput(kind);
	cout << style.getHead();
	wait(gap);
	for (int i = 1; i < m; i++) {
		for (int j = 0; j < elelen; j++)
			cout << style.getLine();
		wait(gap);
		if (showBorder) {
			cout << style.getTran();
			wait(gap);
		}
	}
	for (int j = 0; j < elelen; j++)
		cout << style.getLine();
	cout << style.getTail() << '\n';
	wait(gap);
}


/*
 * ��һ�����У�m Ϊ���ȣ�showBorder Ϊ 1 ���б߿򣬷����ޱ߿�
 * ÿ��Ԫ�صĳ����� elelen ������gap Ϊ�滭ÿ��Ԫ�غ����ͣʱ��
 */
static void drawOneHollowLine(int m, bool showBorder, int elelen, int gap, StyleCSS style)
{
	cout << style.getVert();
	wait(gap);
	for (int i = 1; i < m; i++) {
		for (int j = 0; j < elelen; j++)
			cout << "  ";
		wait(gap);
		if (showBorder) {
			cout << style.getVert();
			wait(gap);
		}
	}
	for (int j = 0; j < elelen; j++)
		cout << "  ";
	cout << style.getVert() << '\n';
	wait(gap);
}

void drawBackground(int n, int m, bool showBorder, int* totx, int* toty,
						   int coren, int corem, StyleCSS style, int gap)
{
	getpos(n, m, totx, toty, showBorder, coren, corem);
	*totx = max(*totx + 12, 40);
	*toty = max(*toty + 7, 14);
	cct_setconsoleborder(*totx, *toty);
	cct_gotoxy(0, 0);
	cout << "��Ļ��" << *totx << "��" << *toty << "��\n";
	cct_setcolor(COLOR_WHITE, COLOR_BLACK);
	drawOneSolidLine(m, BUP, showBorder, corem, gap, style);
	for (int i = 1; i < n; i++) {
		for (int j = 0; j < coren; j++)
			drawOneHollowLine(m, showBorder, corem, gap, style);
		if (showBorder)
			drawOneSolidLine(m, BMID, showBorder, corem, gap, style);
	}
	for (int j = 0; j < coren; j++)
		drawOneHollowLine(m, showBorder, corem, gap, style);
	drawOneSolidLine(m, BDOWN, showBorder, corem, gap, style);
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
	deleteBall(n, m, map, sta, 1, delBall);
	fallBall(n, m, map, sta, 1, slideDownBall);
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

void drawCanvas(int n, int m, const int map[][MAP_SIZE], const int sta[][MAP_SIZE], const char* s, int colorTag)
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
			if(colorTag == -1 || colorTag == sta[i][j])
				cct_setcolor(COLOR_RED * !!sta[i][j]);
			cout << map[i][j];
			cct_setcolor();
		}
		cout << '\n';
	}
}