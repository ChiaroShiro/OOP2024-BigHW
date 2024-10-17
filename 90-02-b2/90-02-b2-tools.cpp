/* 2351871 ������ �ƿ� */

#include <iostream>
#include <iomanip>
#include <cstring>
#include <cmath>
#include <ctime>
#include <windows.h>
#include <conio.h>
#include "../include/cmd_console_tools.h"
#include "../include/cmd_gmw_tools.h"
#include "../include/io_tools.h"
#include "../include/matrix.h"
#include "../include/menu.h"
#include "90-02-b2-head.h"
using namespace std;


/*
 * ����ʣ�����ռ�
 */
int calcRestSize(CONSOLE_GRAPHICS_INFO* const pCGI, int sta[MAP_SIZE][MAP_SIZE])
{
	int size = 0;
	for (int i = 0; i < pCGI->row_num; i++) {
		for (int j = 0; j < pCGI->col_num; j++)
			size += (sta[i][j] == STA_VOID);
	}
	return size;
}


/*
 * ���ռ��Ƿ�����
 */
bool checkFull(CONSOLE_GRAPHICS_INFO* const pCGI, int sta[MAP_SIZE][MAP_SIZE])
{
	return !calcRestSize(pCGI, sta);
}

/*
 * ����Ƿ��޷����ƶ���
 */
bool checkFail(CONSOLE_GRAPHICS_INFO* const pCGI, int map[MAP_SIZE][MAP_SIZE], int sta[MAP_SIZE][MAP_SIZE])
{
	if (calcRestSize(pCGI, sta))
		return 0;
	for (int i = 0; i < pCGI->row_num; i++) {
		for (int j = 0; j < pCGI->col_num; j++) {
			for (int k = 0; k < 4; k++) {
				int bx = i + forwardx[k];
				int by = j + forwardy[k];
				if (inBorder(pCGI, bx, by)) {
					if (map[i][j] == map[bx][by])
						return 0;
				}
			}
		}
	}
	return 1;
}

int calcScore(CONSOLE_GRAPHICS_INFO* const pCGI, int map[MAP_SIZE][MAP_SIZE])
{
	int ret = 0;
	for (int i = 0; i < pCGI->row_num; i++) {
		for (int j = 0; j < pCGI->col_num; j++)
			ret = max(ret, map[i][j]);
	}
	return ret;
}

bool inBorder(CONSOLE_GRAPHICS_INFO* const pCGI, int x, int y)
{
	return x >= 0 && y >= 0 && x < pCGI->row_num && y < pCGI->col_num;
}