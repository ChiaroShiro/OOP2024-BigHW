/* 2351871 ������ �ƿ� */
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
		�˴����Ը�����Ҫ�ľ�̬ȫ�ֱ������������٣����û�У�����̬ȫ��ֻ������/�궨�壨�������ޣ���
   -------------------------------------------------- */

#define debug 			0

/* --------------------------------------------------
		�˴����Ը�����Ҫ���ڲ��������ߺ���
		1�����������ޣ�����Ϊ gmw_inner_* 
		2����������
		3��������static������ȷ��ֻ�ڱ�Դ�ļ���ʹ��
   -------------------------------------------------- */

const char* const DEFAULT_TAB[5][11] = {
	{},
	{"�X", "�^", "�[", "�a", "�T", "�U", "�j", "�m", "�d", "�g", "�p"},
	{"��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��"},
	{"�V", "�\", "�Y", "�_", "�T", "��", "�h", "�k", "�b", "�e", "�n"},
	{"�W", "�]", "�Z", "�`", "��", "�U", "�i", "�l", "�c", "�f", "�o"}
}; // 1 - ȫ�� 2 - ȫ���� 3 - ��˫���� 4 - �ᵥ��˫

static void shows(const char* s) 
{
	int x, y, cx, cy;
	cct_getxy(x, y);
	cct_getcolor(cx, cy);
	cct_showstr(x, y, s, cx, cy);
}

static void showc(const char& c) 
{
	int x, y, cx, cy;
	cct_getxy(x, y);
	cct_getcolor(cx, cy);
	cct_showch(x, y, c, cx, cy);
}

static void showi (const int &p) 
{
	int x, y, cx, cy;
	cct_getxy(x, y);
	cct_getcolor(cx, cy);
	cct_showint(x, y, p, cx, cy);
}

static void showln()
{
	showc('\n');
}

static void initCGI (CONSOLE_GRAPHICS_INFO *const pCGI) 
{
	pCGI->set_rowcol = pCGI->set_color = pCGI->set_font = pCGI->set_frame_linetype = pCGI->set_frame_style = 0;
	pCGI->set_frame_color = pCGI->set_block_border_switch = pCGI->set_status_line_color = 0;
	pCGI->have_set_rowcol = pCGI->have_set_blocksize = 0;

	pCGI->top_status_line = pCGI->lower_status_line = 0;
	pCGI->draw_frame_with_row_no = pCGI->draw_frame_with_col_no = 0;
	pCGI->delay_of_draw_frame = pCGI->delay_of_draw_block = pCGI->delay_of_block_moved = 0;

	pCGI->extern_down_lines = pCGI->extern_left_cols = pCGI->extern_right_cols = pCGI->extern_up_lines = 0;
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

/*
 *  ��ֵ 2 �ֽ�
 *  �� src �� NULL �� "  "
 *  ���� 2 �ֽڲ�ȫ�ո�
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
{
	pCGI->CFI.tot_high = pCGI->row_num * pCGI->CFI.block_high;
	pCGI->CFI.tot_high += (pCGI->row_num - 1) * pCGI->CFI.separator + 2;
	pCGI->CFI.tot_wid = pCGI->col_num * pCGI->CFI.block_width;
	pCGI->CFI.tot_wid += ((pCGI->col_num - 1) * pCGI->CFI.separator + 2) * 2;
}

static void update_linecol(CONSOLE_GRAPHICS_INFO *const pCGI)
{
	pCGI->lines = pCGI->extern_up_lines + pCGI->extern_down_lines + pCGI->CFI.tot_high + 
				  pCGI->top_status_line + pCGI->lower_status_line + pCGI->draw_frame_with_col_no + 4;
	pCGI->cols  = pCGI->CFI.tot_wid + pCGI->extern_left_cols + pCGI->extern_right_cols + pCGI->draw_frame_with_row_no * 2 + 1;
}

static void update_startxy(CONSOLE_GRAPHICS_INFO *const pCGI)
{
	pCGI->start_x = pCGI->extern_left_cols;
	pCGI->start_y = pCGI->extern_up_lines + pCGI->top_status_line;
}

static void update_framexy(CONSOLE_GRAPHICS_INFO *const pCGI)
{
	pCGI->frame_x = pCGI->extern_left_cols + pCGI->draw_frame_with_row_no * 2;
	pCGI->frame_y = pCGI->extern_up_lines + pCGI->top_status_line + pCGI->draw_frame_with_col_no;
}

static void update_sliwid(CONSOLE_GRAPHICS_INFO *const pCGI)
{
	pCGI->SLI.width = pCGI->cols - pCGI->extern_left_cols - pCGI->extern_right_cols;
}

static void updateAll(CONSOLE_GRAPHICS_INFO *const pCGI)
{
	update_tothw(pCGI);
	update_linecol(pCGI);
	update_framexy(pCGI);
	update_startxy(pCGI);
	update_sliwid(pCGI);
}

/*
 * ��һʵ����
 */
static void drawOneSolidLine(const CONSOLE_GRAPHICS_INFO *const pCGI, const char* left, const char* right, const char* mid)
{
	const CONSOLE_FRAME_INFO* const pc = &pCGI->CFI;
	shows(left);
	Sleep(pCGI->delay_of_draw_frame);
	for(int i = 0; i < pCGI->col_num; i++) {
		for(int j = 0; j < pc->block_width; j += 2)
			shows(pc->h_normal);
		Sleep(pCGI->delay_of_draw_frame);
		if(pc->separator && i < pCGI->col_num - 1) {
			shows(mid);
		}
	}
	shows(right);
	Sleep(pCGI->delay_of_draw_frame);
	showln();
}

/*
 * ��һ������
 */
static void drawOneHollowLine(const CONSOLE_GRAPHICS_INFO *const pCGI, const char* left, const char* right, const char* mid)
{
	const CONSOLE_FRAME_INFO* const pc = &pCGI->CFI;
	shows(left);
	Sleep(pCGI->delay_of_draw_frame);
	for(int i = 0; i < pCGI->col_num; i++) {
		for(int j = 0; j < pc->block_width; j += 2)
			shows("  ");
		Sleep(pCGI->delay_of_draw_frame);
		if(pc->separator && i < pCGI->col_num - 1) {
			shows(mid);
		}
	}
	shows(right);
	Sleep(pCGI->delay_of_draw_frame);
	showln();
}

static void drawColNoid(const CONSOLE_GRAPHICS_INFO *const pCGI)
{
	if(pCGI->draw_frame_with_col_no) {
		for(int i = 0; i < pCGI->col_num; i++) {
			cct_gotoxy(pCGI->start_x + 2 + 2 * pCGI->draw_frame_with_row_no + pCGI->CFI.bwidth * i + (pCGI->CFI.block_width + 1) / 2 - 1, pCGI->start_y);
			if(i < 100)
				showi(i);
			else 
				shows("**");
		}
	}
}

static void drawRowNoid(const CONSOLE_GRAPHICS_INFO *const pCGI)
{
	if(pCGI->draw_frame_with_row_no) {
		for(int i = 0; i < pCGI->row_num; i++) {
			cct_gotoxy(pCGI->start_x, pCGI->start_y + 1 + pCGI->draw_frame_with_col_no + (pCGI->CFI.block_high + 1) / 2 + pCGI->CFI.bhigh * i - 1);
			if(i < 52)
				showc(i < 26 ? 'A' + i : 'a' + i - 26);
			else 
				showc('*');
		}
	}
}

/* ----------------------------------------------- 
		ʵ����������ĺ���������������׼����
   ----------------------------------------------- */
/***************************************************************************
  �������ƣ�
  ��    �ܣ�������Ϸ����ܵ�������
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int row						������(������Ϊ0���������ޣ���Ϊ��֤��ȷ��)
			const int col						������(������Ϊ0���������ޣ���Ϊ��֤��ȷ��)
  �� �� ֵ��
  ˵    ����1��ָ��������Ϸ�ľ������������ֵ
            2�����еı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_rowcol(CONSOLE_GRAPHICS_INFO *const pCGI, const int row, const int col)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	initCGI(pCGI);
	if (pCGI->inited != CGI_INITED)
		return -1;
	pCGI->set_rowcol = 1;
	pCGI->row_num = max(0, row);
	pCGI->col_num = max(0, col);
	pCGI->have_set_rowcol = 1;
	updateAll(pCGI);
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������������ڣ�����Ϸ�������������ڵ�����cmd���ڣ�����ɫ
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int bg_color					��ǰ��ɫ��ȱʡCOLOR_BLACK��
		   const int fg_color					������ɫ��ȱʡCOLOR_WHITE��
		   const bool cascade					���Ƿ�����ȱʡΪtrue-������
  �� �� ֵ��
  ˵    ����1��cascade = trueʱ
				ͬ���޸���Ϸ���������ɫ
				ͬ���޸�����״̬���������ı��ı���ɫ��ǰ��ɫ����Ŀ�ı��ı���ɫ��ǰ��ɫ���䣩
			2���������ɫֵ���󼰳�ͻ����Ҫ��Ϊ��֤
				������ɫ��0-15
				    ǰ��ɫ����ɫ��ֵһ�µ����޷���������
					ǰ��ɫ������״̬����Ŀǰ��ɫ�������޷�������Ŀ��ʾ
					...
***************************************************************************/
int gmw_set_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int bgcolor, const int fgcolor, const bool cascade)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
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
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ô��ڵ�����
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const char *fontname					���������ƣ�ֻ����"Terminal"��"������"���֣������򷵻�-1�����ı����壩
		   const int fs_high					������߶ȣ�ȱʡ������Ϊ16�������������ƣ���Ϊ��֤��
		   const int fs_width					������߶ȣ�ȱʡ������Ϊ8�������������ƣ���Ϊ��֤��
  �� �� ֵ��
  ˵    ����1����cmd_console_tools�е�setfontsize���ƣ�Ŀǰֻ֧�֡��������塱�͡������塱
            2������������������ֱ�ӷ��أ�����ԭ�������ò���
***************************************************************************/
int gmw_set_font(CONSOLE_GRAPHICS_INFO *const pCGI, const char *fontname, const int fs_high, const int fs_width)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;  
	if(strcmp(fontname, "Terminal") && strcmp(fontname, "������"))
		return -1;
	pCGI->set_font = 1;
	strcpy(pCGI->CFT.font_type, fontname);
	pCGI->CFT.font_size_width = fs_width;
	pCGI->CFT.font_size_high  = fs_high;
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�������ʱ
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int type						����ʱ�����ͣ�ĿǰΪ3�֣�
		   const int delay_ms					����msΪ��λ����ʱ
			   ���߿����ʱ��0 ~ �������ޣ���Ϊ��֤��ȷ��<0����0��
			   ��ɫ�����ʱ��0 ~ �������ޣ���Ϊ��֤��ȷ��<0����0��
			   ɫ���ƶ�����ʱ��BLOCK_MOVED_DELAY_MS ~ �������ޣ���Ϊ��֤��ȷ�� <BLOCK_MOVED_DELAY_MS ���� BLOCK_MOVED_DELAY_MS��
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int gmw_set_delay(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const int delay_ms)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	if (type == DELAY_OF_DRAW_FRAME)
		pCGI->delay_of_draw_frame  = delay_ms;
	else if (type == DELAY_OF_DRAW_BLOCK)
		pCGI->delay_of_draw_block  = delay_ms;
	else if (type == DELAY_OF_BLOCK_MOVED)
		pCGI->delay_of_block_moved = delay_ms;
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ���������������Ϸ����ܽṹ֮����Ҫ�����Ķ�������
  ��    �ܣ�CONSOLE_GRAPHICS_INFO *const pCGI	��
		   const int up_lines					���ϲ�������У�ȱʡ������Ϊ0���������ޣ���Ϊ��֤��
		   const int down_lines				���²�������У�ȱʡ������Ϊ0���������ޣ���Ϊ��֤��
		   const int left_cols					����߶�����У�ȱʡ������Ϊ0���������ޣ���Ϊ��֤��
		   const int right_cols				���ұ߶�����У�ȱʡ������Ϊ0���������ޣ���Ϊ��֤��
  �� �� ֵ��
  ˵    �����������еı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_ext_rowcol(CONSOLE_GRAPHICS_INFO *const pCGI, const int up_lines, const int down_lines, const int left_cols, const int right_cols)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	pCGI->extern_up_lines = up_lines;
	pCGI->extern_down_lines = down_lines;
	pCGI->extern_left_cols = left_cols;
	pCGI->extern_right_cols = right_cols;
	updateAll(pCGI);
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_FRAME_TYPE �ṹ�е�11�����ͣ�ȱʡ4�֣�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int type						��1 - ȫ�� 2 - ȫ���� 3 - ��˫���� 4 - �ᵥ��˫
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int gmw_set_frame_default_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const int type)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	pCGI->set_frame_linetype = 1;
	char* ptr = (char*)&pCGI->CFI;
	for (int i = 0; i < 11; i++) {
		strcpy(ptr, DEFAULT_TAB[type][i]);
		ptr += CFI_LEN;
	}
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_FRAME_TYPE �ṹ�е�11������
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const char *...						����11�֣������.h���˴���
  �� �� ֵ��
  ˵    ����Լ��Ϊһ�������Ʊ��������ʹ���������ݣ���Ϊ��֤2�ֽ�
			1������2�ֽ���ֻȡǰ2�ֽ�
			2�������NULL���������ո����
			3�������1�ֽڣ���һ���ո������˶�������ʾ�ң������
***************************************************************************/
int gmw_set_frame_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const char *top_left, const char *lower_left, const char *top_right,
	const char *lower_right, const char *h_normal, const char *v_normal, const char *h_top_separator,
	const char *h_lower_separator, const char *v_left_separator, const char *v_right_separator, const char *mid_separator)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
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
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_FRAME_TYPE �ṹ�е�ɫ��������С���Ƿ���Ҫ�ָ��ߵ�
  ������������������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int block_width						����ȣ�����ȱʡ2����ΪԼ�������Ϊ�����Ʊ�����������������Ҫ+1��
			const int block_high						���߶ȣ�����ȱʡ1��
			const bool separator						���Ƿ���Ҫ�ָ��ߣ�ȱʡΪtrue����Ҫ�ָ��ߣ�
  �� �� ֵ��
  ˵    ������ܴ�С/�Ƿ���Ҫ�ָ��ߵȵı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_frame_style(CONSOLE_GRAPHICS_INFO *const pCGI, const int block_width, const int block_high, const bool separator)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
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
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_BORDER_TYPE �ṹ�е���ɫ
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int bg_color					������ɫ��ȱʡ -1��ʾ�ô��ڱ���ɫ��
			const int fg_color					��ǰ��ɫ��ȱʡ -1��ʾ�ô���ǰ��ɫ��
  �� �� ֵ��
  ˵    �����������ɫֵ���󼰳�ͻ����Ҫ��Ϊ��֤
				������ɫ��0-15��ǰ��ɫ����ɫ��ֵһ�µ����޷��������ݵ�
