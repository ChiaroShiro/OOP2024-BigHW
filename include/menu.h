/*2351871 郎若谷 计科*/


/*
 *
 * menu.h
 *
 * 提供了通用的游戏菜单显示函数
 *
 */

#pragma once

const int __MENU_INIT_X = 150;
const int __MENU_INIT_Y = 50;

/*
 * optmin:可选择项的起始位置
 * len:每一行的最大长度
 * context:字符串内容(最后必须留有一行空行)
 * optq:特殊选项（可以没有）
 * qcontent:特殊选项内容
 * 不区分大小写
 * 返回选择选项的编号（从 1 开始计数）
 * 如果选择了 optq 则返回 -1
 */
int showMenu(int optmin, const char* content[], int optq = -1, const char* qcontent = NULL);