/* 2351871 郎若谷 计科 */
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
#include "../include/menu.h"
#include "90-02-b1-head.h"
using namespace std;

void baseDrawing(int n, int m, int map[][MAP_SIZE], int sta[][MAP_SIZE])
{
	clearStatus(n, m, sta, STA_NORMAL);
	while (1) {
		drawCanvas(n, m, map, sta, "当前数组");
		char indata[] = "";
		while (1) {
			cout << "请以字母+数字形式[例：c2]输入矩阵坐标：";
			cin >> indata;
			if()
		}
	}
}