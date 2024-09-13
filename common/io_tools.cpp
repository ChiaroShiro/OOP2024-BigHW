/* 2351871 ¿…»Ùπ» º∆ø∆ */
#include <iostream>
#include <iomanip>
#include <cstring>
#include <cmath>
#include <ctime>
#include <windows.h>
#include <conio.h>
#include "../include/cmd_console_tools.h"
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
	cout << s;
}

char getcc(bool show)
{
	if (show) {
		char c = _getch();
		if (c > 20 && c < 127)
			cout << c;
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
	cout << setw(w) << v;
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
			cout << endl;
			cct_getxy(tmpx, tmpy);
			cout << hint;
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
	cout << s;
}

void showLine(const char* s, int* retx, int* rety, int x, int y)
{
	if (x >= 0 && y >= 0)
		cct_gotoxy(x, y);
	cout << s;
	cct_getxy(*retx, *rety);
}

int getLineNumber(int minli, int maxli, const char* msg)
{
	int n;
	while (1) {
		cout << msg << '\n';
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
	cout << "\n\n\n\n\n\n";
	system("pause");
}
