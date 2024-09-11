/*2351871 郎若谷 计科*/
#include <iostream>
#include <iomanip>
#include <cstring>
#include <cmath>
#include <ctime>
#include <windows.h>
#include <conio.h>
#include "../include/cmd_console_tools.h"
#include "../include/menu.h"
#include "../include/io_tools.h"
#include "90-01-b2-magic_ball.h"
using namespace std;

const char* MENU_CONTENT[] = {
	"内部数组，生成初始状态，寻找是否有初始可消除项",
	"内部数组，消除初始可消除项后非0项下落并用0填充",
	"内部数组，消除初始可消除项后查找消除提示",
	"n * n的框架(无分隔线)，显示初始状态",
	"n * n的框架(有分隔线)，显示初始状态",
	"n * n的框架(无分隔线)，显示初始状态及初始可消除项",
	"n * n的框架(有分隔线)，消除初始可消除项后显示消除提示",
	"cmd图形界面完整版(有分隔线，鼠标移动时显示坐标，右键退出)",
	"cmd图形界面完整版",
	""
};

int main()
{
	cct_setfontsize("新宋体", 28);
	cct_setcursor(CURSOR_INVISIBLE);
	int n, m;
	srand((unsigned int)time(0));
	while (1) {
		cct_setconsoleborder(110, 25);
		int opt = showMenu('1', MENU_CONTENT, '0', "退出");
		if (opt == -1) {
			programExit();
			break;
		}
		cct_cls();
		n = getLineNumber(5, 9, "请输入行数(5-9)：");
		m = getLineNumber(5, 9, "请输入列数(5-9)：");
		if (opt == 1)
			baseOptionInit(n, m);
		else if (opt == 2)
			baseOptionDelete(n, m);
		else if (opt == 3)
			baseOptionDeleteHint(n, m);
		else if (opt == 4)
			graphOptionInitBorder(n, m, 0);
		else if (opt == 5)
			graphOptionInitBorder(n, m, 1);
		else if (opt == 6)
			graphOptionDelete(n, m);
		else if (opt == 7)
			graphOptionDeleteHint(n, m);
		else if (opt == 8)
			mainWork(n, m, 0);
		else
			mainWork(n, m);
	}
	return 0;
}