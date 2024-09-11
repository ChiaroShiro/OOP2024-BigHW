/* 2351871 ¿…»Ùπ» º∆ø∆ */
#pragma once

#define X_A_COL 13
#define X_B_COL 43
#define X_C_COL 73
#define X_A_BOT 7
#define X_B_BOT 12
#define X_C_BOT 17
#define X_LEFT_BOT 5
#define X_RIGHT_BOT 19

#define Y_COL_BOTTOM 17
#define Y_BOT_BOTTOM 17

#define COL_HEIGHT 13
#define COL_BOTTOM_LENGTH 25
#define BOT_BOTTOM_LENGTH 15
#define BOT_BIAS 15

#define SMALL_GAP 50

void stack_build(int id, int n);
void stack_push(int id, int x);
void stack_pop(int id);
int stack_top(int id);

void wait_endl(const char s[]);
int get_third_number(int a, int b);
void clear();
int get_gap(int t);

void input_data(int opt, int& n, int& bg, int& ed, int& tim);
void draw_canvas(int n, int bg, int opt, int ed, int tim);
void hanoi(int n, int src, int tmp, int dst, int opt);
void game_mode(int n, int ed);
void process_opt5();
void process_opt6(int bg);
void process_opt7(int n, int bg, int md, int ed);