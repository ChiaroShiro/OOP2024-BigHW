/* 2351871 郎若谷 计科 */
#include <iostream>
#include <iomanip>
#include <conio.h>
#include <windows.h>
#include "../include/cmd_console_tools.h"
#include "../include/io_tools.h"
#include "90-01-b1-hanoi.h"
using namespace std;

const int X_COL[] = { X_A_COL, X_B_COL, X_C_COL };
const int X_BOT[] = { X_A_BOT, X_B_BOT, X_C_BOT };

int top[3];
int stk[3][20];
int step, gap;

void stack_build(int id, int n)
{
    for (int i = n; i; i--)
        stack_push(id, i);
}

void stack_push(int id, int x)
{
    stk[id][++top[id]] = x;
}

void stack_pop(int id)
{
    stk[id][top[id]--] = 0;
}

int stack_top(int id)
{
    return stk[id][top[id]];
}

void draw(int x, int y, char c)
{
    cct_gotoxy(x, y);
    cout << c;
}

int get_third_number(int a, int b)
{
    bool post[] = { 0, 0, 0 };
    post[a] = post[b] = 1;
    for (int i = 0; i < 3; i++) {
        if (post[i] == 0)
            return i;
    }
    return -1;
}

void wait_endl(const char s[])
{
    cout << s;
    char c = _getch();
    while (c != '\r' && c != '\n')
        c = _getch();
}

void wait_gap(int swi)
{
    if(gap > 5)
        Sleep(gap);
    else if (gap == 0) {
        if (swi)
            Sleep(SMALL_GAP);
        else
            wait_endl("");
    }
}

int get_gap(int t)
{
    switch (t) {
        case 0:
            return 0;
        case 1:
            return 1000;
        case 2:
            return 400;
        case 3:
            return 100;
        case 4:
            return 6;
        case 5:
            return 2;
    }
    return -1;
}

void set_color_init()
{
    cct_setcolor(COLOR_BLACK, COLOR_WHITE);
}

int getlen(int x)
{
    return x * 2 + 1;
}

void input_data(int opt, int& n, int& bg, int& ed, int& tim)
{
    char begint, endt;
    while (1) {
        cout << "请输入汉诺塔的层数(1-10)\n";
        std::cin >> n;
        std::cin.clear();
        std::cin.ignore(1000000, '\n');
        if (std::cin.fail())
            continue;
        if (n >= 1 && n <= 10)
            break;
    }
    while (1) {
        cout << "请输入起始柱(A-C)\n";
        std::cin >> begint;
        std::cin.clear();
        std::cin.ignore(1000000, '\n');
        if (begint >= 'a' && begint <= 'c')
            begint = begint - 'a' + 'A';
        if (begint >= 'A' && begint <= 'C')
            break;
    }
    while (1) {
        cout << "请输入目标柱(A-C)\n";
        std::cin >> endt;
        std::cin.clear();
        std::cin.ignore(1000000, '\n');
        if (endt >= 'a' && endt <= 'c')
            endt = endt - 'a' + 'A';
        if (endt >= 'A' && endt <= 'C') {
            if (endt == begint)
                cout << "目标柱(" << begint << ")不能与起始柱(" << begint << ")相同\n";
            else
                break;
        }
    }
    if (opt == 4 || opt == 8) {
        while (1) {
            cout << "请输入移动速度(0-5: 0-按回车单步演示 1-延时最长 5-延时最短)\n";
            std::cin >> tim;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(1000000, '\n');
                continue;
            }
            if (tim >= 0 && tim <= 5)
                break;
        }
    }
    bg = begint - 'A';
    ed = endt - 'A';
}

void clear()
{
    memset(top, 0, sizeof(top));
    memset(stk, 0, sizeof(stk));
    step = 0;
}

void print_abc(int id, int from, int to)
{
    cout << setw(2) << id << "#: " << itou(from) << "-->" << itou(to);
}

void print_step(int id, int from, int to)
{
    cout << "第" << setw(4) << ++step << "步(";
    print_abc(id, from, to);
    cout << ") ";
}

void print_stack()
{
    for (int i = 0; i < 3; i++) {
        cout << ' ' << itou(i) << ":";
        for (int j = 1; j <= top[i]; j++)
            cout << setw(2) << stk[i][j];
        for (int j = top[i]; j <= 10; j++)
            cout << "  ";
    }
    puts("");
}

void draw_line(int opt, int id, int from, int to)
{
    if (opt == 1)
        print_abc(id, from, to);
    else
        print_step(id, from, to);
}

