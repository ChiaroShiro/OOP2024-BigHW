/* 2351871 ������ �ƿ� */
#include <iostream>
#include <windows.h>
#include "../include/cmd_console_tools.h"
#include "../include/menu.h"
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
	cct_setfontsize("������", 28);
	cct_setcursor(CURSOR_INVISIBLE);
	srand((unsigned int)time(0));
	while (1) {
		cct_setconsoleborder(110, 25);
		int opt = showMenu('a', MENU_CONTENT, 'q', "�˳�");
		
	}
	return 0;
}
