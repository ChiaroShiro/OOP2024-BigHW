/* 2351871 ������ �ƿ� */

#include <iostream>
#include <iomanip>
#include <cstring>
#include <cmath>
#include <ctime>
#include <windows.h>
#include <conio.h>
#include "../include/cmd_console_tools.h"
#include "../include/cmd_gmw_tools.h"
#include "../include/io_tools.h"
#include "../include/matrix.h"
#include "../include/menu.h"
#include "90-02-b2-head.h"
using namespace std;

int main() 
{
	cct_setcolor();
	cct_setfontsize("������", 16);
	cct_setcursor(CURSOR_INVISIBLE);
	srand((unsigned int)time(0));
	int n, m, v, p;
	while (1) {
		cct_setconsoleborder(110, 35);
		cct_cls();
		n = getLineNumber(3, 7, "������������3-7��:");
		m = getLineNumber(3, 7, "������������3-7��:");
		v = getLineNumber(0, 2, "��ѡ���ƶ��ٶȣ�0-2��0Ϊ���٣�1Ϊ���٣�2Ϊ���٣�");
		p = getLineNumber(0, 2048, "��ѡ���³��ֿ����ֵ���ޣ�0-2048���Ƽ�Ϊ4�������������2�������η�����ȡ����");
		for (int i = 0; i < 11; i++) {
			if (NUM2K[i] >= p) {
				p = NUM2K[i];
				break;
			}
		}
		if (v == 2)
			v = 10;
		game2048(n, m, v, p);
	}
	return 0;
}