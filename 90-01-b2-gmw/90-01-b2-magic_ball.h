/*2351871 ������ �ƿ�*/
#pragma once

#include "../include/matrix.h"
#include "../include/cmd_gmw_tools.h"

#define INIT_X_SIZE			100
#define INIT_Y_SIZE			25
#define X_LIM				40
#define Y_LIM				14
#define SMALL_GAP			5
#define MID_GAP				30
#define BIG_GAP				400

#define BALL_CATEGORY_SIZE	9

#define CLICK_UNABLE		0
#define CLICK_SINGLE		1
#define CLICK_DOUBLE		2

const BLOCK_DISPLAY_INFO BDI_NORMAL[] = {
		{BDI_VALUE_BLANK, -1, -1, "  "},  //0����ʾ���ÿո���伴��
		{1, COLOR_HBLACK, -1, "��"},
		{2, COLOR_YELLOW, -1, "��"},
		{3, COLOR_HGREEN, -1, "��"},
		{4, COLOR_HCYAN, -1, "��"},
		{5, COLOR_HRED, -1, "��"},
		{6, COLOR_HPINK, -1, "��"},
		{7, COLOR_HYELLOW, -1, "��"},
		{8, COLOR_CYAN, -1, "��"},
		{9, COLOR_BLUE, -1, "��"},
		{BDI_VALUE_END, -1, -1, NULL} //�жϽ�������Ϊ-999
};
/* ����1-9�������ú�����ʽ��ʾ�ڽ����ϣ�ѡ��״̬�� */
const BLOCK_DISPLAY_INFO BDI_SELECTED[] = {
	{BDI_VALUE_BLANK, -1, -1, "  "},  //�հ�
	{1, COLOR_HBLACK, -1, "��"},
	{2, COLOR_YELLOW, -1, "��"},
	{3, COLOR_HGREEN, -1, "��"},
	{4, COLOR_HCYAN, -1, "��"},
	{5, COLOR_HRED, -1, "��"},
	{6, COLOR_HPINK, -1, "��"},
	{7, COLOR_HYELLOW, -1, "��"},
	{8, COLOR_CYAN, -1, "��"},
	{9, COLOR_BLUE, -1, "��"},
	{BDI_VALUE_END, -1, -1, NULL} //�жϽ�������Ϊ-999
};
/* ����1-9�������ú�����ʽ��ʾ�ڽ����ϣ���������ʾ״̬�� */
const BLOCK_DISPLAY_INFO BDI_PROMPT[] = {
	{BDI_VALUE_BLANK, -1, -1, "  "},  //�հ�
	{1, COLOR_HBLACK, -1, "��"},
	{2, COLOR_YELLOW, -1, "��"},
	{3, COLOR_HGREEN, -1, "��"},
	{4, COLOR_HCYAN, -1, "��"},
	{5, COLOR_HRED, -1, "��"},
	{6, COLOR_HPINK, -1, "��"},
	{7, COLOR_HYELLOW, -1, "��"},
	{8, COLOR_CYAN, -1, "��"},
	{9, COLOR_BLUE, -1, "��"},
	{BDI_VALUE_END, -1, -1, NULL} //�жϽ�������Ϊ-999
};
/* ����1-9�������ú�����ʽ��ʾ�ڽ����ϣ���ը/����״̬�� */
const BLOCK_DISPLAY_INFO BDI_EXPLODED[] = {
	{BDI_VALUE_BLANK, -1, -1, "  "},  //�հ�
	{1, COLOR_HBLACK, -1, "��"},
	{2, COLOR_YELLOW, -1, "��"},
	{3, COLOR_HGREEN, -1, "��"},
	{4, COLOR_HCYAN, -1, "��"},
	{5, COLOR_HRED, -1, "��"},
	{6, COLOR_HPINK, -1, "��"},
	{7, COLOR_HYELLOW, -1, "��"},
	{8, COLOR_CYAN, -1, "��"},
	{9, COLOR_BLUE, -1, "��"},
	{BDI_VALUE_END, -1, -1, NULL} //�жϽ�������Ϊ-999
};
/* �ɹ�ѡ��״̬��bdi */
const BLOCK_DISPLAY_INFO BDI[] = {
	{BDI_VALUE_BLANK, -1, -1, "  "},  //�հ�
	{1, COLOR_HBLACK, COLOR_BLACK, "��"},
	{2, COLOR_YELLOW, COLOR_BLACK, "��"},
	{3, COLOR_HGREEN, COLOR_BLACK, "��"},
	{4, COLOR_HCYAN, COLOR_BLACK, "��"},
	{5, COLOR_HRED, COLOR_BLACK, "��"},
	{6, COLOR_HPINK, COLOR_BLACK, "��"},
	{7, COLOR_HYELLOW, COLOR_BLACK, "��"},
	{8, COLOR_CYAN, COLOR_BLACK, "��"},
	{9, COLOR_BLUE, COLOR_BLACK, "��"},
	{11, COLOR_HBLACK, COLOR_WHITE, "��"},
	{12, COLOR_YELLOW, COLOR_WHITE, "��"},
	{13, COLOR_HGREEN, COLOR_WHITE, "��"},
	{14, COLOR_HCYAN, COLOR_WHITE, "��"},
	{15, COLOR_HRED, COLOR_WHITE, "��"},
	{16, COLOR_HPINK, COLOR_WHITE, "��"},
	{17, COLOR_HYELLOW, COLOR_WHITE, "��"},
	{18, COLOR_CYAN, COLOR_WHITE, "��"},
	{19, COLOR_BLUE, COLOR_WHITE, "��"},
	{BDI_VALUE_END, -1, -1, NULL} //�жϽ�������Ϊ-999
};

struct Pair {
	int x;
	int y;
};

void mainWork(int n, int m);

bool findBlock(int n, int m, int map[][MAP_SIZE], int res[][MAP_SIZE]);
bool findAvailable(int n, int m, int map[][MAP_SIZE], int sta[][MAP_SIZE]);
bool tryFindBlock(int n, int m, int map[][MAP_SIZE]);
bool tryFindAvailable(int n, int m, int map[][MAP_SIZE]);

void eliminateBall(int x, int y, int val, int showBorder);
void slideDownBall(int n, int m, int x, int y, int val, int showBorder);