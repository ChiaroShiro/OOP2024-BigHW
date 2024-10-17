/* 2351871 郎若谷 计科 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <climits>
#include <cstring>
#include <conio.h>
#include <windows.h>
#include "../include/cmd_console_tools.h"
#include "../include/cmd_gmw_tools.h"
using namespace std;

/* --------------------------------------------------
		此处可以给出需要的静态全局变量（尽可能少，最好没有）、静态全局只读变量/宏定义（个数不限）等
   -------------------------------------------------- */

#define debug 			0

/* --------------------------------------------------
		此处可以给出需要的内部辅助工具函数
		1、函数名不限，建议为 gmw_inner_* 
		2、个数不限
		3、必须是static函数，确保只在本源文件中使用
   -------------------------------------------------- */

const char* const DEFAULT_TAB[5][11] = {
	{},
	{"╔", "╚", "╗", "╝", "═", "║", "╦", "╩", "╠", "╣", "╬"},
	{"┏", "┗", "┓", "┛", "━", "┃", "┳", "┻", "┣", "┫", "╋"},
	{"╒", "╘", "╕", "╛", "═", "│", "╤", "╧", "╞", "╡", "╪"},
	{"╓", "╙", "╖", "╜", "─", "║", "╥", "╨", "╟", "╢", "╫"}
}; // 1 - 全线 2 - 全单线 3 - 横双竖单 4 - 横单竖双

/*
 * 得到色块 (row, col) 的左上角坐标 (x, y)
 */
static void getBlockXY(const CONSOLE_GRAPHICS_INFO *const pCGI, const int row, const int col, int &x, int &y)
{
	x = pCGI->frame_x + 2 + col * pCGI->CFI.bwidth;
	y = pCGI->frame_y + 1 + row * pCGI->CFI.bhigh;
}

/*
 * 根据坐标得到属于哪个色块
 * 返回值为是否合法
 * 如果返回 0 且 row col 为 -1 则说明在框架区域外
 * 如果返回 0 且 row col 不为 -1 则说明在某个色块的非法区内
 * 
 * 若有边框，定义一个色块的管辖区域是色块面积及右侧和上侧的边框区域
 * 最上侧和最左侧边界由附近色块管辖
 */
static bool getBlockCoord(const CONSOLE_GRAPHICS_INFO *const pCGI, const int x, const int y, int &row, int &col)
{
	if(pCGI->CFI.separator) { // 因为有无框架情形下对于每个色块的接管面积定义不同，因此需要分开计算
		if(x <= pCGI->frame_x || y <= pCGI->frame_y - 1 || x >= pCGI->frame_x + pCGI->CFI.tot_wid - 1 || y >= pCGI->frame_y + pCGI->CFI.tot_high) {
			row = col = -1; // 框架外非法区域
			return 0;
		}
		int rex = max(x - pCGI->frame_x - 1, 0);
		int rey = max(y - pCGI->frame_y - 1, 0); // 取 max 是为了处理在边界最右侧和最上侧的情况
		col = rex / pCGI->CFI.bwidth;
		row = rey / pCGI->CFI.bhigh;
		getBlockXY(pCGI, row, col, rex, rey); // 取得这个色块的左上角
		return x < rex + pCGI->CFI.block_width && y < rey + pCGI->CFI.block_high;
	}
	else {
		if(x <= pCGI->frame_x + 1 || y <= pCGI->frame_y || x >= pCGI->frame_x + pCGI->CFI.tot_wid - 2 || y >= pCGI->frame_y + pCGI->CFI.tot_high - 1) {
			row = col = -1;
			return 0;
		}
		int rex = max(x - pCGI->frame_x - 2, 0);
		int rey = max(y - pCGI->frame_y - 1, 0);
		col = rex / pCGI->CFI.bwidth;
		row = rey / pCGI->CFI.bhigh;
		return 1;
	}
}

static char getRowNo(int num)
{
	if(num < 26)
		return 'A' + num;
	if(num < 52)
		return 'a' + num - 26;
	return '*';
}

static char* getColNo(int num)
{
	static char s[5];
	memset(s, 0, sizeof(s));
	if(num < 100)
		sprintf(s, "%d", num);
	else 
		s[0] = s[1] = '*';
	return s;
}

static void shows(const char* s) 
{
	int x, y, cx, cy;
	cct_getxy(x, y);
	cct_getcolor(cx, cy);
	cct_showstr(x, y, s, cx, cy);
}

static void showc(const char c) 
{
	int x, y, cx, cy;
	cct_getxy(x, y);
	cct_getcolor(cx, cy);
	cct_showch(x, y, c, cx, cy);
}

static void showi (const int p) 
{
	int x, y, cx, cy;
	cct_getxy(x, y);
	cct_getcolor(cx, cy);
	cct_showint(x, y, p, cx, cy);
}

static void showiLen(const int p, const int len)
{
	int x, y, cx, cy, endx, endy;
	cct_getxy(x, y);
	cct_getcolor(cx, cy);
	cct_showint(x, y, p, cx, cy);
	cct_getxy(endx, endy);
	int n = len - (endx - x);
	while(n --> 0) 
		showc(' ');
}

static void showln()
{
	showc('\n');
}

static void waitFrame(const CONSOLE_GRAPHICS_INFO *const pCGI)
{
	if(pCGI->init_delay_frame && pCGI->delay_of_draw_frame)
		Sleep(pCGI->delay_of_draw_frame);
}

static void waitBlock(const CONSOLE_GRAPHICS_INFO *const pCGI)
{
	if(pCGI->init_delay_block && pCGI->delay_of_draw_block)
		Sleep(pCGI->delay_of_draw_block);
}

static void waitMoved(const CONSOLE_GRAPHICS_INFO *const pCGI)
{
	if(pCGI->init_delay_moved && pCGI->delay_of_block_moved)
		Sleep(pCGI->delay_of_block_moved);
}

static void initCGI(CONSOLE_GRAPHICS_INFO *const pCGI) 
{
	char *ptr = (char*)&pCGI->top_status_line;
	for(int i = 0; i < 4; i++, ptr++) {
		if(*ptr != 0 && *ptr != 1) 
			*ptr = 0;
	}
	ptr = (char*)&pCGI->set_rowcol;
	for(int i = 0; i < 13; i++, ptr++) {
		if(*ptr != 0 && *ptr != 1)
			*ptr = 0;
	}
	// pCGI->set_rowcol = pCGI->set_color = pCGI->set_font = pCGI->set_frame_linetype = pCGI->set_frame_style = 0;
	// pCGI->set_frame_color = pCGI->set_block_border_switch = pCGI->set_status_line_color = 0;
	// pCGI->have_set_rowcol = pCGI->have_set_blocksize = 0;

	// pCGI->top_status_line = pCGI->lower_status_line = 0;
	// pCGI->draw_frame_with_row_no = pCGI->draw_frame_with_col_no = 0;
	// pCGI->delay_of_draw_frame = pCGI->delay_of_draw_block = pCGI->delay_of_block_moved = 0;

	// pCGI->extern_down_lines = pCGI->extern_left_cols = pCGI->extern_right_cols = pCGI->extern_up_lines = 0;
}

