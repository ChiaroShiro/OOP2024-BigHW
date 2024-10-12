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

static int readPosition(int n, int m, int map[][MAP_SIZE], int sta[][MAP_SIZE])
{
	char indata[200] = { 0 };
	int ret = 0;
	int x = 0, y = 0;
	while (1) {
		shows("������ĸ+������ʽ[����c2]����������꣺");
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
					ret = isErasable(n, m, x, y, map, sta);
					if (ret > 0) {
						shows("���Ϊ");
						showc(ltou(indata[0]));
						shows("��");
						showc(indata[1]);
						shows("��\n\n");
						break;
					}
					if (ret == -1)
						shows("����ľ�������λ��Ϊ0���Ƿ�λ�ã�������������\n");
					else
						shows("����ľ�������λ�ô���������ֵͬ������������\n");
					continue;
				}
			}
		}
		shows("�����������������");
		clearLine(clsx, clsy);
		cct_gotoxy(0, clsy);
	}
	return ret;
}

void baseDrawing(int n, int m, int map[][MAP_SIZE], int sta[][MAP_SIZE], int optChoose)
{
	int nowScore = 0, totScore = 0;
	clearStatus(n, m, sta, STA_NORMAL);
	while (isPossible(n, m, map, sta)) {
		TransferStatus(n, m, sta, STA_NEED_DEL, STA_NORMAL);
		drawCanvas(n, m, map, sta, "\n\n��ǰ����");
		nowScore = readPosition(n, m, map, sta);
		drawStatus(n, m, sta, "���ҽ������");
		showln();
		drawCanvas(n, m, map, sta, "��ǰ���飨��ͬɫ��ʶ��:", STA_NEED_DEL);
		
		if (optChoose == 1)
			break;

		shows("��ȷ���Ƿ��A1����Χ����ֵͬ����(Y/N/Q)��");
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

		deleteBall(n, m, 0, map, sta);
		drawCanvas(n, m, map, sta, "\n\n��ֵͬ�鲢�������(��ͬɫ��ʶ)��", STA_NOW_DEL);
		nowScore = nowScore * nowScore * 5;
		totScore += nowScore;
		shows("\n���ε÷֣�");
		showi(nowScore);
		shows(" �ܵ÷֣�");
		showi(totScore);
		shows("\n\n");
		waitLine(0, "���س������������������...", '\n', 0);
		fallBall(n, m, map, sta);
		squeezeBall(n, m, map, sta);
		drawCanvas(n, m, map, sta, "���������飺");

		if (optChoose == 2)
			break;

		waitLine(0, "�����������������س���������һ�ε�����...", '\n', 0);
	}
}