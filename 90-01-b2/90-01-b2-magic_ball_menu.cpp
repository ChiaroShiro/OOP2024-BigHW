/*2351871 ������ �ƿ�*/
#include <iostream>
#include <iomanip>
#include <cstring>
#include <cmath>
#include <ctime>
#include <windows.h>
#include <conio.h>
#include "../include/cmd_console_tools.h"
#include "90-01-b2-magic_ball.h"
using namespace std;

int __showMenu()
{
	cct_cls();
	cct_setconsoleborder(INIT_X_SIZE, INIT_Y_SIZE);
	cout << "\
------------------------------------------------------------\n\
1.�ڲ����飬���ɳ�ʼ״̬��Ѱ���Ƿ��г�ʼ��������\n\
2.�ڲ����飬������ʼ����������0�����䲢��0���\n\
3.�ڲ����飬������ʼ������������������ʾ\n\
4.n * n�Ŀ��(�޷ָ���)����ʾ��ʼ״̬\n\
5.n * n�Ŀ��(�зָ���)����ʾ��ʼ״̬\n\
6.n * n�Ŀ��(�޷ָ���)����ʾ��ʼ״̬����ʼ��������\n\
7.n * n�Ŀ��(�зָ���)��������ʼ�����������ʾ������ʾ\n\
8.cmdͼ�ν���������(�зָ��ߣ�����ƶ�ʱ��ʾ���꣬�Ҽ��˳�)\n\
9.cmdͼ�ν���������(��δ��ɣ��ᾡ�첹��)\n\
0.�˳�\n\
------------------------------------------------------------\n\
[��ѡ��:] ";
	char c = getc();
	while (c < '0' || c > '9')
		c = getc();
	cout << c << '\n';
	wait(200);
	return c - '0';
}