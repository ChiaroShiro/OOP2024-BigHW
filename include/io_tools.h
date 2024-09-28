/* 2351871 郎若谷 计科 */


/*
 *
 * io_tools.h
 *
 * 提供了常用的IO相关函数
 * 以及简单的通用函数
 *
 */

#pragma once

/*
 * 判断是否是大写字母
 */
bool isUpper(char c);

/*
 * 判断是否是小写字母
 */
bool isLower(char c);

/*
 * 判断是否是数字字符
 */
bool isNumber(char c);

/*
 * 大写字母转小写
 */
char utol(char c);

/*
 * 小写字母转大写
 */
char ltou(char c);

/*
 * 数字转小写字母
 */
char itol(int a);

/*
 * 数字转大写字母
 */
char itou(int a);

/*
 * 读入一个字符，根据参数决定是否回显
 * 默认不显示
 */
char getcc(bool show = 0);

/*
 * 等待x毫秒，若x=0则不等待
 */
void wait(int x);

/*
 * 在 (x,y) 坐标显示数字 v，显示宽度为 w（默认为 1）
 */
void showNum(int x, int y, int v, int w = 1);

/*
 * 若传入两个参数则是在 (x,y) 坐标开始清空一行，清空后的内容可以指定，默认为空白
 * 若传入一个参数则把那一行直接清空，清空后的内容可以指定，默认为空白
 */
void clearLine(int x, int y = -1, const char* s = "                                   ");

/*
 * 输出一行内容后等待用户输入指定内容，有错误提示
 * x: 输出后的暂停时间
 * output: 输出的提示内容
 * hint: 输入错误后的提示内容
 * input: 指定要求输入的内容
 * show: 输入是否回显，默认回显
 */
void waitLine(int x, const char* output, const char* hint = "", const char* input = "", bool show = 1);

/*
 * 输出一行内容后等待用户输入指定字符
 * x: 输出后的暂停时间
 * output: 输出的提示内容
 * input: 指定要求输入的字符
 * show: 输入是否回显，默认不回显
 */
void waitLine(int x, const char* output, char input, bool show = 0);

/*
 * 在(x,y)位置输出一段字符串
 */
void showLine(const char* s, int x, int y);

/*
 * 在(x,y)位置输出一段字符串，并把输出结束后的位置存入 (retx, rety) 中
 * 若不指定xy则默认在当前光标位置输出
 */
void showLine(const char* s, int* retx, int* rety, int x = -1, int y = -1);

/*
 * 读入一个指定范围的数字，可以处理超出范围已经各种不合法输入的情况
 * 指定范围是 [minli, maxli]
 * 输出提示信息为 msg
 */
int getLineNumber(int minli, int maxli, const char* msg);

/*
 * 程序结束函数
 */
void programExit();

/*
 * 等待回车换左键单击
 */
void waitForEndlineOrClick();

/*
 * 在二维动画的底部输出文字
 */
void gotoBottom(int n, int showBorder);
void showBottom(int n, const char* s, int showBorder, int bgcol = COLOR_BLACK, int frcol = COLOR_WHITE);



/*
 * 在当前光标位置以当前颜色输出
 */
void shows(const char* s);
void showc(const char& c);
void showi(const int& p);
void showln();