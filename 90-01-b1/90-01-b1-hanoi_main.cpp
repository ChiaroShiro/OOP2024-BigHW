/* 2351871 ������ �ƿ� */
#include <iostream>
#include <iomanip>
#include <windows.h>
#include "../include/cmd_console_tools.h"
#include "../include/menu.h"
#include "90-01-b1-hanoi.h"
using namespace std;

const char* MENU_CONTENT[] = {
    "������",
    "������(������¼)",
    "�ڲ�������ʾ(����)",
    "�ڲ�������ʾ(����+����)",
    "ͼ�ν�-Ԥ��-������Բ��",
    "ͼ�ν�-Ԥ��-����ʼ���ϻ�n������",
    "ͼ�ν�-Ԥ��-��һ���ƶ�",
    "ͼ�ν�-�Զ��ƶ��汾",
    "ͼ�ν�-��Ϸ��",
    ""
};

int main()
{
    cct_setconsoleborder(120, 40, 120, 9000);
    int n, bg, ed, tim = -1;
    while (1) {
        clear();
        cct_setcursor(CURSOR_VISIBLE_NORMAL);
        int opt = showMenu('1', MENU_CONTENT, '0', "�˳�");
        if (opt == 0)
            break;
        if (opt == 5) {
            process_opt5();
            continue;
        }
        input_data(opt, n, bg, ed, tim);
        cct_setcursor(CURSOR_INVISIBLE);
        stack_build(bg, n);
        if (opt == 6)
            process_opt6(bg);
        else if (opt == 7)
            process_opt7(n, bg, get_third_number(bg, ed), ed);
        else {
            draw_canvas(n, bg, opt, ed, tim);
            if (opt == 9) {
                game_mode(n, ed);
                continue;
            }
            hanoi(n, bg, get_third_number(bg, ed), ed, opt);
            if(opt >= 4)
                cct_gotoxy(0, Y_BOT_BOTTOM + BOT_BIAS + 3);
            wait_endl("\n���س������� ");
        }
    }
    return 0;
}