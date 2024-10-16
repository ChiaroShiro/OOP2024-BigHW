/* 2351871 郎若谷 计科 */
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
using namespace std;

bool isUpper(char c)
{
	return c >= 'A' && c <= 'Z';
}

bool isLower(char c)
{
	return c >= 'a' && c <= 'z';
}

bool isNumber(char c)
{
	return c >= '0' && c <= '9';
}

char utol(char c)
{
	return isUpper(c) * (c - 'A' + 'a');
}

char ltou(char c)
{
	return isLower(c) * (c - 'a' + 'A');
}

char itol(int a)
{
	return 'a' + a;
}

char itou(int a)
{
	return 'A' + a;
}

void clearLine(int x, int y, const char* s)
{
	if (y == -1)
		cct_gotoxy(0, x);
	else
		cct_gotoxy(x, y);
	shows(s);
}

char getcc(bool show)
{
	if (show) {
		char c = _getch();
		if (c > 20 && c < 127)
			showc(c);
		return c;
	}
	else
		return _getch();
}

void wait(int x)
{
	if (x)
		Sleep(x);
}

void showNum(int x, int y, int v, int w)
{
	cct_gotoxy(x, y);
	cout << setw(w);
	showi(v);
}

void to_be_continued(const char* prompt, const CONSOLE_GRAPHICS_INFO* const bgi)
{
	if (bgi->inited == CGI_INITED) { //初始化过
		cct_setcolor(bgi->area_bgcolor, bgi->area_fgcolor); //恢复初始颜色

		cct_gotoxy(0, bgi->SLI.lower_start_y + 2);//光标设到指定位置
		cout << setw(bgi->cols - 1) << ' '; //先用空格清空整行

		cct_gotoxy(0, bgi->SLI.lower_start_y + 2);//光标设到指定位置
	}
	else { //未初始化过
		cct_setcolor(); //缺省颜色
		cct_gotoxy(0, 0);//光标设到指定位置
	}

	if (prompt)
		cout << prompt << "，按回车键继续...   ";
	else
		cout << "按回车键继续...   ";

	while (_getch() != '\r')
		;

	return;
}

void waitLine(int x, const char* output, const char* hint, const char* input, bool show)
{
	int posx, posy;
	char s[40] = { 0 }, * lp = s;
	const char* ip = input;
	while (*ip && ip - input < 35) {
		*lp = *ip;
		++ip, ++lp;
	}
	lp = s;
	showLine(output, &posx, &posy);
	cct_gotoxy(posx, posy);
	wait(x);
	while (*lp) {
		if (isUpper(*lp))
			*lp = utol(*lp);
		lp++;
	}
	while (*input) {
		cct_gotoxy(posx, posy);
		clearLine(posx, posy);
		cct_gotoxy(posx, posy);
		char c = getcc(show), * now = s;
		while (c != '\n' && c != '\r' && *now) {
			if ((*now != c && *now != utol(c)) || *(++now) == 0)
				break;
			c = getcc(show);
		}
		if (*input && *now == 0)
			break;
		while (c != '\n' && c != '\r')
			c = getcc(show);
		if (*hint) {
			int tmpx, tmpy;
			showln();
			cct_getxy(tmpx, tmpy);
			shows(hint);
			wait(400);
			clearLine(tmpy);
		}
	}
	wait(400);
}

void waitLine(int x, const char* output, char input, bool show)
{
	int posx, posy;
	showLine(output, &posx, &posy);
	clearLine(posx, posy);
	cct_gotoxy(posx, posy);
	wait(x);
	char c = getcc(show);
	while (c != input) {
		if (c == '\r' && input == '\n')
			break;
		c = getcc(show);
	}
}

void showLine(const char* s, int x, int y)
{
	cct_gotoxy(x, y);
	shows(s);
}

void showLine(const char* s, int* retx, int* rety, int x, int y)
{
	if (x >= 0 && y >= 0)
		cct_gotoxy(x, y);
	shows(s);
	cct_getxy(*retx, *rety);
}

void shows(const char* s) 
{
	int x, y, cx, cy;
	cct_getxy(x, y);
	cct_getcolor(cx, cy);
	cct_showstr(x, y, s, cx, cy);
}

void showc(const char& c) 
{
	int x, y, cx, cy;
	cct_getxy(x, y);
	cct_getcolor(cx, cy);
	cct_showch(x, y, c, cx, cy);
}

void showi (const int &p) 
{
	int x, y, cx, cy;
	cct_getxy(x, y);
	cct_getcolor(cx, cy);
	cct_showint(x, y, p, cx, cy);
}

void showln()
{
	showc('\n');
}

int getLineNumber(int minli, int maxli, const char* msg)
{
	int n;
	while (1) {
		shows(msg);
		showln();
		cin >> n;
		if (!cin.fail() && n <= maxli && n >= minli)
			break;
		cin.clear();
		cin.ignore(10000, '\n');
	}
	return n;
}

void programExit()
{
	showln();
	showln();
	showln();
	showln();
	showln();
	showln();
	system("pause");
}

void waitForEndlineOrClick()
{
	int x, y, mevent, keya, keyb;
	while (1) {
		cct_enable_mouse();
		int korm = cct_read_keyboard_and_mouse(x, y, mevent, keya, keyb);
		if (korm == CCT_KEYBOARD_EVENT && (keya == '\n' || keya == '\r'))
			break;
		if (korm == CCT_MOUSE_EVENT && mevent == MOUSE_LEFT_BUTTON_CLICK)
			break;
	}
}

void gotoBottom(int n, int showBorder)
{
	cct_gotoxy(0, 4 + 3 * n + (n - 1) * showBorder);
}

void showBottom(int n, const char* s, int showBorder, int bgcol, int frcol)
{
	gotoBottom(n, showBorder);
	cct_setcolor(bgcol, frcol);
	shows(s);
}