static bool __debugCheckGWMInit(const CONSOLE_GRAPHICS_INFO *const pCGI) 
{
	bool flag = 0;
	if(!pCGI->set_rowcol) {
		flag = 1;
		shows("<ERROR>: no set_rowcol\n");
	}
	if(!pCGI->set_color) {
		flag = 1;
		shows("<ERROR>: no set_color\n");
	}
	if(!pCGI->set_font) {
		flag = 1;
		shows("<ERROR>: no set_font\n");
	}
	if(!pCGI->set_frame_linetype) {
		flag = 1;
		shows("<ERROR>: no set_frame_linetype\n");
	}
	if(!pCGI->set_frame_style) {
		flag = 1;
		shows("<ERROR>: no set_frame_style\n");
	}
	if(!pCGI->set_frame_color) {
		flag = 1;
		shows("<ERROR>: no set_frame_color\n");
	}
	if(!pCGI->set_block_border_switch) {
		flag = 1;
		shows("<ERROR>: no set_block_border_switch\n");
	}
	if(!pCGI->set_status_line_color) {
		flag = 1;
		shows("<ERROR>: no set_status_line_color\n");
	}
	if(!flag) {
		shows("FINISH CHECKING: NO ERROR\n");
	}
	return flag;
}

static void __debugPrint(const char* s, int x = 0, int y = 0)
{
	int orx, ory;
	cct_getxy(orx, ory);
	cct_gotoxy(x, y);
	shows(s);
	showln();
	cct_gotoxy(orx, ory);
}

static BLOCK_DISPLAY_INFO findBDIBlock(const BLOCK_DISPLAY_INFO *const bdi, const int bdi_value)
{
	int i = 0;
	while(bdi[i].value != BDI_VALUE_END) {
		if(bdi[i].value == bdi_value)
			break;
		i++;
	}
	return bdi[i];
}

/*
 *  赋值 2 字节
 *  若 src 是 NULL 则赋 "  "
 *  少于 2 字节补全空格
 */
static void fillTabString(char* dest, const char* src)
{
	if(src == NULL) {
		strcpy(dest, "  ");
		return;
	}
	if(*src) 
		*dest = *src;
	else 
		*dest = ' ';
	++src;
	++dest;
	if(*src) 
		*dest = *src;
	else 
		*dest = ' ';
}

static void update_tothw(CONSOLE_GRAPHICS_INFO *const pCGI)
{ // 更新框架（仅框线部分）总大小
	pCGI->CFI.tot_high = pCGI->row_num * pCGI->CFI.block_high;
	pCGI->CFI.tot_high += (pCGI->row_num - 1) * pCGI->CFI.separator + 2;
	pCGI->CFI.tot_wid = pCGI->col_num * pCGI->CFI.block_width;
	pCGI->CFI.tot_wid += ((pCGI->col_num - 1) * pCGI->CFI.separator + 2) * 2;
}

static void update_linecol(CONSOLE_GRAPHICS_INFO *const pCGI)
{ // 更新 cmd 窗口大小
	pCGI->lines = pCGI->extern_up_lines + pCGI->extern_down_lines + pCGI->CFI.tot_high + 
				  pCGI->top_status_line + pCGI->lower_status_line + pCGI->draw_frame_with_col_no + 4;
	pCGI->cols  = pCGI->CFI.tot_wid + pCGI->extern_left_cols + pCGI->extern_right_cols + pCGI->draw_frame_with_row_no * 2 + 1;
}

static void update_startxy(CONSOLE_GRAPHICS_INFO *const pCGI)
{ // 更新主体部分（框线 + 标号）起始位置（左上角）
	pCGI->start_x = pCGI->extern_left_cols;
	pCGI->start_y = pCGI->extern_up_lines + pCGI->top_status_line;
}

static void update_framexy(CONSOLE_GRAPHICS_INFO *const pCGI)
{ // 更新框架（仅框线）部分起始位置（左上角）
	pCGI->frame_x = pCGI->extern_left_cols + pCGI->draw_frame_with_row_no * 2;
	pCGI->frame_y = pCGI->extern_up_lines + pCGI->top_status_line + pCGI->draw_frame_with_col_no;
}

static void update_sliwid(CONSOLE_GRAPHICS_INFO *const pCGI)
{ // 更新状态栏长度
	pCGI->SLI.width = pCGI->cols - pCGI->extern_left_cols - pCGI->extern_right_cols;
}

static void update_status(CONSOLE_GRAPHICS_INFO *const pCGI)
{ // 更新状态栏位置
	pCGI->SLI.top_start_x = pCGI->extern_left_cols;
	pCGI->SLI.top_start_y = pCGI->extern_up_lines;
	pCGI->SLI.lower_start_x = pCGI->extern_left_cols;
	pCGI->SLI.lower_start_y = pCGI->extern_up_lines + pCGI->CFI.tot_high + pCGI->top_status_line + pCGI->draw_frame_with_col_no;
}

static void updateAll(CONSOLE_GRAPHICS_INFO *const pCGI)
{ // 全部更新
	update_tothw(pCGI);
	update_linecol(pCGI);
	update_framexy(pCGI);
	update_startxy(pCGI);
	update_sliwid(pCGI);
	update_status(pCGI);
}

/*
 * 画一实心行
 * 左侧头部字符是 left，右侧尾部字符是 right，中间穿插分隔符是 mid
 */
static void drawOneSolidLine(const CONSOLE_GRAPHICS_INFO *const pCGI, const char* left, const char* right, const char* mid)
{
	const CONSOLE_FRAME_INFO* const pc = &pCGI->CFI;
	shows(left);
	waitFrame(pCGI);
	for(int i = 0; i < pCGI->col_num; i++) {
		for(int j = 0; j < pc->block_width; j += 2)
			shows(pc->h_normal);
		waitFrame(pCGI);
		if(pc->separator && i < pCGI->col_num - 1) {
			shows(mid);
		}
	}
	shows(right);
	waitFrame(pCGI);
	showln();
}

/*
 * 画一空心行
 * 左侧头部字符是 left，右侧尾部字符是 right，中间穿插分隔符是 mid
 */
static void drawOneHollowLine(const CONSOLE_GRAPHICS_INFO *const pCGI, const char* left, const char* right, const char* mid)
{
	const CONSOLE_FRAME_INFO* const pc = &pCGI->CFI;
	shows(left);
	waitFrame(pCGI);
	for(int i = 0; i < pCGI->col_num; i++) {
		for(int j = 0; j < pc->block_width; j += 2)
			shows("  ");
		waitFrame(pCGI);
		if(pc->separator && i < pCGI->col_num - 1) {
			shows(mid);
		}
	}
	shows(right);
	waitFrame(pCGI);
	showln();
}

