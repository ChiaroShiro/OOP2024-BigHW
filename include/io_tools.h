/* 2351871 ������ �ƿ� */


/*
 *
 * io_tools.h
 *
 * �ṩ�˳��õ�IO��غ���
 * �Լ��򵥵�ͨ�ú���
 *
 */

#pragma once

/*
 * �ж��Ƿ��Ǵ�д��ĸ
 */
bool isUpper(char c);

/*
 * �ж��Ƿ���Сд��ĸ
 */
bool isLower(char c);

/*
 * �ж��Ƿ��������ַ�
 */
bool isNumber(char c);

/*
 * ��д��ĸתСд
 */
char utol(char c);

/*
 * Сд��ĸת��д
 */
char ltou(char c);

/*
 * ����תСд��ĸ
 */
char itol(int a);

/*
 * ����ת��д��ĸ
 */
char itou(int a);

/*
 * ����һ���ַ������ݲ��������Ƿ����
 * Ĭ�ϲ���ʾ
 */
char getcc(bool show = 0);

/*
 * �ȴ�x���룬��x=0�򲻵ȴ�
 */
void wait(int x);

/*
 * �� (x,y) ������ʾ���� v����ʾ���Ϊ w��Ĭ��Ϊ 1��
 */
void showNum(int x, int y, int v, int w = 1);

/*
 * �������������������� (x,y) ���꿪ʼ���һ�У���պ�����ݿ���ָ����Ĭ��Ϊ�հ�
 * ������һ�����������һ��ֱ����գ���պ�����ݿ���ָ����Ĭ��Ϊ�հ�
 */
void clearLine(int x, int y = -1, const char* s = "                                   ");

/*
 * ���һ�����ݺ�ȴ��û�����ָ�����ݣ��д�����ʾ
 * x: ��������ͣʱ��
 * output: �������ʾ����
 * hint: �����������ʾ����
 * input: ָ��Ҫ�����������
 * show: �����Ƿ���ԣ�Ĭ�ϻ���
 */
void waitLine(int x, const char* output, const char* hint = "", const char* input = "", bool show = 1);

/*
 * ���һ�����ݺ�ȴ��û�����ָ���ַ�
 * x: ��������ͣʱ��
 * output: �������ʾ����
 * input: ָ��Ҫ��������ַ�
 * show: �����Ƿ���ԣ�Ĭ�ϲ�����
 */
void waitLine(int x, const char* output, char input, bool show = 0);

/*
 * ��(x,y)λ�����һ���ַ���
 */
void showLine(const char* s, int x, int y);

/*
 * ��(x,y)λ�����һ���ַ�������������������λ�ô��� (retx, rety) ��
 * ����ָ��xy��Ĭ���ڵ�ǰ���λ�����
 */
void showLine(const char* s, int* retx, int* rety, int x = -1, int y = -1);

/*
 * ����һ��ָ����Χ�����֣����Դ�������Χ�Ѿ����ֲ��Ϸ���������
 * ָ����Χ�� [minli, maxli]
 * �����ʾ��ϢΪ msg
 */
int getLineNumber(int minli, int maxli, const char* msg);

/*
 * �����������
 */
void programExit();

/*
 * �ȴ��س����������
 */
void waitForEndlineOrClick();

/*
 * �ڶ�ά�����ĵײ��������
 */
void gotoBottom(int n, int showBorder);
void showBottom(int n, const char* s, int showBorder, int bgcol = COLOR_BLACK, int frcol = COLOR_WHITE);



/*
 * �ڵ�ǰ���λ���Ե�ǰ��ɫ���
 */
void shows(const char* s);
void showc(const char& c);
void showi(const int& p);
void showln();