void draw_column()
{
    cct_gotoxy(X_A_COL, Y_COL_BOTTOM);
    cct_showch(X_A_COL - COL_BOTTOM_LENGTH / 2, Y_COL_BOTTOM, ' ', COLOR_HYELLOW, COLOR_WHITE, COL_BOTTOM_LENGTH);
    cct_gotoxy(X_B_COL, Y_COL_BOTTOM);
    cct_showch(X_B_COL - COL_BOTTOM_LENGTH / 2, Y_COL_BOTTOM, ' ', COLOR_HYELLOW, COLOR_WHITE, COL_BOTTOM_LENGTH);
    cct_gotoxy(X_C_COL, Y_COL_BOTTOM);
    cct_showch(X_C_COL - COL_BOTTOM_LENGTH / 2, Y_COL_BOTTOM, ' ', COLOR_HYELLOW, COLOR_WHITE, COL_BOTTOM_LENGTH);
    for (int i = 1; i <= COL_HEIGHT; i++) {
        cct_showch(X_A_COL, Y_COL_BOTTOM - i, ' ', COLOR_HYELLOW);
        Sleep(SMALL_GAP);
        cct_showch(X_B_COL, Y_COL_BOTTOM - i, ' ', COLOR_HYELLOW);
        Sleep(SMALL_GAP);
        cct_showch(X_C_COL, Y_COL_BOTTOM - i, ' ', COLOR_HYELLOW);
        Sleep(SMALL_GAP);
    }
    set_color_init();
}

void draw_init_plate(int bg)
{
    for (int j = 1; j <= top[bg]; j++) {
        cct_showch(X_COL[bg] - stk[bg][j], Y_COL_BOTTOM - j, ' ', stk[bg][j], COLOR_WHITE, getlen(stk[bg][j]));
        Sleep(SMALL_GAP);
    }
    set_color_init();
}

void draw_bottom(const int Y_BOTTOM, int bg)
{
    cct_showch(X_LEFT_BOT, Y_BOTTOM, '=', 0, 7, BOT_BOTTOM_LENGTH);
    cct_showch(X_A_BOT, Y_BOTTOM + 1, 'A');
    cct_showch(X_B_BOT, Y_BOTTOM + 1, 'B');
    cct_showch(X_C_BOT, Y_BOTTOM + 1, 'C');
    for (int j = 1; j <= top[bg]; j++) {
        showNum(X_BOT[bg] - 1, Y_BOTTOM - j, stk[bg][j], 2);
    }
}

void draw_canvas(int n, int bg, int opt, int ed, int tim)
{
    gap = get_gap(tim);
    cct_setcursor(CURSOR_INVISIBLE);
    if (opt <= 3)
        return;
    cct_cls();
    cout << "从 " << itou(bg) << " 移动到 " << itou(ed) << "，共 " << n << " 层，延时设置为 " << tim << '\n';
    if (opt != 4) {
        draw_column();
        draw_init_plate(bg);
    }
    draw_bottom(Y_BOT_BOTTOM + BOT_BIAS * (opt != 4), bg);
    Sleep(1000);
    cct_setcursor(CURSOR_VISIBLE_NORMAL);
}

void move_bot(const int Y_BOT, int from, int to)
{
    draw(X_BOT[from], Y_BOT - top[from], ' ');
    draw(X_BOT[from] - 1, Y_BOT - top[from], ' ');
    showNum(X_BOT[to] - 1, Y_BOT - top[to] - 1, stack_top(from), 2);
}

void move_updown(int x, int y, int p, int d)
{
    cct_showch(x - p, y, ' ', 0, 7, getlen(p));
    cct_showch(x - p, y + d, ' ', p, 7, getlen(p));
    cct_showch(x - p - 1, y, ' ');
    cct_showch(x + p + 1, y, ' ');
    cct_showch(x - p - 1, y + d, ' ');
    cct_showch(x + p + 1, y + d, ' ');
}

void move_lr(int from, int to, int y, int p)
{
    int d, dadd, dmin;
    if (from > to)
        d = -1, dadd = -p - 1, dmin = p;
    else
        d = 1, dadd = p + 1, dmin = -p;
    for (int x = from; x != to; x += d) {
        cct_showch(x + dadd, y, ' ', p);
        cct_showch(x + dmin, y, ' ');
        cct_showch(x + dmin - d, y, ' ');
        cct_showch(x + dadd + d, y, ' ');
        wait_gap(1);
    }
}

void move_slide(int from, int to)
{
    int nowx = X_COL[from];
    int nowy = Y_COL_BOTTOM - top[from];
    int p = stack_top(from);
    while (nowy >= Y_COL_BOTTOM - COL_HEIGHT) {
        move_updown(nowx, nowy, p, -1);
        cct_showch(nowx, nowy, ' ', COLOR_HYELLOW);
        wait_gap(1);
        --nowy;
    }
    move_updown(nowx, nowy, p, -1);
    wait_gap(1);
    --nowy;
    move_lr(nowx, X_COL[to], nowy, p);
    nowx = X_COL[to];
    move_updown(nowx, nowy, p, 1);
    wait_gap(1);
    ++nowy;
    move_updown(nowx, nowy, p, 1);
    wait_gap(1);
    ++nowy;
    while (nowy < Y_COL_BOTTOM  - top[to] - 1) {
        move_updown(nowx, nowy, p, 1);
        cct_showch(nowx, nowy, ' ', COLOR_HYELLOW);
        wait_gap(1);
        ++nowy;
    }
    set_color_init();
}