/*
 * 画列标号
 */
static void drawColNoid(const CONSOLE_GRAPHICS_INFO *const pCGI)
{
	if(pCGI->draw_frame_with_col_no) {
		for(int i = 0; i < pCGI->col_num; i++) {
			cct_gotoxy(pCGI->start_x + 2 + 2 * pCGI->draw_frame_with_row_no + pCGI->CFI.bwidth * i + (pCGI->CFI.block_width + 1) / 2 - 1, pCGI->start_y);
			shows(getColNo(i));
		}
	}
}

/*
 * 画行标号
 */
static void drawRowNoid(const CONSOLE_GRAPHICS_INFO *const pCGI)
{
	if(pCGI->draw_frame_with_row_no) {
		for(int i = 0; i < pCGI->row_num; i++) {
			cct_gotoxy(pCGI->start_x, pCGI->start_y + 1 + pCGI->draw_frame_with_col_no + (pCGI->CFI.block_high + 1) / 2 + pCGI->CFI.bhigh * i - 1);
			showc(getRowNo(i));
		}
	}
}

/* ----------------------------------------------- 
		实现下面给出的函数（函数声明不准动）
   ----------------------------------------------- */
/***************************************************************************
  函数名称：
  功    能：设置游戏主框架的行列数
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int row						：行数(错误则为0，不设上限，人为保证正确性)
			const int col						：列数(错误则为0，不设上限，人为保证正确性)
  返 回 值：
  说    明：1、指消除类游戏的矩形区域的行列值
            2、行列的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_rowcol(CONSOLE_GRAPHICS_INFO *const pCGI, const int row, const int col)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	initCGI(pCGI);
	if (pCGI->inited != CGI_INITED)
		return -1;
	pCGI->set_rowcol = 1;
	pCGI->row_num = max(0, row);
	pCGI->col_num = max(0, col);
	pCGI->have_set_rowcol = 1;
	updateAll(pCGI);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置整个窗口（含游戏区、附加区在内的整个cmd窗口）的颜色
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int bg_color					：前景色（缺省COLOR_BLACK）
		   const int fg_color					：背景色（缺省COLOR_WHITE）
		   const bool cascade					：是否级联（缺省为true-级联）
  返 回 值：
  说    明：1、cascade = true时
				同步修改游戏主区域的颜色
				同步修改上下状态栏的正常文本的背景色和前景色，醒目文本的背景色（前景色不变）
			2、不检查颜色值错误及冲突，需要人为保证
				例：颜色非0-15
				    前景色背景色的值一致导致无法看到内容
					前景色正好是状态栏醒目前景色，导致无法看到醒目提示
					...
***************************************************************************/
int gmw_set_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int bgcolor, const int fgcolor, const bool cascade)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	pCGI->set_color = 1;
	pCGI->area_bgcolor = bgcolor;
	pCGI->area_fgcolor = fgcolor;
	if(cascade) {
		pCGI->set_frame_color = pCGI->set_status_line_color = 1;
		pCGI->SLI.top_normal_bgcolor = pCGI->SLI.lower_normal_bgcolor = bgcolor;
		pCGI->SLI.top_normal_fgcolor = pCGI->SLI.lower_normal_fgcolor = fgcolor;
		pCGI->SLI.top_catchy_bgcolor = pCGI->SLI.lower_catchy_bgcolor = bgcolor;
		if(pCGI->SLI.top_catchy_fgcolor < 0 || pCGI->SLI.top_catchy_fgcolor > 15)
			pCGI->SLI.top_catchy_fgcolor = pCGI->SLI.lower_catchy_fgcolor = COLOR_HYELLOW;
		pCGI->CFI.bgcolor = bgcolor;
		pCGI->CFI.fgcolor = fgcolor;
	}
	updateAll(pCGI);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置窗口的字体
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const char *fontname					：字体名称（只能是"Terminal"和"新宋体"两种，错误则返回-1，不改变字体）
		   const int fs_high					：字体高度（缺省及错误为16，不设其它限制，人为保证）
		   const int fs_width					：字体高度（缺省及错误为8，不设其它限制，人为保证）
  返 回 值：
  说    明：1、与cmd_console_tools中的setfontsize相似，目前只支持“点阵字体”和“新宋体”
            2、若设置其它字体则直接返回，保持原字体设置不变
***************************************************************************/
int gmw_set_font(CONSOLE_GRAPHICS_INFO *const pCGI, const char *fontname, const int fs_high, const int fs_width)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	if(strcmp(fontname, "Terminal") && strcmp(fontname, "新宋体"))
		return -1;
	pCGI->set_font = 1;
	strcpy(pCGI->CFT.font_type, fontname);
	if(strcmp(fontname, "新宋体") == 0) {
		pCGI->CFT.font_size_high = fs_high;
		pCGI->CFT.font_size_width = 0;
	}
	else {
		pCGI->CFT.font_size_width = fs_width;
		pCGI->CFT.font_size_high = fs_high;
	}
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置延时
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int type						：延时的类型（目前为3种）
		   const int delay_ms					：以ms为单位的延时
			   画边框的延时：0 ~ 不设上限，人为保证正确（<0则置0）
			   画色块的延时：0 ~ 不设上限，人为保证正确（<0则置0）
			   色块移动的延时：BLOCK_MOVED_DELAY_MS ~ 不设上限，人为保证正确（ <BLOCK_MOVED_DELAY_MS 则置 BLOCK_MOVED_DELAY_MS）
  返 回 值：
  说    明：
***************************************************************************/
int gmw_set_delay(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const int delay_ms)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	if (type == DELAY_OF_DRAW_FRAME) {
		pCGI->delay_of_draw_frame = delay_ms;
		pCGI->init_delay_frame = 1;
	}
	else if (type == DELAY_OF_DRAW_BLOCK) {
		pCGI->delay_of_draw_block = delay_ms;
		pCGI->init_delay_block = 1;
	}
	else if (type == DELAY_OF_BLOCK_MOVED) {
		pCGI->delay_of_block_moved = delay_ms;
		pCGI->init_delay_moved = 1;
	}
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  输入参数：设置游戏主框架结构之外需要保留的额外区域
  功    能：CONSOLE_GRAPHICS_INFO *const pCGI	：
		   const int up_lines					：上部额外的行（缺省及错误为0，不设上限，人为保证）
		   const int down_lines				：下部额外的行（缺省及错误为0，不设上限，人为保证）
		   const int left_cols					：左边额外的列（缺省及错误为0，不设上限，人为保证）
		   const int right_cols				：右边额外的列（缺省及错误为0，不设上限，人为保证）
  返 回 值：
  说    明：额外行列的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_ext_rowcol(CONSOLE_GRAPHICS_INFO *const pCGI, const int up_lines, const int down_lines, const int left_cols, const int right_cols)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	pCGI->extern_up_lines = up_lines;
	pCGI->extern_down_lines = down_lines;
	pCGI->extern_left_cols = left_cols;
	pCGI->extern_right_cols = right_cols;
	updateAll(pCGI);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_FRAME_TYPE 结构中的11种线型（缺省4种）
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int type						：1 - 全线 2 - 全单线 3 - 横双竖单 4 - 横单竖双
  返 回 值：
  说    明：
