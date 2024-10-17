/* 2351871 郎若谷 计科 */

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

int main() 
{
	cct_setcolor();
	cct_setfontsize("新宋体", 16);
	cct_setcursor(CURSOR_INVISIBLE);
	srand((unsigned int)time(0));
	int n, m, v, p;
	while (1) {
		cct_setconsoleborder(110, 35);
		cct_cls();
		n = getLineNumber(3, 7, "请输入行数（3-7）:");
		m = getLineNumber(3, 7, "请输入列数（3-7）:");
		v = getLineNumber(0, 2, "请选择移动速度（0-2：0为快速，1为中速，2为慢速）");
		p = getLineNumber(0, 2048, "请选择新出现块的数值上限（0-2048，推荐为4，输入的数若非2的整数次方则上取整）");
		for (int i = 0; i < 11; i++) {
			if (NUM2K[i] >= p) {
				p = NUM2K[i];
				break;
			}
		}
		if (v == 2)
			v = 10;
		game2048(n, m, v, p);
	}
	return 0;
}