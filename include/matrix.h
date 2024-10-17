/*2351871 郎若谷 计科*/


/*
 * 
 * matrix.h
 * 
 * 提供了二维矩阵游戏中通用的操作函数和动画函数
 * 
 */

#include "../include/cmd_gmw_tools.h"

#pragma once

#define STA_NORMAL			0
#define STA_NEED_DEL		1
#define STA_NOW_DEL			2
#define STA_SWAP			3
#define STA_VOID			4
#define STA_NEW				5
#define STA_CHOOSE			6

#define MAP_SIZE			20

#define BUP					0
#define BMID				1
#define BDOWN				2
 
#define CHOLLOW				0
#define CSOLID				1
#define CDOUBLE				2
#define CFOUR				3
#define CVOID				4


const int forwardx[] = { 0, 0, 1, -1 };
const int forwardy[] = { 1, -1, 0, 0 };

const char* const DOUBLE_HEAD[] = {"╔", "╠","╚"};
const char* const DOUBLE_TRAN[] = { "╦", "╬", "╩" };
const char* const DOUBLE_TAIL[] = { "╗","╣","╝" };
const char* const DOUBLE_LINE = "═";
const char* const DOUBLE_VERT = "║";

const char* const SINGLE_HEAD[] = { "┏", "┣","┗" };
const char* const SINGLE_TRAN[] = { "┳", "╋", "┻" };
const char* const SINGLE_TAIL[] = { "┓","┫","┛" };
const char* const SINGLE_LINE = "━";
const char* const SINGLE_VERT = "┃";

const char* const CORE[] = { "〇","●","◎","¤","  " };

class StyleCSS {
private:
	const char* head[3];	// 行首
	const char* tran[3];	// 中间段
	const char* tail[3];	// 行尾
	const char* line;		// 行横
	const char* vert;		// 列竖
	int now;				// 现在要输出哪种格式

public:
	StyleCSS();
	void setHead(const char* const news[]);
	void setTail(const char* const news[]);
	void setTran(const char* const news[]);
	void setLine(const char* const news);
	void setVert(const char* const news);
	void setOutput(int kind);
	const char* getHead();
	const char* getTail();
	const char* getLine();
	const char* getTran();
	const char* getVert();
};

/*
 * 计算某个元素在屏幕上的位置
 * 计算元素 (i,j) 在屏幕上进行输出的起始位置，坐标存储在 (x,y) 中，x 是行数，y 是列数
 * showBorder 为 1 则是有边界，为 0 则是无边界
 * cn 和 cm 是每个元素的行数和列数
 */
void getpos(int i, int j, int* x, int* y, int addx, int addy, int cn, int cm);
void rgetpos(int i, int j, int* x, int* y, int addx, int addy, int cn, int cm);

/*
 * 随机生成一个 n*m 矩阵，存储在 map 中
 * 每个元素的值域为 [1, cates]
 */
void generate(int n, int m, int map[][MAP_SIZE], int cates);

/*
 * 清空 n*m 的矩阵 sta
 * 所有元素变成 x
 */
void clearStatus(int n, int m, int sta[][MAP_SIZE], int x, int beginpos = 1);

/*
 * 将 n*m 矩阵中 sta 的 ori 全部换成 trans
 */
void TransferStatus(int n, int m, int sta[][MAP_SIZE], int ori, int trans, int beginpos = 1);

/*
 * 在 n*m 的矩阵中从 (x,y) 点开始向四周寻找颜色（map值）相同且状态（sta值）被标记为 STA_NEED_DEL 的元素
 * 并将该元素删除，状态（sta）随后被置为 STA_NOW_DEL
 * 删除动画调用 delBall(x, y, col) 函数
 * 
 * delBall函数前两个参数描述被删位置的坐标(x,y)，第三个参数为这个位置的颜色（map值）
 */
void dfsFindBlock(int x, int y, int n, int m, int showBorder, const int map[][MAP_SIZE], int sta[][MAP_SIZE], 
	              void (*delBall)(int, int, int, int));

void dfsFindBlock(int x, int y, CONSOLE_GRAPHICS_INFO* const pCGI, const int map[][MAP_SIZE], int sta[][MAP_SIZE], void (*delBall)(CONSOLE_GRAPHICS_INFO* const, int, int, int));