***************************************************************************/
int gmw_set_frame_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int bgcolor, const int fgcolor)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	pCGI->set_frame_color = 1;
	pCGI->CFI.bgcolor = (bgcolor == -1 ? pCGI->area_bgcolor : bgcolor);
	pCGI->CFI.fgcolor = (fgcolor == -1 ? pCGI->area_fgcolor : fgcolor);
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_BLOCK_INFO �ṹ�е�6�����ͣ�ȱʡ4�֣�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int type						��1 - ȫ˫�� 2 - ȫ���� 3 - ��˫���� 4 - �ᵥ��˫
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int gmw_set_block_default_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const int type)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	char* ptr = (char*)&pCGI->CBI;
	for(int i = 0; i < 6; i++) {
		strcpy(ptr, DEFAULT_TAB[type][i]);
		ptr += CBI_LEN;
	}
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_BLOCK_INFO �ṹ�е�6������
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const char *...					����6�֣������.h���˴���
  �� �� ֵ��
  ˵    ����Լ��Ϊһ�������Ʊ��������ʹ���������ݣ���Ϊ��֤2�ֽ�
			1������2�ֽ���ֻȡǰ2�ֽ�
			2�������NULL���������ո����
			3�������1�ֽڣ���һ���ո������˶�������ʾ�ң������
***************************************************************************/
int gmw_set_block_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const char *top_left, const char *lower_left, const char *top_right, const char *lower_right, const char *h_normal, const char *v_normal)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	fillTabString(pCGI->CBI.top_right, top_right);
	fillTabString(pCGI->CBI.top_left, top_left);
	fillTabString(pCGI->CBI.lower_left, lower_left);
	fillTabString(pCGI->CBI.lower_right, lower_right);
	fillTabString(pCGI->CBI.h_normal, h_normal);
	fillTabString(pCGI->CBI.v_normal, v_normal);
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�����ÿ����Ϸɫ��(����)�Ƿ���ҪС�߿�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const bool on_off					��true - ��Ҫ flase - ����Ҫ��ȱʡfalse��
  �� �� ֵ��
  ˵    �����߿�Լ��Ϊ�����Ʊ����˫��
***************************************************************************/
int gmw_set_block_border_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	pCGI->set_block_border_switch = 1;
	pCGI->CBI.block_border = on_off;
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������Ƿ���ʾ����״̬��
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int type						��״̬�����ͣ���/�£�
			const bool on_off					��true - ��Ҫ flase - ����Ҫ��ȱʡtrue��
  �� �� ֵ��
  ˵    ����1��״̬�������Լ�����£�
			   1.1����״̬��ֻ��һ�У������������Ϸ�����/�б�����棬Ϊ��������ʼһ�У�����������Ͻ����������״̬�������꣩
			   1.2����״̬��ֻ��һ�У������������·����ߵ�����
			   1.3��״̬���Ŀ��Ϊ�������ȣ������Ϣ������ض�
		   2�����еı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_status_line_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const bool on_off)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	bool *chip = (bool*)&pCGI->top_status_line;
	bool *slip = (bool*)&pCGI->SLI.is_top_status_line;
	*(chip + type) = on_off;
	*(slip + type) = on_off;
	updateAll(pCGI);
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���������״̬������ɫ
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int type						��״̬�����ͣ���/�£�
			const int normal_bgcolor			�������ı�����ɫ��ȱʡ -1��ʾʹ�ô��ڱ���ɫ��
			const int normal_fgcolor			�������ı�ǰ��ɫ��ȱʡ -1��ʾʹ�ô���ǰ��ɫ��
			const int catchy_bgcolor			����Ŀ�ı�����ɫ��ȱʡ -1��ʾʹ�ô��ڱ���ɫ��
			const int catchy_fgcolor			����Ŀ�ı�ǰ��ɫ��ȱʡ -1��ʾʹ������ɫ��
  ���������
  �� �� ֵ��
  ˵    �����������ɫֵ���󼰳�ͻ����Ҫ��Ϊ��֤
				������ɫ��0-15��ǰ��ɫ����ɫ��ֵһ�µ����޷��������ݵ�
***************************************************************************/
int gmw_set_status_line_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const int normal_bgcolor, const int normal_fgcolor, const int catchy_bgcolor, const int catchy_fgcolor)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	pCGI->set_status_line_color = 1;
	pCGI->SLI.lower_normal_bgcolor = pCGI->SLI.top_normal_bgcolor = normal_bgcolor;
	pCGI->SLI.lower_normal_fgcolor = pCGI->SLI.top_normal_fgcolor = normal_fgcolor;
	pCGI->SLI.lower_catchy_bgcolor = pCGI->SLI.top_catchy_bgcolor = catchy_bgcolor;
	pCGI->SLI.lower_catchy_fgcolor = pCGI->SLI.top_catchy_fgcolor = catchy_fgcolor;
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������Ƿ���ʾ�к�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const bool on_off					��true - ��ʾ flase - ����ʾ��ȱʡfalse��
  �� �� ֵ��
  ˵    ����1���к�Լ��Ϊ��ĸA��ʼ�������У��������26�����a��ʼ������52��ͳһΪ*��ʵ��Ӧ�ò����ܣ�
            2���Ƿ���ʾ�кŵı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_rowno_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	pCGI->draw_frame_with_row_no = on_off;
	updateAll(pCGI);
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������Ƿ���ʾ�б�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const bool on_off					��true - ��ʾ flase - ����ʾ��ȱʡfalse��
  �� �� ֵ��
  ˵    ����1���б�Լ��Ϊ����0��ʼ�������У�����0-99������99ͳһΪ**��ʵ��Ӧ�ò����ܣ�
            2���Ƿ���ʾ�б�ı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_colno_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	pCGI->draw_frame_with_col_no = on_off;
	updateAll(pCGI);
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���ӡ CONSOLE_GRAPHICS_INFO �ṹ���еĸ���Աֵ
  ���������
  �� �� ֵ��
  ˵    ����1�����������ã���ӡ��ʽ�Զ���
            2������������������δ���ù������Բ�ʵ��
***************************************************************************/
int gmw_print(const CONSOLE_GRAPHICS_INFO *const pCGI)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	cct_cls();
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
	printf("��Ϸ�������������Ŀ�������� col_num = %d, row_num = %d\n", pCGI->col_num, pCGI->row_num);
	printf("cmd���ڵĴ�С cols = %d, lines = %d\n", pCGI->cols, pCGI->lines);
	printf("�������ʼλ�� (startx = %d, starty = %d)��������ʼλ�� (framex = %d, framey = %d)\n", pCGI->start_x, pCGI->start_y, pCGI->frame_x, pCGI->frame_y);
	printf("������ʽ��");
	char *ptr = (char*)&pCGI->CFI;
	for(int i = 0; i < 11; i++) {
		shows(ptr);
		ptr += CFI_LEN;
	}
	puts("");
	shows(pCGI->CFI.top_left);
	puts("");
	printf("����ܴ�С��wid = %d, high = %d\n", pCGI->CFI.tot_wid, pCGI->CFI.tot_high);
	printf("�Ƿ��зָ��ߣ�%c\n", "FT"[pCGI->CFI.separator]);
	printf("ɫ���Ⱥ͸߶� wid = %d, high = %d\n", pCGI->CFI.block_width, pCGI->CFI.block_high);
	printf("ɫ���ܴ�С�� wid = %d, high = %d\n", pCGI->CFI.bwidth, pCGI->CFI.bhigh);
	printf("������ɫ area_bg = %d, area_fg = %d\n", pCGI->area_bgcolor, pCGI->area_fgcolor);
	printf("��״̬��: norbg = %d, norfg = %d, chbg = %d, chfg = %d\n", pCGI->SLI.top_normal_bgcolor, pCGI->SLI.top_normal_fgcolor, pCGI->SLI.top_catchy_bgcolor, pCGI->SLI.top_catchy_fgcolor);
	printf("��״̬��: norbg = %d, norfg = %d, chbg = %d, chfg = %d\n", pCGI->SLI.lower_normal_bgcolor, pCGI->SLI.lower_normal_fgcolor, pCGI->SLI.lower_catchy_bgcolor, pCGI->SLI.lower_catchy_fgcolor);
	printf("״̬�����ȣ�width = %d\n", pCGI->SLI.width);
	_getch();
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��� CONSOLE_GRAPHICS_INFO �ṹ����ȱʡֵ���г�ʼ��
  ���������CONSOLE_GRAPHICS_INFO *const pCGI������ṹָ��
		   const int row					����Ϸ����ɫ��������ȱʡ10��
		   const int col					����Ϸ����ɫ��������ȱʡ10��
		   const int bgcolor				���������ڱ���ɫ��ȱʡ COLOR_BLACK��
		   const int fgcolor				���������ڱ���ɫ��ȱʡ COLOR_WHITE��
  �� �� ֵ��
  ˵    �������ڱ�����/ǰ���ף�����16*8�����������޶������У�����״̬�����У����к�/�б꣬�������Ϊ˫�ߣ�ɫ����2/�߶�1/��С�߿���ɫ��
***************************************************************************/
int gmw_init(CONSOLE_GRAPHICS_INFO *const pCGI, const int row, const int col, const int bgcolor, const int fgcolor)
{
	/* �����ñ�� */
	pCGI->inited = CGI_INITED;
	gmw_set_rowcol(pCGI, row, col);
	gmw_set_color(pCGI, bgcolor, fgcolor, 1);
	gmw_set_font(pCGI, "������");
	gmw_set_ext_rowcol(pCGI);
	gmw_set_status_line_switch(pCGI, 0);
	gmw_set_status_line_switch(pCGI, 1);
	gmw_set_frame_style(pCGI);
	gmw_set_block_border_switch(pCGI);
	gmw_set_frame_default_linetype(pCGI, 1);
	updateAll(pCGI);
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�������Ϸ���
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
  �� �� ֵ��
  ˵    ��������ɲο�demo��Ч��
***************************************************************************/
int gmw_draw_frame(const CONSOLE_GRAPHICS_INFO *const pCGI)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	gmw_print(pCGI);
#if debug
	if(__debugCheckGWMInit(pCGI)) {
		_getch();
		return -1;
	}
#endif
	if (pCGI->inited != CGI_INITED)
		return -1;
	cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);
	cct_setfontsize(pCGI->CFT.font_type, pCGI->CFT.font_size_high, pCGI->CFT.font_size_width);
	cct_cls();
	cct_setconsoleborder(pCGI->cols, pCGI->lines);
	drawColNoid(pCGI);
	drawRowNoid(pCGI);
	cct_gotoxy(pCGI->frame_x, pCGI->frame_y);
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
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���״̬������ʾ��Ϣ
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int type							��ָ������/��״̬��
		   const char *msg						��������Ϣ
		   const char *catchy_msg					����Ҫ�ر��ע����Ϣ����������Ϣǰ��ʾ��
  �� �� ֵ��
  ˵    ����1���������޶�Ϊ����ܵĿ�ȣ����к��б�λ�ã����������ȥ
            2��������һ���ַ���ĳ���ֵ�ǰ������ᵼ�º������룬Ҫ����
***************************************************************************/
int gmw_status_line(const CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const char *msg, const char *catchy_msg)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	int posx, posy;
	int length = pCGI->SLI.width - 1 - (msg ? strlen(msg) : 0) - (catchy_msg ? strlen(catchy_msg) : 0);
	if(!type && pCGI->top_status_line) {
		posx = pCGI->extern_left_cols;
		posy = pCGI->extern_up_lines;
		if (catchy_msg) {
			cct_showstr(posx, posy, catchy_msg, pCGI->SLI.top_catchy_bgcolor, pCGI->SLI.top_catchy_fgcolor);
			cct_getxy(posx, posy);
		}
		if(msg)
			cct_showstr(posx, posy, msg, pCGI->SLI.top_normal_bgcolor, pCGI->SLI.top_normal_fgcolor);
		while(length --> 0)
			showc(' ');
	}
	if(type && pCGI->lower_status_line) {
		posx = pCGI->extern_left_cols;
		posy = pCGI->extern_up_lines + pCGI->CFI.tot_high + pCGI->top_status_line + pCGI->draw_frame_with_col_no;
		if (catchy_msg) {
			cct_showstr(posx, posy, catchy_msg, pCGI->SLI.lower_catchy_bgcolor, pCGI->SLI.lower_catchy_fgcolor);
			cct_getxy(posx, posy);
		}
		if(msg)
			cct_showstr(posx, posy, msg, pCGI->SLI.lower_normal_bgcolor, pCGI->SLI.lower_normal_fgcolor);
		while(length --> 0)
			showc(' ');
	}
	showln();
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���ʾĳһ��ɫ��(����Ϊ�ַ���������Ϊrow/col)
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int row_no						���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
		   const int col_no						���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
		   const int bdi_value						����Ҫ��ʾ��ֵ
		   const BLOCK_DISPLAY_INFO *const bdi		����Ÿ�ֵ��Ӧ����ʾ��Ϣ�Ľṹ������
  �� �� ֵ��
  ˵    ����1��BLOCK_DISPLAY_INFO �ĺ����ͷ�ļ����÷��ο���������
            2��bdi_valueΪ BDI_VALUE_BLANK ��ʾ�հ׿飬Ҫ���⴦��
***************************************************************************/
int gmw_draw_block(const CONSOLE_GRAPHICS_INFO *const pCGI, const int row_no, const int col_no, const int bdi_value, const BLOCK_DISPLAY_INFO *const bdi)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��ƶ�ĳһ��ɫ��
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int row_no						���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
		   const int col_no						���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
		   const int bdi_value						����Ҫ��ʾ��ֵ
		   const int blank_bdi_value				���ƶ����������ڶ���Ч����ʾʱ���ڱ�ʾ�հ׵�ֵ��һ��Ϊ0���˴���Ϊ�������룬�ǿ��ǵ����ܳ��ֵ����������
		   const BLOCK_DISPLAY_INFO *const bdi		�������ʾֵ/�հ�ֵ��Ӧ����ʾ��Ϣ�Ľṹ������
		   const int direction						���ƶ�����һ�����֣������cmd_gmw_tools.h
		   const int distance						���ƶ����루��1��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int gmw_move_block(const CONSOLE_GRAPHICS_INFO *const pCGI, const int row_no, const int col_no, const int bdi_value, const int blank_bdi_value, const BLOCK_DISPLAY_INFO *const bdi, const int direction, const int distance)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������̻����
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   int &MAction							��������� CCT_MOUSE_EVENT�����ֵ��Ч��Ϊ MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK/MOUSE_RIGHT_BUTTON_CLICK ����֮һ
		                                               ������� CCT_KEYBOARD_EVENT�����ֵ��Ч
		   int &MRow								��������� CCT_MOUSE_EVENT �� MAction = MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK�����ֵ��Ч����ʾ���ѡ�����Ϸ������кţ���0��ʼ��
												  ���������ֵ��Ч�����������Чֵ���´��󣬲��Ǳ������Ĵ�!!!��
		   int &MCol								��������� CCT_MOUSE_EVENT �� MAction = MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK�����ֵ��Ч����ʾ���ѡ�����Ϸ������кţ���0��ʼ��
												  ���������ֵ��Ч�����������Чֵ���´��󣬲��Ǳ������Ĵ�!!!��
		   int &KeyCode1							��������� CCT_KEYBOARD_EVENT�����ֵ��Ч��Ϊ�����ļ��루���˫���룬��Ϊ��һ����
												  ���������ֵ��Ч�����������Чֵ���´��󣬲��Ǳ������Ĵ�!!!��
		   int &KeyCode2							��������� CCT_KEYBOARD_EVENT�����ֵ��Ч��Ϊ�����ļ��루���˫���룬��Ϊ�ڶ���������ǵ����룬��Ϊ0��
												  ���������ֵ��Ч�����������Чֵ���´��󣬲��Ǳ������Ĵ�!!!��
		   const bool update_lower_status_line		������ƶ�ʱ���Ƿ�Ҫ�ڱ���������ʾ"[��ǰ���] *��*��/λ�÷Ƿ�"����Ϣ��true=��ʾ��false=����ʾ��ȱʡΪtrue��
  �� �� ֵ����������Լ��
		   1�����������ƶ����õ���MRow/MCol�봫�����ͬ(���ָ��΢С���ƶ�)���򲻷��أ�������
							  �õ����зǷ�λ�ã��򲻷��أ����� update_lower_status_line ����������״̬����ʾ"[��ǰ���] λ�÷Ƿ�"
							  �õ���MRow/MCol�봫��Ĳ�ͬ(��������һ���仯)������ update_lower_status_line ����������״̬����ʾ"[��ǰ���] *��*��"���ٷ���MOUSE_ONLY_MOVED����Щ��Ϸ���غ�Ҫ����ɫ��Ĳ�ͬ��ɫ��ʾ��
		   2������ǰ������������ҵ�ǰ���ָ��ͣ��������Ϸ�����*��*���ϣ��򷵻� CCT_MOUSE_EVENT ��MAction Ϊ MOUSE_LEFT_BUTTON_CLICK, MRow Ϊ�кţ�MCol Ϊ�б�
		                          �ҵ�ǰ���ָ��ͣ���ڷǷ����򣨷���Ϸ������Ϸ�����еķָ��ߣ����򲻷��أ����� update_lower_status_line ����������״̬����ʾ"[��ǰ���] λ�÷Ƿ�"
		   3������ǰ�������Ҽ������ж����ָ��ͣ�������Ƿ�Ϸ���ֱ�ӷ��� CCT_MOUSE_EVENT ��MAction Ϊ MOUSE_RIGHT_BUTTON_CLICK, MRow��MColȡ��ǰֵ����Ϊ�������ǵ��Ҽ������Ҫ���꣩
		   4��������¼����ϵ�ĳ������˫���밴��������ֱ�ӷ��� CCT_KEYBOARD_EVENT��KeyCode1/KeyCode2��Ϊ��Ӧ�ļ���ֵ
 ˵    ����ͨ������ cmd_console_tools.cpp �е� read_keyboard_and_mouse ����ʵ��
***************************************************************************/
int gmw_read_keyboard_and_mouse(const CONSOLE_GRAPHICS_INFO *const pCGI, int &MAction, int &MRow, int &MCol, int &KeyCode1, int &KeyCode2, const bool update_lower_status_line)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	return 0; //�˾�ɸ�����Ҫ�޸�
}