void move(int from, int to, int opt)
{
    cct_setcursor(CURSOR_INVISIBLE);
    if (opt <= 3) {
        draw_line(opt, stack_top(from), from, to);
        stack_push(to, stack_top(from));
        stack_pop(from);
        if (opt == 3)
            print_stack();
        else
            puts("");
        return;
    }
    int YB = Y_BOT_BOTTOM + BOT_BIAS * (opt != 4);
    cct_gotoxy(3, YB + 3);
    draw_line(opt, stack_top(from), from, to);

    stack_push(to, stack_top(from));
    stack_pop(from);
    cct_gotoxy(25, YB + 3);
    print_stack();
    stack_push(from, stack_top(to));
    stack_pop(to);

    move_bot(YB, from, to);
    if(opt != 4)
        move_slide(from, to);

    stack_push(to, stack_top(from));
    stack_pop(from);
    cct_setcursor(CURSOR_VISIBLE_NORMAL);
    wait_gap(0);
}

void hanoi(int n, int src, int tmp, int dst, int opt)
{
    if (n == 1) {
        move(src, dst, opt);
        return;
    }
    hanoi(n - 1, src, dst, tmp, opt);
    move(src, dst, opt);
    hanoi(n - 1, tmp, src, dst, opt);
}

void process_opt5()
{
    cct_setcursor(CURSOR_INVISIBLE);
    cct_cls();
    draw_column();
    cct_gotoxy(0, 25);
    cct_setcursor(CURSOR_VISIBLE_NORMAL);
    wait_endl("\n按回车键继续 ");
}

void process_opt6(int bg) 
{
    cct_setcursor(CURSOR_INVISIBLE);
    cct_cls();
    draw_column();
    draw_init_plate(bg);
    cct_gotoxy(0, 25);
    cct_setcursor(CURSOR_VISIBLE_NORMAL);
    wait_endl("\n按回车键继续 ");
}

void process_opt7(int n, int bg, int md, int ed)
{
    cct_setcursor(CURSOR_INVISIBLE);
    cct_cls();
    draw_column();
    draw_init_plate(bg);
    Sleep(1000);
    gap = 100;
    if (n % 2)
        move_slide(bg, ed);
    else
        move_slide(bg, md);
    cct_gotoxy(0, 25);
    cct_setcursor(CURSOR_VISIBLE_NORMAL);
    wait_endl("\n按回车键继续 ");
}

bool check(int n, int x)
{
    for (int i = 1; i <= n; i++) {
        if (stk[x][i] != n - i + 1)
            return 0;
    }
    return 1;
}

void game_mode(int n, int ed)
{
    gap = SMALL_GAP;
    cct_gotoxy(0, 37);
    cout << "请输入移动的柱号(命令形式：AC=A顶端的盘子移动到C，Q=退出) ：";
    char s[20] = { 0 };
    int nowx, nowy;
    while (1) {
        cct_getxy(nowx, nowy);
        cct_setcursor(CURSOR_VISIBLE_NORMAL);
        char c = _getch();
        int len = 0;
        while (c != '\r' && c != '\n' && len <= 15) {
            if(c < 128 && c > 10)
                cout << c;
            s[++len] = c;
            c = _getch();
        }
        if (len == 1 && (s[1] == 'q' || s[1] == 'Q'))
            break;
        if (len == 2) {
            if (s[1] >= 'a' && s[1] <= 'c')
                s[1] = s[1] - 'a' + 'A';
            if (s[1] >= 'A' && s[1] <= 'C') {
                if (s[2] >= 'a' && s[2] <= 'c')
                    s[2] = s[2] - 'a' + 'A';
                if (s[2] >= 'A' && s[2] <= 'C') {
                    int from = s[1] - 'A';
                    int to = s[2] - 'A';
                    if (from == to) {
                        cout << "\n错误输入!";
                        Sleep(1500);
                    }
                    else if (stack_top(from) == 0) {
                        cout << "\n此柱为空!";
                        Sleep(1500);
                    }
                    else if (stack_top(to) != 0 && stack_top(from) > stack_top(to)) {
                        cout << "\n非法移动!";
                        Sleep(1500);
                    }
                    else {
                        cct_setcursor(CURSOR_INVISIBLE);
                        move(from, to, 9);
                        if (check(n, ed))
                            break;
                    }
                }
            }
        }
        cct_showch(nowx, nowy, ' ', 0, 7, 20);
        cct_showch(0, nowy + 1, ' ', 0, 7, 20);
        cct_gotoxy(nowx, nowy);
    }
    cct_gotoxy(nowx, nowy);
    cout << "\n游戏结束!";
    wait_endl("\n按回车键继续 ");
}