/*
 * 在 n* m 的矩阵中，寻找所有状态（sta值）为 STA_NEED_DEL 的同色连通块，并依次删除
 * sta 置为 STA_NOW_DEL
 * 如果 showGraph 是 1 则会调用 delBall 参数显示动画效果
 * 
 * n, m: 矩阵大小
 * map: 矩阵每个元素的颜色；  sta: 矩阵每个元素的状态
 * showGraph: 是否调用动画效果，默认为0（不调用）
 * delBall(x, y, col): 删除元素的动画函数。前两个参数描述被删除元素的坐标 (x, y)，第三个参数描述被删除元素的颜色 col (map值)
 */
void deleteBall(int n, int m, int showBorder, int map[][MAP_SIZE], int sta[][MAP_SIZE], bool showGraph = 0, void (*delBall)(int, int, int, int) = NULL);

void deleteBall(CONSOLE_GRAPHICS_INFO* const pCGI, int map[][MAP_SIZE], int sta[][MAP_SIZE], void (*delBall)(CONSOLE_GRAPHICS_INFO* const, int, int, int));

/*
 * 将所有被删除元素 (sta 为 STA_NOW_DEL) 上方的元素下移至最高的没有被删除的元素上方
 * 随后把 sta 置为 STA_VOID
 * 如果 showGraph 是 1 则会调用 slideDownBall 参数显示动画效果
 * 
 * n, m: 矩阵大小
 * map: 矩阵每个元素的颜色；  sta: 矩阵每个元素的状态
 * showGraph: 是否调用动画效果，默认为0（不调用）
 * slideDownBall(n, m, x, y, col): 前两个参数n, m描述矩阵大小，三四参数描述被操作元素的坐标 (x, y)，col 为该元素的颜色。函数效果是显示将这个位置的元素向下移动一格的动画
 */
void fallBall(int n, int m, int map[][MAP_SIZE], int sta[][MAP_SIZE], bool showGraph = 0, int showBorder = 1, void (*slideDownBall)(int, int, int, int, int, int) = NULL);

/*
 * n*m 的矩阵，颜色存在 map 中，状态存在 sta 中
 * 函数作用是将所有状态是 STA_VOID 的位置填上新球
 * 球的颜色种类是 cates
 * 如果 showGraph 是 1 则动画显示球下落过程，需要函数 slideDownBall
 * slideDownBall(n, m, x, y, col): 前两个参数n, m描述矩阵大小，三四参数描述被操作元素的坐标 (x, y)，col 为该元素的颜色。函数效果是显示将这个位置的元素向下移动一格的动画
 */
int fillVoidBall(int n, int m, int map[][MAP_SIZE], int sta[][MAP_SIZE],
	int cates, void (*slideDownBall)(int, int, int, int, int, int), bool showGraph = 1, int showBorder = 1);

/*
 * pCGI 存所有信息
 * map存颜色，sta存状态
 * cates是球的种类
 * slideDownBall是下移一个球的函数
 * 第一个参数是pCGI，第二三参数是坐标，第四个参数是颜色
 */
int fillVoidBall(CONSOLE_GRAPHICS_INFO* const pCGI, int map[][MAP_SIZE], int sta[][MAP_SIZE],
	int cates, void (*slideDownBall)(CONSOLE_GRAPHICS_INFO* const, int, int, int));

/*
 * pCGI存框架所有信息
 * map存颜色，sta存状态，slideDownBall 是下坠一格的函数
 * slideDownBall(CGI* const pCGI, int x, int y, int val)
 * pCGI存所有信息，完成从(x,y)往下的一格坠落操作，颜色是 val
 */
void fallBall(CONSOLE_GRAPHICS_INFO* const pCGI, int map[][MAP_SIZE], int sta[][MAP_SIZE], void (*slideDownBall)(CONSOLE_GRAPHICS_INFO* const, int, int, int));

/*
 * 绘制背景框架，矩阵为 n*m 大小，showBorder 为 1 则绘制边框
 * totx 和 toty 存储重新变化后的屏幕大小
 * coren 和 corem 描述每个元素的大小
 * gap 为绘制每个元素后的间隔
 */
void drawBackground(int n, int m, bool showBorder, int showFrame, int* totx, int* toty,
	int coren, int corem, StyleCSS style, int gap = 0, int times = 1);

/*
 * 无动画选项下的文字绘画矩阵功能
 * n*m的矩阵 map 和 sta ，s 为画表的头部
 */
void drawCanvas(int n, int m, const int map[][MAP_SIZE], const int sta[][MAP_SIZE], const char* s, int colorTag = -1);