***************************************************************************/
int gmw_set_frame_default_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const int type)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	pCGI->set_frame_linetype = 1;
	char* ptr = (char*)&pCGI->CFI;
	for (int i = 0; i < 11; i++) {
		strcpy(ptr, DEFAULT_TAB[type][i]);
		ptr += CFI_LEN;
	}
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_FRAME_TYPE 结构中的11种线型
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const char *...						：共11种，具体见.h，此处略
  返 回 值：
  说    明：约定为一个中文制表符，可以使用其它内容，人为保证2字节
			1、超过2字节则只取前2字节
			2、如果给NULL，用两个空格替代
			3、如果给1字节，则补一个空格，如果因此而导致显示乱，不算错
***************************************************************************/
int gmw_set_frame_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const char *top_left, const char *lower_left, const char *top_right,
	const char *lower_right, const char *h_normal, const char *v_normal, const char *h_top_separator,
	const char *h_lower_separator, const char *v_left_separator, const char *v_right_separator, const char *mid_separator)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	pCGI->set_frame_linetype = 1;
	CONSOLE_FRAME_INFO *const p = &pCGI->CFI;
	fillTabString(p->top_left, top_left);
	fillTabString(p->lower_left, lower_left);
	fillTabString(p->top_right, top_right);
	fillTabString(p->lower_right, lower_right);
	fillTabString(p->h_normal, h_normal);
	fillTabString(p->v_normal, v_normal);
	fillTabString(p->h_top_separator, h_top_separator);
	fillTabString(p->h_lower_separator, h_lower_separator);
	fillTabString(p->v_left_separator, v_left_separator);
	fillTabString(p->v_right_separator, v_right_separator);
	fillTabString(p->mid_separator, mid_separator);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_FRAME_TYPE 结构中的色块数量大小、是否需要分隔线等
  输入参数：输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int block_width						：宽度（错误及缺省2，因为约定表格线为中文制表符，如果给出奇数，要+1）
			const int block_high						：高度（错误及缺省1）
			const bool separator						：是否需要分隔线（缺省为true，需要分隔线）
  返 回 值：
  说    明：框架大小/是否需要分隔线等的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_frame_style(CONSOLE_GRAPHICS_INFO *const pCGI, const int block_width, const int block_high, const bool separator)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	pCGI->set_frame_style = 1;
	pCGI->CFI.block_width = block_width + (block_width & 1);
	pCGI->CFI.block_high  = block_high;
	pCGI->CFI.separator   = separator;
	pCGI->CFI.block_width_ext = separator * 2;
	pCGI->CFI.block_high_ext  = separator;
	pCGI->CFI.bwidth = pCGI->CFI.block_width + pCGI->CFI.block_width_ext;
	pCGI->CFI.bhigh  = pCGI->CFI.block_high  + pCGI->CFI.block_high_ext;
	pCGI->have_set_blocksize = 1;
	updateAll(pCGI);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_BORDER_TYPE 结构中的颜色
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int bg_color					：背景色（缺省 -1表示用窗口背景色）
			const int fg_color					：前景色（缺省 -1表示用窗口前景色）
  返 回 值：
  说    明：不检查颜色值错误及冲突，需要人为保证
				例：颜色非0-15，前景色背景色的值一致导致无法看到内容等
***************************************************************************/
int gmw_set_frame_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int bgcolor, const int fgcolor)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	pCGI->set_frame_color = 1;
	pCGI->CFI.bgcolor = (bgcolor == -1 ? pCGI->area_bgcolor : bgcolor);
	pCGI->CFI.fgcolor = (fgcolor == -1 ? pCGI->area_fgcolor : fgcolor);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_BLOCK_INFO 结构中的6种线型（缺省4种）
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int type						：1 - 全双线 2 - 全单线 3 - 横双竖单 4 - 横单竖双
  返 回 值：
  说    明：
***************************************************************************/
int gmw_set_block_default_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const int type)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	char* ptr = (char*)&pCGI->CBI;
	for(int i = 0; i < 6; i++) {
		strcpy(ptr, DEFAULT_TAB[type][i]);
		ptr += CBI_LEN;
	}
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_BLOCK_INFO 结构中的6种线型
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const char *...					：共6种，具体见.h，此处略
  返 回 值：
  说    明：约定为一个中文制表符，可以使用其它内容，人为保证2字节
			1、超过2字节则只取前2字节
			2、如果给NULL，用两个空格替代
			3、如果给1字节，则补一个空格，如果因此而导致显示乱，不算错
***************************************************************************/
int gmw_set_block_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const char *top_left, const char *lower_left, const char *top_right, const char *lower_right, const char *h_normal, const char *v_normal)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	fillTabString(pCGI->CBI.top_right, top_right);
	fillTabString(pCGI->CBI.top_left, top_left);
	fillTabString(pCGI->CBI.lower_left, lower_left);
	fillTabString(pCGI->CBI.lower_right, lower_right);
	fillTabString(pCGI->CBI.h_normal, h_normal);
	fillTabString(pCGI->CBI.v_normal, v_normal);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置每个游戏色块(彩球)是否需要小边框
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const bool on_off					：true - 需要 flase - 不需要（缺省false）
  返 回 值：
  说    明：边框约定为中文制表符，双线
***************************************************************************/
int gmw_set_block_border_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	pCGI->set_block_border_switch = 1;
	pCGI->CBI.block_border = on_off;
	gmw_set_block_default_linetype(pCGI, 1);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置是否显示上下状态栏
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int type						：状态栏类型（上/下）
			const bool on_off					：true - 需要 flase - 不需要（缺省true）
  返 回 值：
  说    明：1、状态栏的相关约定如下：
			   1.1、上状态栏只能一行，在主区域最上方框线/列标的上面，为主区域的最开始一行（主区域的左上角坐标就是上状态栏的坐标）
			   1.2、下状态栏只能一行，在主区域最下方框线的下面
			   1.3、状态栏的宽度为主区域宽度，如果信息过长则截断
		   2、行列的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_status_line_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const bool on_off)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	bool *chip = (bool*)&pCGI->top_status_line;
	bool *slip = (bool*)&pCGI->SLI.is_top_status_line;
	*(chip + type) = on_off;
	*(slip + type) = on_off;
	updateAll(pCGI);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置上下状态栏的颜色
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int type						：状态栏类型（上/下）
			const int normal_bgcolor			：正常文本背景色（缺省 -1表示使用窗口背景色）
			const int normal_fgcolor			：正常文本前景色（缺省 -1表示使用窗口前景色）
			const int catchy_bgcolor			：醒目文本背景色（缺省 -1表示使用窗口背景色）
			const int catchy_fgcolor			：醒目文本前景色（缺省 -1表示使用亮黄色）
  输入参数：
  返 回 值：
  说    明：不检查颜色值错误及冲突，需要人为保证
				例：颜色非0-15，前景色背景色的值一致导致无法看到内容等
