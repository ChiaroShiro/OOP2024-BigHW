/*2351871 郎若谷 计科*/
#include <iostream>
#include <iomanip>
#include <cstring>
#include <cmath>
#include <ctime>
#include <windows.h>
#include <conio.h>
#include "../include/cmd_console_tools.h"
#include "../include/menu.h"
#include "../include/io_tools.h"
#include "90-01-b2-magic_ball.h"
using namespace std;

int main()
{
	cct_setfontsize("新宋体", 28);
	cct_setcursor(CURSOR_INVISIBLE);
	int n, m;
	srand((unsigned int)time(0));
	while (1) {
		cct_setconsoleborder(110, 25);
		cct_cls();
		n = getLineNumber(5, 9, "请输入行数(5-9)：");
		m = getLineNumber(5, 9, "请输入列数(5-9)：");
		mainWork(n, m);
	}
	return 0;
}