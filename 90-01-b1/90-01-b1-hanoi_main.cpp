/* 2351871 郎若谷 计科 */
#include <iostream>
#include <iomanip>
#include <windows.h>
#include "../include/cmd_console_tools.h"
#include "../include/menu.h"
#include "90-01-b1-hanoi.h"
using namespace std;

const char* MENU_CONTENT[] = {
    "基本解",
    "基本解(步数记录)",
    "内部数组显示(横向)",
    "内部数组显示(纵向+横向)",
    "图形解-预备-画三个圆柱",
    "图形解-预备-在起始柱上画n个盘子",
    "图形解-预备-第一次移动",
    "图形解-自动移动版本",
    "图形解-游戏版",
    ""
};

int main()
{
    cct_setconsoleborder(120, 40, 120, 9000);
    int n, bg, ed, tim = -1;
    while (1) {
        clear();
        cct_setcursor(CURSOR_VISIBLE_NORMAL);
        int opt = showMenu('1', MENU_CONTENT, '0', "退出");
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
            wait_endl("\n按回车键继续 ");
        }
    }
    return 0;
}