***************************************************************************/
int gmw_set_status_line_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const int normal_bgcolor, const int normal_fgcolor, const int catchy_bgcolor, const int catchy_fgcolor)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	pCGI->set_status_line_color = 1;
	if (type == LOWER_STATUS_LINE) {
		pCGI->SLI.lower_normal_bgcolor = normal_bgcolor == -1 ? pCGI->area_bgcolor : normal_bgcolor;
		pCGI->SLI.lower_normal_fgcolor = normal_fgcolor == -1 ? pCGI->area_fgcolor : normal_fgcolor;
		pCGI->SLI.lower_catchy_bgcolor = catchy_bgcolor == -1 ? pCGI->area_bgcolor : catchy_bgcolor;
		pCGI->SLI.lower_catchy_fgcolor = catchy_fgcolor == -1 ? COLOR_HYELLOW : catchy_fgcolor;
	}
	else {
		pCGI->SLI.top_normal_bgcolor = normal_bgcolor == -1 ? pCGI->area_bgcolor : normal_bgcolor;
		pCGI->SLI.top_normal_fgcolor = normal_fgcolor == -1 ? pCGI->area_fgcolor : normal_fgcolor;
		pCGI->SLI.top_catchy_bgcolor = catchy_bgcolor == -1 ? pCGI->area_bgcolor : catchy_bgcolor;
		pCGI->SLI.top_catchy_fgcolor = catchy_fgcolor == -1 ? COLOR_HYELLOW : catchy_fgcolor;
	}
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置是否显示行号
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const bool on_off					：true - 显示 flase - 不显示（缺省false）
  返 回 值：
  说    明：1、行号约定为字母A开始连续排列（如果超过26，则从a开始，超过52的统一为*，实际应用不可能）
            2、是否显示行号的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_rowno_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	pCGI->draw_frame_with_row_no = on_off;
	updateAll(pCGI);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置是否显示列标
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const bool on_off					：true - 显示 flase - 不显示（缺省false）
  返 回 值：
  说    明：1、列标约定为数字0开始连续排列（数字0-99，超过99统一为**，实际应用不可能）
            2、是否显示列标的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_colno_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	pCGI->draw_frame_with_col_no = on_off;
	updateAll(pCGI);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：打印 CONSOLE_GRAPHICS_INFO 结构体中的各成员值
  输入参数：
  返 回 值：
  说    明：1、仅供调试用，打印格式自定义
            2、本函数测试用例中未调用过，可以不实现
***************************************************************************/
int gmw_print(const CONSOLE_GRAPHICS_INFO *const pCGI)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	cct_cls();
	cct_setfontsize("新宋体", 16);
	cct_gotoxy(0, 0);
	for(int i = 1; i <= 4; i++) {
		for(int j = 0; j < 11; j++) {
			shows(DEFAULT_TAB[i][j]);
			putchar(' ');
		}
		puts("");
	}
	puts("GMW DEBUG INFO:\n");
	printf("bgcolor = %d, fgcolor = %d\n", pCGI->area_bgcolor, pCGI->area_fgcolor);
	printf("游戏主框架区域包含的块的行列数 col_num = %d, row_num = %d\n", pCGI->col_num, pCGI->row_num);
	printf("附加去：up = %d, down = %d, left = %d, right = %d\n", pCGI->extern_up_lines, pCGI->extern_down_lines, pCGI->extern_left_cols, pCGI->extern_right_cols);
	printf("cmd窗口的大小 cols = %d, lines = %d\n", pCGI->cols, pCGI->lines);
	printf("主框架起始位置 (startx = %d, starty = %d)，框线起始位置 (framex = %d, framey = %d)\n", pCGI->start_x, pCGI->start_y, pCGI->frame_x, pCGI->frame_y);
	printf("框线样式：");
	char *ptr = (char*)&pCGI->CFI;
	for(int i = 0; i < 11; i++) {
		shows(ptr);
		ptr += CFI_LEN;
	}
	puts("");
	shows(pCGI->CFI.top_left);
	puts("");
	printf("框架总大小：wid = %d, high = %d\n", pCGI->CFI.tot_wid, pCGI->CFI.tot_high);
	printf("是否有分割线：%c, 是否有上状态栏 %c, 是否有下状态栏 %c，是否有行号 %c，是否有列号 %c\n", 
			"FT"[pCGI->CFI.separator], "FT"[pCGI->top_status_line], "FT"[pCGI->lower_status_line], "FT"[pCGI->draw_frame_with_row_no], "FT"[pCGI->draw_frame_with_col_no]);
	printf("色块宽度和高度 wid = %d, high = %d\n", pCGI->CFI.block_width, pCGI->CFI.block_high);
	printf("色块总大小： wid = %d, high = %d\n", pCGI->CFI.bwidth, pCGI->CFI.bhigh);
	printf("区域总色 area_bg = %d, area_fg = %d\n", pCGI->area_bgcolor, pCGI->area_fgcolor);
	printf("上状态栏: norbg = %d, norfg = %d, chbg = %d, chfg = %d\n", pCGI->SLI.top_normal_bgcolor, pCGI->SLI.top_normal_fgcolor, pCGI->SLI.top_catchy_bgcolor, pCGI->SLI.top_catchy_fgcolor);
	printf("下状态栏: norbg = %d, norfg = %d, chbg = %d, chfg = %d\n", pCGI->SLI.lower_normal_bgcolor, pCGI->SLI.lower_normal_fgcolor, pCGI->SLI.lower_catchy_bgcolor, pCGI->SLI.lower_catchy_fgcolor);
	printf("状态栏长度：width = %d\n", pCGI->SLI.width);
	_getch();
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：将 CONSOLE_GRAPHICS_INFO 结构体用缺省值进行初始化
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI：整体结构指针
		   const int row					：游戏区域色块行数（缺省10）
		   const int col					：游戏区域色块列数（缺省10）
		   const int bgcolor				：整个窗口背景色（缺省 COLOR_BLACK）
		   const int fgcolor				：整个窗口背景色（缺省 COLOR_WHITE）
  返 回 值：
  说    明：窗口背景黑/前景白，点阵16*8，上下左右无额外行列，上下状态栏均有，无行号/列标，框架线型为双线，色块宽度2/高度1/无小边框，颜色略
