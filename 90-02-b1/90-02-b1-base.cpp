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
#include "../include/menu.h"
#include "90-02-b1-head.h"
using namespace std;

void baseDrawing(int n, int m, int map[][MAP_SIZE], int sta[][MAP_SIZE])
{
	clearStatus(n, m, sta, STA_NORMAL);
	while (1) {
		drawCanvas(n, m, map, sta, "��ǰ����");
		char indata[] = "";
		while (1) {
			cout << "������ĸ+������ʽ[����c2]����������꣺";
			cin >> indata;
			if()
		}
	}
}