/* 2351871 ������ �ƿ� */
#include <iostream>
#include <windows.h>
#include "../include/cmd_console_tools.h"
#include "../include/io_tools.h"
#include "../include/matrix.h"
#include "../include/menu.h"
#include "90-02-b1-head.h"
using namespace std;

int main()
{
	cct_setcolor();
	cct_setfontsize("������", 16);
	cct_setcursor(CURSOR_INVISIBLE);
	srand((unsigned int)time(0));
	int n, m;
	while (1) {
		cct_setconsoleborder(110, 35);
		cct_cls();
		n = getLineNumber(8, 10, "������������8-10��:");
		m = getLineNumber(8, 10, "������������8-10��:");
		AnimeOption(n, m, 7);
	}
	return 0;
}