***************************************************************************/
int gmw_init(CONSOLE_GRAPHICS_INFO *const pCGI, const int row, const int col, const int bgcolor, const int fgcolor)
{
	/* 首先置标记 */
	pCGI->inited = CGI_INITED;
	gmw_set_rowcol(pCGI, row, col);
	gmw_set_color(pCGI, bgcolor, fgcolor, 1);
	gmw_set_font(pCGI, "新宋体");
	gmw_set_ext_rowcol(pCGI);
	gmw_set_status_line_switch(pCGI, 0);
	gmw_set_status_line_switch(pCGI, 1);
	gmw_set_frame_style(pCGI);
	gmw_set_block_border_switch(pCGI);
	gmw_set_frame_default_linetype(pCGI, 1);
	gmw_set_delay(pCGI, DELAY_OF_BLOCK_MOVED, 50);
	updateAll(pCGI);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：画主游戏框架
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
  返 回 值：
  说    明：具体可参考demo的效果
***************************************************************************/
int gmw_draw_frame(const CONSOLE_GRAPHICS_INFO *const pCGI)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
#if debug
	gmw_print(pCGI);
	if(__debugCheckGWMInit(pCGI)) {
		_getch();
		return -1;
	}
#endif
	if (pCGI->inited != CGI_INITED)
		return -1;
	
	cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);    
	cct_cls();
	cct_setfontsize(pCGI->CFT.font_type, pCGI->CFT.font_size_high, pCGI->CFT.font_size_width);
	cct_setconsoleborder(pCGI->cols, pCGI->lines);
	
	drawColNoid(pCGI); // 画行列标号
	drawRowNoid(pCGI);
	cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
	cct_gotoxy(pCGI->frame_x, pCGI->frame_y); // 画框架线
	drawOneSolidLine(pCGI, pCGI->CFI.top_left, pCGI->CFI.top_right, pCGI->CFI.h_top_separator);
	int linecnt = 1;
	for(int i = 0; i < pCGI->row_num; i++) {
		for(int j = 0; j < pCGI->CFI.block_high; j++) {
			cct_gotoxy(pCGI->frame_x, pCGI->frame_y + linecnt++);
			drawOneHollowLine(pCGI, pCGI->CFI.v_normal, pCGI->CFI.v_normal, pCGI->CFI.v_normal);
		}
		if(pCGI->CFI.separator && i < pCGI->row_num - 1) {
			cct_gotoxy(pCGI->frame_x, pCGI->frame_y + linecnt++);
			drawOneSolidLine(pCGI, pCGI->CFI.v_left_separator, pCGI->CFI.v_right_separator, pCGI->CFI.mid_separator);
		}
	}
	cct_gotoxy(pCGI->frame_x, pCGI->frame_y + linecnt++);
	drawOneSolidLine(pCGI, pCGI->CFI.lower_left, pCGI->CFI.lower_right, pCGI->CFI.h_lower_separator);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：在状态栏上显示信息
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int type							：指定是上/下状态栏
		   const char *msg						：正常信息
		   const char *catchy_msg					：需要特别标注的信息（在正常信息前显示）
  返 回 值：
  说    明：1、输出宽度限定为主框架的宽度（含行号列标位置），超出则截去
            2、如果最后一个字符是某汉字的前半个，会导致后面乱码，要处理
***************************************************************************/
int gmw_status_line(const CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const char *msg, const char *catchy_msg)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	char* realmsg = NULL;
	if (msg) {
		realmsg = new(nothrow) char[strlen(msg) + 3];
		strcpy(realmsg, msg);
		char* ptr = realmsg;
		bool hanzi = 0;
		for (; ptr - realmsg < pCGI->SLI.width - 1; ptr++) {
			if (*ptr == 0)
				break;
			if (*ptr < 0 || *ptr > 128) // 是汉字的情况
				hanzi ^= 1; // 两位是一个汉字
			else
				hanzi = 0;
		}
		if (ptr - realmsg == pCGI->SLI.width - 1) { // msg 字符串太长，需要截断
			*ptr = 0;
			if (hanzi) // 最后一个字符是汉字的后半部分
				*(ptr - 1) = 0;
		}
	}
	int posx, posy;
	int length = pCGI->SLI.width - 1 - (realmsg ? strlen(realmsg) : 0) - (catchy_msg ? strlen(catchy_msg) : 0);
	if(!type && pCGI->top_status_line) {
		posx = pCGI->SLI.top_start_x;
		posy = pCGI->SLI.top_start_y;
		if (catchy_msg) {
			cct_showstr(posx, posy, catchy_msg, pCGI->SLI.top_catchy_bgcolor, pCGI->SLI.top_catchy_fgcolor);
			cct_getxy(posx, posy);
		}
		if(realmsg)
			cct_showstr(posx, posy, realmsg, pCGI->SLI.top_normal_bgcolor, pCGI->SLI.top_normal_fgcolor);
		while(length --> 0)
			showc(' ');
	}
	if(type && pCGI->lower_status_line) {
		posx = pCGI->SLI.lower_start_x;
		posy = pCGI->SLI.lower_start_y;
		if (catchy_msg) {
			cct_showstr(posx, posy, catchy_msg, pCGI->SLI.lower_catchy_bgcolor, pCGI->SLI.lower_catchy_fgcolor);
			cct_getxy(posx, posy);
		}
		if(realmsg)
			cct_showstr(posx, posy, realmsg, pCGI->SLI.lower_normal_bgcolor, pCGI->SLI.lower_normal_fgcolor);
		while(length --> 0)
			showc(' ');
	}
	showln();
	if (realmsg)
		delete[] realmsg;
	return 0; //此句可根据需要修改
}

/*
 * 在给定 (x, y) 坐标上绘制一个指定 block
 * (x, y) 是左上角
 */
