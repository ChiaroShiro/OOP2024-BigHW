/*2351871 ������ �ƿ�*/


/*
 *
 * menu.h
 *
 * �ṩ��ͨ�õ���Ϸ�˵���ʾ����
 *
 */

#pragma once

const int __MENU_INIT_X = 150;
const int __MENU_INIT_Y = 50;

/*
 * optmin:��ѡ�������ʼλ��
 * len:ÿһ�е���󳤶�
 * context:�ַ�������(����������һ�п���)
 * optq:����ѡ�����û�У�
 * qcontent:����ѡ������
 * �����ִ�Сд
 * ����ѡ��ѡ��ı�ţ��� 1 ��ʼ������
 * ���ѡ���� optq �򷵻� -1
 */
int showMenu(int optmin, const char* content[], int optq = -1, const char* qcontent = NULL);