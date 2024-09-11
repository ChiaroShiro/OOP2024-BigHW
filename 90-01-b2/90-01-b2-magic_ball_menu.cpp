/*2351871 郎若谷 计科*/
#include <iostream>
#include <iomanip>
#include <cstring>
#include <cmath>
#include <ctime>
#include <windows.h>
#include <conio.h>
#include "../include/cmd_console_tools.h"
#include "90-01-b2-magic_ball.h"
using namespace std;

int __showMenu()
{
	cct_cls();
	cct_setconsoleborder(INIT_X_SIZE, INIT_Y_SIZE);
	cout << "\
------------------------------------------------------------\n\
1.内部数组，生成初始状态，寻找是否有初始可消除项\n\
2.内部数组，消除初始可消除项后非0项下落并用0填充\n\
3.内部数组，消除初始可消除项后查找消除提示\n\
4.n * n的框架(无分隔线)，显示初始状态\n\
5.n * n的框架(有分隔线)，显示初始状态\n\
6.n * n的框架(无分隔线)，显示初始状态及初始可消除项\n\
7.n * n的框架(有分隔线)，消除初始可消除项后显示消除提示\n\
8.cmd图形界面完整版(有分隔线，鼠标移动时显示坐标，右键退出)\n\
9.cmd图形界面完整版(尚未完成，会尽快补发)\n\
0.退出\n\
------------------------------------------------------------\n\
[请选择:] ";
	char c = getc();
	while (c < '0' || c > '9')
		c = getc();
	cout << c << '\n';
	wait(200);
	return c - '0';
}