static void drawBlockXY(const CONSOLE_GRAPHICS_INFO *const pCGI, const int x, const int y, const BLOCK_DISPLAY_INFO block)
{
	int posx = x, posy = y, colx, coly, nowcolbg, nowcolfg;
	int cn = pCGI->CFI.block_width / 2;
	int rn = pCGI->CFI.block_high;
	nowcolbg = block.bgcolor == -1 ? pCGI->CFI.bgcolor : block.bgcolor;
	nowcolfg = block.fgcolor == -1 ? pCGI->CFI.fgcolor : block.fgcolor;
	cct_getcolor(colx, coly);
	cct_setcolor(nowcolbg, nowcolfg);
	if(block.value == BDI_VALUE_BLANK) {
		for(int i = 0; i < rn; i++) {
			cct_gotoxy(posx, posy + i);
			for(int j = 0; j < cn; j++)
				shows("  ");
		}
		cct_setcolor(colx, coly);
		return;
	}
	if(pCGI->CBI.block_border) { // 绘边框
		cct_gotoxy(posx, posy);
		shows(pCGI->CBI.top_left);
		waitBlock(pCGI);
		for(int i = 2; i < cn; i++) {
			shows(pCGI->CBI.h_normal);
			waitBlock(pCGI);
		}
		shows(pCGI->CBI.top_right);
		waitBlock(pCGI);
		for(int i = 2; i < rn; i++) {
			cct_gotoxy(posx, posy + i - 1);
			shows(pCGI->CBI.v_normal);
			waitBlock(pCGI);
			cct_gotoxy(posx + 2 * cn - 2, posy + i - 1);
			shows(pCGI->CBI.v_normal);
			waitBlock(pCGI);
		}
		cct_gotoxy(posx, posy + rn - 1);
		shows(pCGI->CBI.lower_left);
		waitBlock(pCGI);
		for(int i = 2; i < cn; i++) {
			shows(pCGI->CBI.h_normal);
			waitBlock(pCGI);
		}
		shows(pCGI->CBI.lower_right);
		waitBlock(pCGI);
		posx += 2;
	}
	int yPlus = (pCGI->CFI.block_high - pCGI->CBI.block_border * 2 + 1) / 2; // 定位到中间的 y 轴偏移量
	posy = posy + yPlus - !pCGI->CBI.block_border; // 定位到中间
	if(yPlus >= 1) {
		cct_gotoxy(posx, posy);
		if(block.content)
			shows(block.content);
		else
			showiLen(block.value, pCGI->CFI.block_width - pCGI->CBI.block_border * 4);
		for(int i = 1; i < yPlus; i++) // 其他行补全空格
			cct_showstr(posx, y + i - !pCGI->CBI.block_border, " ", nowcolbg, nowcolfg, pCGI->CFI.block_width - pCGI->CBI.block_border * 4);
		for(int i = yPlus + 1; i < pCGI->CFI.block_high - pCGI->CBI.block_border; i++)
			cct_showstr(posx, y + i - !pCGI->CBI.block_border, " ", nowcolbg, nowcolfg, pCGI->CFI.block_width - pCGI->CBI.block_border * 4);

	}
	waitBlock(pCGI);
	cct_setcolor(colx, coly);
}

/***************************************************************************
  函数名称：
  功    能：显示某一个色块(内容为字符串，坐标为row/col)
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int row_no						：行号（从0开始，人为保证正确性，程序不检查）
		   const int col_no						：列号（从0开始，人为保证正确性，程序不检查）
		   const int bdi_value						：需要显示的值
		   const BLOCK_DISPLAY_INFO *const bdi		：存放该值对应的显示信息的结构体数组
  返 回 值：
  说    明：1、BLOCK_DISPLAY_INFO 的含义见头文件，用法参考测试样例
            2、bdi_value为 BDI_VALUE_BLANK 表示空白块，要特殊处理
***************************************************************************/
int gmw_draw_block(const CONSOLE_GRAPHICS_INFO *const pCGI, const int row_no, 
				const int col_no, const int bdi_value, const BLOCK_DISPLAY_INFO *const bdi)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	int posx, posy;
	getBlockXY(pCGI, row_no, col_no, posx, posy);
	drawBlockXY(pCGI, posx, posy, findBDIBlock(bdi, bdi_value));
	return 0; //此句可根据需要修改
}

// /* .h 中定义色块的四种移动方向 */
// #define DOWN_TO_UP		0
// #define UP_TO_DOWN		1
// #define RIGHT_TO_LEFT	2
// #define LEFT_TO_RIGHT	3
// 不同方向时 (x, y) 的偏移量：
const int FORWARD_BIAS_COL[4] = {0, 0, -1, 1};
const int FORWARD_BIAS_ROW[4] = {-1, 1, 0, 0};

/*
 * 把一个格子移动一格坐标值
 * 将位于 (x, y) 的格子按照 direction 方向移动一个坐标
 * 动画按照 block 显示
 */
static void moveBlockOnePixel(const CONSOLE_GRAPHICS_INFO *const pCGI, const int x, const int y, const BLOCK_DISPLAY_INFO block, const BLOCK_DISPLAY_INFO blankBlock, const int direction)
{
	drawBlockXY(pCGI, x, y, {BDI_VALUE_BLANK, -1, -1, NULL});
	if(blankBlock.value != BDI_VALUE_BLANK) {
		drawBlockXY(pCGI, x + FORWARD_BIAS_COL[direction], y + FORWARD_BIAS_ROW[direction], blankBlock);
		waitMoved(pCGI);
	}
	drawBlockXY(pCGI, x + FORWARD_BIAS_COL[direction], y + FORWARD_BIAS_ROW[direction], block);
}

/*
 * 把一个格子移动一格
 * 将位于 (row_no, col_no) 的格子按照 direction 方向移动一格
 * 动画按照 block 显示
 */
static void moveOneBlock(const CONSOLE_GRAPHICS_INFO *const pCGI, const int row_no, const int col_no, const BLOCK_DISPLAY_INFO block, const BLOCK_DISPLAY_INFO blankBlock, const int direction)
{
	int len = direction < 2 ? pCGI->CFI.bhigh : pCGI->CFI.bwidth;
	int nowx, nowy;
	getBlockXY(pCGI, row_no, col_no, nowx, nowy);
	for(int i = 0; i < len; i++) { 
		if(i)
			waitMoved(pCGI);
		moveBlockOnePixel(pCGI, nowx, nowy, block, blankBlock, direction);
		nowx = nowx + FORWARD_BIAS_COL[direction];
		nowy = nowy + FORWARD_BIAS_ROW[direction];
	}
	if (pCGI->CFI.separator || row_no < 0 || col_no < 0) { // 补全分割线
		cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
		if (direction < 2 || row_no < 0) { // 0 向上，1 向下
			int r = row_no + direction; // 重绘制 (r, c) 上方的横条
			int c = col_no;
			getBlockXY(pCGI, r, c, nowx, nowy);
			nowy--;
			cct_gotoxy(nowx, nowy);
			for (int i = 0; i < pCGI->CFI.block_width / 2; i++)
				shows(pCGI->CFI.h_normal);
		}
		else { // 2 向左，3 向右
			int r = row_no;
			int c = col_no + (direction == 3); // 重绘制 (r, c) 左侧的竖条
			getBlockXY(pCGI, r, c, nowx, nowy);
			nowx -= 2;
			for (int i = 0; i < pCGI->CFI.block_high; i++) {
				cct_gotoxy(nowx, nowy + i);
				shows(pCGI->CFI.v_normal);
			}
		}
	}
	waitMoved(pCGI);
}

