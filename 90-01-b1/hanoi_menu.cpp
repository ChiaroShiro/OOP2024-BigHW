/* 2351871 ������ �ƿ� */
#include <iostream>
#include <conio.h>
#include "../include/cmd_console_tools.h"
using namespace std;

int menu()
{
	cct_cls();
	cout << "---------------------------------" << endl;
	cout << "1.������" << endl;
	cout << "2.������(������¼)" << endl;
	cout << "3.�ڲ�������ʾ(����)" << endl;
	cout << "4.�ڲ�������ʾ(����+����)" << endl;
	cout << "5.ͼ�ν�-Ԥ��-������Բ��" << endl;
	cout << "6.ͼ�ν�-Ԥ��-����ʼ���ϻ�n������" << endl;
	cout << "7.ͼ�ν�-Ԥ��-��һ���ƶ�" << endl;
	cout << "8.ͼ�ν�-�Զ��ƶ��汾" << endl;
	cout << "9.ͼ�ν�-��Ϸ��" << endl;
	cout << "0.�˳�" << endl;
	cout << "---------------------------------" << endl;
	cout << "[��ѡ��:] ";
	char c = _getch();
	while (c < '0' || c > '9')
		c = _getch();
	cout << c << "\n\n\n";
	return c - '0';
}