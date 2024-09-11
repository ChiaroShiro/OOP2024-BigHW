/* 2351871 郎若谷 计科 */
#include <iostream>
#include <conio.h>
#include "../include/cmd_console_tools.h"
using namespace std;

int menu()
{
	cct_cls();
	cout << "---------------------------------" << endl;
	cout << "1.基本解" << endl;
	cout << "2.基本解(步数记录)" << endl;
	cout << "3.内部数组显示(横向)" << endl;
	cout << "4.内部数组显示(纵向+横向)" << endl;
	cout << "5.图形解-预备-画三个圆柱" << endl;
	cout << "6.图形解-预备-在起始柱上画n个盘子" << endl;
	cout << "7.图形解-预备-第一次移动" << endl;
	cout << "8.图形解-自动移动版本" << endl;
	cout << "9.图形解-游戏版" << endl;
	cout << "0.退出" << endl;
	cout << "---------------------------------" << endl;
	cout << "[请选择:] ";
	char c = _getch();
	while (c < '0' || c > '9')
		c = _getch();
	cout << c << "\n\n\n";
	return c - '0';
}