/*2351871 ������ �ƿ�*/
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

const char* MENU_CONTENT[] = {
	"�ڲ����飬���ɳ�ʼ״̬��Ѱ���Ƿ��г�ʼ��������",
	"�ڲ����飬������ʼ����������0�����䲢��0���",
	"�ڲ����飬������ʼ������������������ʾ",
	"n * n�Ŀ��(�޷ָ���)����ʾ��ʼ״̬",
	"n * n�Ŀ��(�зָ���)����ʾ��ʼ״̬",
	"n * n�Ŀ��(�޷ָ���)����ʾ��ʼ״̬����ʼ��������",
	"n * n�Ŀ��(�зָ���)��������ʼ�����������ʾ������ʾ",
	"cmdͼ�ν���������(�зָ��ߣ�����ƶ�ʱ��ʾ���꣬�Ҽ��˳�)",
	"cmdͼ�ν���������",
	""
};

int main()
{
	cct_setfontsize("������", 28);
	cct_setcursor(CURSOR_INVISIBLE);
	int n, m;
	srand((unsigned int)time(0));
	while (1) {
		cct_setconsoleborder(110, 25);
		int opt = showMenu('1', MENU_CONTENT, '0', "�˳�");
		if (opt == -1) {
			programExit();
			break;
		}
		cct_cls();
		n = getLineNumber(5, 9, "����������(5-9)��");
		m = getLineNumber(5, 9, "����������(5-9)��");
		if (opt == 1)
			baseOptionInit(n, m);
		else if (opt == 2)
			baseOptionDelete(n, m);
		else if (opt == 3)
			baseOptionDeleteHint(n, m);
		else if (opt == 4)
			graphOptionInitBorder(n, m, 0);
		else if (opt == 5)
			graphOptionInitBorder(n, m, 1);
		else if (opt == 6)
			graphOptionDelete(n, m);
		else if (opt == 7)
			graphOptionDeleteHint(n, m);
		else if (opt == 8)
			mainWork(n, m, 0);
		else
			mainWork(n, m);
	}
	return 0;
}