/***************************************************************************
  函数名称：
  功    能：移动某一个色块
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int row_no						：行号（从0开始，人为保证正确性，程序不检查）
		   const int col_no						：列号（从0开始，人为保证正确性，程序不检查）
		   const int bdi_value						：需要显示的值
		   const int blank_bdi_value				：移动过程中用于动画效果显示时用于表示空白的值（一般为0，此处做为参数代入，是考虑到可能出现的特殊情况）
		   const BLOCK_DISPLAY_INFO *const bdi		：存放显示值/空白值对应的显示信息的结构体数组
		   const int direction						：移动方向，一共四种，具体见cmd_gmw_tools.h
		   const int distance						：移动距离（从1开始，人为保证正确性，程序不检查）
  返 回 值：
  说    明：
***************************************************************************/
int gmw_move_block(const CONSOLE_GRAPHICS_INFO *const pCGI, const int row_no, const int col_no, const int bdi_value, const int blank_bdi_value, const BLOCK_DISPLAY_INFO *const bdi, const int direction, const int distance)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	int nowr = row_no, nowc = col_no;
	for(int i = 0; i < distance; i++) {
		moveOneBlock(pCGI, nowr, nowc, findBDIBlock(bdi, bdi_value), findBDIBlock(bdi, blank_bdi_value), direction);
		nowr = nowr + FORWARD_BIAS_ROW[direction];
		nowc = nowc + FORWARD_BIAS_COL[direction];
	}
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：读键盘或鼠标
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   int &MAction							：如果返回 CCT_MOUSE_EVENT，则此值有效，为 MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK/MOUSE_RIGHT_BUTTON_CLICK 三者之一
		                                               如果返回 CCT_KEYBOARD_EVENT，则此值无效
		   int &MRow								：如果返回 CCT_MOUSE_EVENT 且 MAction = MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK，则此值有效，表示左键选择的游戏区域的行号（从0开始）
												  其余情况此值无效（如果访问无效值导致错误，不是本函数的错!!!）
		   int &MCol								：如果返回 CCT_MOUSE_EVENT 且 MAction = MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK，则此值有效，表示左键选择的游戏区域的列号（从0开始）
												  其余情况此值无效（如果访问无效值导致错误，不是本函数的错!!!）
		   int &KeyCode1							：如果返回 CCT_KEYBOARD_EVENT，则此值有效，为读到的键码（如果双键码，则为第一个）
												  其余情况此值无效（如果访问无效值导致错误，不是本函数的错!!!）
		   int &KeyCode2							：如果返回 CCT_KEYBOARD_EVENT，则此值有效，为读到的键码（如果双键码，则为第二个，如果是单键码，则为0）
												  其余情况此值无效（如果访问无效值导致错误，不是本函数的错!!!）
		   const bool update_lower_status_line		：鼠标移动时，是否要在本函数中显示"[当前光标] *行*列/位置非法"的信息（true=显示，false=不显示，缺省为true）
  返 回 值：函数返回约定
		   1、如果是鼠标移动，得到的MRow/MCol与传入的相同(鼠标指针微小的移动)，则不返回，继续读
							  得到行列非法位置，则不返回，根据 update_lower_status_line 的设置在下状态栏显示"[当前光标] 位置非法"
							  得到的MRow/MCol与传入的不同(行列至少一个变化)，根据 update_lower_status_line 的设置在下状态栏显示"[当前光标] *行*列"，再返回MOUSE_ONLY_MOVED（有些游戏返回后要处理色块的不同颜色显示）
		   2、如果是按下鼠标左键，且当前鼠标指针停留在主游戏区域的*行*列上，则返回 CCT_MOUSE_EVENT ，MAction 为 MOUSE_LEFT_BUTTON_CLICK, MRow 为行号，MCol 为列标
		                          且当前鼠标指针停留在非法区域（非游戏区域，游戏区域中的分隔线），则不返回，根据 update_lower_status_line 的设置在下状态栏显示"[当前光标] 位置非法"
		   3、如果是按下鼠标右键，则不判断鼠标指针停留区域是否合法，直接返回 CCT_MOUSE_EVENT ，MAction 为 MOUSE_RIGHT_BUTTON_CLICK, MRow、MCol取当前值（因为消灭星星的右键标记需要坐标）
		   4、如果按下键盘上的某键（含双键码按键），则直接返回 CCT_KEYBOARD_EVENT，KeyCode1/KeyCode2中为对应的键码值
 说    明：通过调用 cmd_console_tools.cpp 中的 read_keyboard_and_mouse 函数实现
***************************************************************************/
int gmw_read_keyboard_and_mouse(const CONSOLE_GRAPHICS_INFO *const pCGI, int &MAction, int &MRow, int &MCol, int &KeyCode1, int &KeyCode2, const bool update_lower_status_line)
{

	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	const int oriRow = MRow, oriCol = MCol;
	char s[30];
	int ret, mx, my, lstallow = 0;
	cct_enable_mouse();
	while(1) {
		ret = cct_read_keyboard_and_mouse(mx, my, MAction, KeyCode1, KeyCode2);
		int allow = getBlockCoord(pCGI, mx, my, MRow, MCol);
#if debug
		char str[100];
		sprintf(str, "xy=(%d, %d),Row=%d,Col=%d,ret=%d    ", mx, my, MRow, MCol, tmpret);
		__debugPrint(str);
#endif
		if(ret == CCT_KEYBOARD_EVENT) // 处理键盘
			return CCT_KEYBOARD_EVENT;
		if(MAction == MOUSE_ONLY_MOVED) { // 移动鼠标
			if (allow != lstallow) {
				if (update_lower_status_line) {
					sprintf(s, "[当前光标] %c行%s列\0", getRowNo(MRow), getColNo(MCol));
					gmw_status_line(pCGI, LOWER_STATUS_LINE, s);
				}
			}
			lstallow = allow;
			if(MRow == oriRow && MCol == oriCol) 
				continue;
			if(allow) {
				if(update_lower_status_line) {
					sprintf(s, "[当前光标] %c行%s列\0", getRowNo(MRow), getColNo(MCol));
					gmw_status_line(pCGI, LOWER_STATUS_LINE, s);
				}
				return CCT_MOUSE_EVENT;
			}
			if(update_lower_status_line) {
				sprintf(s, "[当前光标] 位置非法\0");
				gmw_status_line(pCGI, LOWER_STATUS_LINE, s);
			}
		}
		else if(MAction == MOUSE_LEFT_BUTTON_DOUBLE_CLICK || MAction == MOUSE_LEFT_BUTTON_CLICK) { // 按左键
			if(allow)
				return CCT_MOUSE_EVENT;
			if(update_lower_status_line) {
				sprintf(s, "[当前光标] 位置非法\0");
				gmw_status_line(pCGI, LOWER_STATUS_LINE, s);
			}
		}
		else if(MAction == MOUSE_RIGHT_BUTTON_DOUBLE_CLICK || MAction == MOUSE_RIGHT_BUTTON_CLICK) { // 按右键
			return CCT_MOUSE_EVENT;
		}
		lstallow = allow;
	}
	return 0; //此句可根据需要修改
}
