/* 2351871 ������ �ƿ� */
#include <iostream>
#include <fstream>
//������������κ�ͷ�ļ����ر���<Windows.h>���鵽����0�������ǵ���-20!!!!!
using namespace std;

#include "15-b8-bmp.h"

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    �������貹��
***************************************************************************/
bitmap::bitmap(const char *const filename)
{
	fstream fin;
	fin.open(filename);
	if (fin.is_open() == 0) {
		cout << "���ļ�[" << filename << "]ʧ��.\n";
		return;
	}
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    �������貹��
***************************************************************************/
bitmap::~bitmap()
{
}

//������������

int bitmap::show(const int top_left_x, const int top_left_y, const int angle, const bool is_mirror,
	void (*draw_point)(const int x, const int y, const unsigned char red, const unsigned char green, const unsigned char blue)) const 
{
	
	return 0;
}