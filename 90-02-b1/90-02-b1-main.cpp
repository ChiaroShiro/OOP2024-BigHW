/* 2351871 ������ �ƿ� */
#include <iostream>
#include <windows.h>
#include "../include/cmd_console_tools.h"
#include "../include/io_tools.h"
#include "../include/matrix.h"
#include "../include/menu.h"
#include "90-02-b1-head.h"
using namespace std;

const char* MENU_CONTENT[] = {
	"�������ҳ����������ʶ",
	"���������һ���������ֲ�����ʾ��",
	"���������һ�أ��ֲ�����ʾ��",
	"αͼ�ν����������ѡ��һ��ɫ�飨�޷ָ��ߣ�",
	"α�ν����������ѡ��һ��ɫ�飨�зָ��ߣ�",
	"αͼ�ν������һ���������ֲ��裩",
	"αͼ�ν���������",
	""
};

int main()
{
	cct_setfontsize("������", 20);
	cct_setcursor(CURSOR_INVISIBLE);
	srand((unsigned int)time(0));
	int n, m;
	while (1) {
		cct_setconsoleborder(110, 35);
		int opt = showMenu('a', MENU_CONTENT, 'q', "�˳�");
		if (opt == -1) {
			programExit();
			break;
		}
		cct_cls();
		n = getLineNumber(8, 10, "������������8-10��:");
		m = getLineNumber(8, 10, "������������8-10��:");
		if (opt <= 3)
			NonAnimeOption(n, m, opt);
		if (opt == 4)
			;
		if (opt == 5)
			;
		if (opt == 6)
			;
		if (opt == 7)
			;
	}
	return 0;
}
