/* 2351871 郎若谷 计科 */
#include <iostream>
#include <fstream>
//不再允许加入任何头文件，特别是<Windows.h>，查到就是0分甚至是倒扣-20!!!!!
using namespace std;

#include "15-b8-bmp.h"

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：按需补充
***************************************************************************/
bitmap::bitmap(const char *const filename)
{
	fstream fin;
	fin.open(filename);
	if (fin.is_open() == 0) {
		cout << "打开文件[" << filename << "]失败.\n";
		return;
	}
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：按需补充
***************************************************************************/
bitmap::~bitmap()
{
}

//按需增加内容

int bitmap::show(const int top_left_x, const int top_left_y, const int angle, const bool is_mirror,
	void (*draw_point)(const int x, const int y, const unsigned char red, const unsigned char green, const unsigned char blue)) const 
{
	
	return 0;
}