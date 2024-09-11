/*2351871 ¿…»Ùπ» º∆ø∆*/

#include <iostream>
#include <iomanip>
#include <cstring>
#include <ctime>
#include <windows.h>
#include <conio.h>
#include "../include/cmd_console_tools.h"
#include "../include/io_tools.h"
#include "../include/menu.h"
using namespace std;

int showMenu(int optmin, const char* content[], int optq, const char* qcontent)
{
	cct_cls();
	optmin = isLower(optmin) ? ltou(optmin) : optmin;
	optq   = isLower(optq)   ? ltou(optq)   : optq;
	int rows = 0, cols = 0;
	const char* p = content[0];
	while (*p) {
		cols = max(cols, (int)strlen(p));
		p = content[++rows];
	}

	for (int i = 0; i < cols + 3; i++)
		cout << '-';
	cout << '\n';
	for (int i = 0; i < rows; i++)
		cout << (char)(i + optmin) << ". " << content[i] << '\n';
	if (optq != -1)
		cout << (char)optq << ". " << qcontent << '\n';
	for (int i = 0; i < cols + 3; i++)
		cout << '-';

	cout << "\n[«Î—°‘Ò:] ";
	char c = getcc();
	c = isLower(c) ? ltou(c) : c;
	while ((c < optmin || c >= optmin + rows) && c != optq) {
		c = getcc();
		c = isLower(c) ? ltou(c) : c;
	}
	cout << c << '\n';
	wait(200);
	return c == optq ? -1 : c - optmin + 1;
}