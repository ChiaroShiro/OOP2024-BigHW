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



void baseDrawing(int n, int m, int map[][MAP_SIZE], int sta[][MAP_SIZE], int optChoose)
{
	int nowScore = 0, totScore = 0;
	clearStatus(n, m, sta, STA_NORMAL);
	while (isPossible(n, m, map, sta)) {
		TransferStatus(n, m, sta, STA_NEED_DEL, STA_NORMAL);
		drawCanvas(n, m, map, sta, "\n\n当前数组");
		char indata[200] = { 0 };
		int x = 0, y = 0;
		while (1) {
			cout << "请以字母+数字形式[例：c2]输入矩阵坐标：";
			int clsx, clsy;
			cct_getxy(clsx, clsy);
			cin >> indata;
			if (isUpper(indata[0]))
				indata[0] = utol(indata[0]);
			if (indata[0] >= 'a' && indata[0] < 'a' + n) {
				if (indata[1] >= '1' && indata[1] < '1' + m) {
					x = indata[0] - 'a' + 1;
					y = indata[1] - '1' + 1;
					bool lencheck = 1;
					if (indata[1] == '1' && indata[2] == '0' && m == 10) {
						if (strlen(indata) != 3)
							lencheck = 0;
						y = 10;
					}
					else if (strlen(indata) != 2)
						lencheck = 0;
					if (lencheck) {
						nowScore = isErasable(n, m, x, y, map, sta);
						if (nowScore > 0) {
							cout << "输出为" << ltou(indata[0]) << "行" << indata[1] << "列\n\n";
							break;
						}
						if (nowScore == -1)
							cout << "输入的矩阵坐标位置为0（非法位置），请重新输入\n";
						else
							cout << "输入的矩阵坐标位置处无连续相同值，请重新输入\n";
						continue;
					}
				}
			}
			cout << "输入错误请重新输入";
			clearLine(clsx, clsy);
			cct_gotoxy(0, clsy);
		}
		drawStatus(n, m, sta, "查找结果数组");
		cout << "\n";
		drawCanvas(n, m, map, sta, "当前数组（不同色标识）:", STA_NEED_DEL);
		
		if (optChoose == 1)
			break;

		cout << "请确认是否把A1及周围的相同值消除(Y/N/Q)：";
		char chs;
		while (1) {
			chs = getcc();
			chs = isLower(chs) ? ltou(chs) : chs;
			if (chs == 'Y' || chs == 'N' || chs == 'Q')
				break;
		}
		if (chs == 'Q')
			break;
		if (chs == 'N')
			continue;

		deleteBall(n, m, map, sta);
		drawCanvas(n, m, map, sta, "\n\n相同值归并后的数组(不同色标识)：", STA_NOW_DEL);
		nowScore = nowScore * nowScore * 5;
		totScore += nowScore;
		cout << "\n本次得分：" << nowScore << " 总得分：" << totScore << "\n\n";
		waitLine(0, "按回车键进行数组下落操作...", '\n', 0);
		fallBall(n, m, map, sta);
		squeezeBall(n, m, map, sta);
		drawCanvas(n, m, map, sta, "下落后的数组：");

		if (optChoose == 2)
			break;

		waitLine(0, "本次消除结束，按回车键继续新一次的消除...", '\n', 0);
	}
}