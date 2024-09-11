/* 2351871 郎若谷 计科 */
#include <iostream>
#include <windows.h>
#include "../include/cmd_console_tools.h"
#include "../include/menu.h"
using namespace std;

const char* MENU_CONTENT[] = {
	"命令行找出可消除项并标识",
	"命令行完成一次消除（分步骤显示）",
	"命令行完成一关（分步骤显示）",
	"伪图形界面下用鼠标选择一个色块（无分隔线）",
	"伪形界面下用鼠标选择一个色块（有分隔线）",
	"伪图形界面完成一次消除（分步骤）",
	"伪图形界面完整版",
	""
};

int main()
{
	cct_setfontsize("新宋体", 28);
	cct_setcursor(CURSOR_INVISIBLE);
	srand((unsigned int)time(0));
	while (1) {
		cct_setconsoleborder(110, 25);
		int opt = showMenu('a', MENU_CONTENT, 'q', "退出");
		
	}
	return 0;
}
