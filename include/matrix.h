/*2351871 ������ �ƿ�*/


/*
 * 
 * matrix.h
 * 
 * �ṩ�˶�ά������Ϸ��ͨ�õĲ��������Ͷ�������
 * 
 */

#include "../include/cmd_gmw_tools.h"

#pragma once

#define STA_NORMAL			0
#define STA_NEED_DEL		1
#define STA_NOW_DEL			2
#define STA_SWAP			3
#define STA_VOID			4
#define STA_NEW				5
#define STA_CHOOSE			6

#define MAP_SIZE			20

#define BUP					0
#define BMID				1
#define BDOWN				2
 
#define CHOLLOW				0
#define CSOLID				1
#define CDOUBLE				2
#define CFOUR				3
#define CVOID				4


const int forwardx[] = { 0, 0, 1, -1 };
const int forwardy[] = { 1, -1, 0, 0 };

const char* const DOUBLE_HEAD[] = {"�X", "�d","�^"};
const char* const DOUBLE_TRAN[] = { "�j", "�p", "�m" };
const char* const DOUBLE_TAIL[] = { "�[","�g","�a" };
const char* const DOUBLE_LINE = "�T";
const char* const DOUBLE_VERT = "�U";

const char* const SINGLE_HEAD[] = { "��", "��","��" };
const char* const SINGLE_TRAN[] = { "��", "��", "��" };
const char* const SINGLE_TAIL[] = { "��","��","��" };
const char* const SINGLE_LINE = "��";
const char* const SINGLE_VERT = "��";

const char* const CORE[] = { "��","��","��","��","  " };

class StyleCSS {
private:
	const char* head[3];	// ����
	const char* tran[3];	// �м��
	const char* tail[3];	// ��β
	const char* line;		// �к�
	const char* vert;		// ����
	int now;				// ����Ҫ������ָ�ʽ

public:
	StyleCSS();
	void setHead(const char* const news[]);
	void setTail(const char* const news[]);
	void setTran(const char* const news[]);
	void setLine(const char* const news);
	void setVert(const char* const news);
	void setOutput(int kind);
	const char* getHead();
	const char* getTail();
	const char* getLine();
	const char* getTran();
	const char* getVert();
};

/*
 * ����ĳ��Ԫ������Ļ�ϵ�λ��
 * ����Ԫ�� (i,j) ����Ļ�Ͻ����������ʼλ�ã�����洢�� (x,y) �У�x ��������y ������
 * showBorder Ϊ 1 �����б߽磬Ϊ 0 �����ޱ߽�
 * cn �� cm ��ÿ��Ԫ�ص�����������
 */
void getpos(int i, int j, int* x, int* y, int addx, int addy, int cn, int cm);
void rgetpos(int i, int j, int* x, int* y, int addx, int addy, int cn, int cm);

/*
 * �������һ�� n*m ���󣬴洢�� map ��
 * ÿ��Ԫ�ص�ֵ��Ϊ [1, cates]
 */
void generate(int n, int m, int map[][MAP_SIZE], int cates);

/*
 * ��� n*m �ľ��� sta
 * ����Ԫ�ر�� x
 */
void clearStatus(int n, int m, int sta[][MAP_SIZE], int x, int beginpos = 1);

/*
 * �� n*m ������ sta �� ori ȫ������ trans
 */
void TransferStatus(int n, int m, int sta[][MAP_SIZE], int ori, int trans, int beginpos = 1);

/*
 * �� n*m �ľ����д� (x,y) �㿪ʼ������Ѱ����ɫ��mapֵ����ͬ��״̬��staֵ�������Ϊ STA_NEED_DEL ��Ԫ��
 * ������Ԫ��ɾ����״̬��sta�������Ϊ STA_NOW_DEL
 * ɾ���������� delBall(x, y, col) ����
 * 
 * delBall����ǰ��������������ɾλ�õ�����(x,y)������������Ϊ���λ�õ���ɫ��mapֵ��
 */
void dfsFindBlock(int x, int y, int n, int m, int showBorder, const int map[][MAP_SIZE], int sta[][MAP_SIZE], 
	              void (*delBall)(int, int, int, int));

void dfsFindBlock(int x, int y, CONSOLE_GRAPHICS_INFO* const pCGI, const int map[][MAP_SIZE], int sta[][MAP_SIZE], void (*delBall)(CONSOLE_GRAPHICS_INFO* const, int, int, int));

/*
 * �� n* m �ľ����У�Ѱ������״̬��staֵ��Ϊ STA_NEED_DEL ��ͬɫ��ͨ�飬������ɾ��
 * sta ��Ϊ STA_NOW_DEL
 * ��� showGraph �� 1 ������ delBall ������ʾ����Ч��
 * 
 * n, m: �����С
 * map: ����ÿ��Ԫ�ص���ɫ��  sta: ����ÿ��Ԫ�ص�״̬
 * showGraph: �Ƿ���ö���Ч����Ĭ��Ϊ0�������ã�
 * delBall(x, y, col): ɾ��Ԫ�صĶ���������ǰ��������������ɾ��Ԫ�ص����� (x, y)������������������ɾ��Ԫ�ص���ɫ col (mapֵ)
 */
void deleteBall(int n, int m, int showBorder, int map[][MAP_SIZE], int sta[][MAP_SIZE], bool showGraph = 0, void (*delBall)(int, int, int, int) = NULL);

void deleteBall(CONSOLE_GRAPHICS_INFO* const pCGI, int map[][MAP_SIZE], int sta[][MAP_SIZE], void (*delBall)(CONSOLE_GRAPHICS_INFO* const, int, int, int));

/*
 * �����б�ɾ��Ԫ�� (sta Ϊ STA_NOW_DEL) �Ϸ���Ԫ����������ߵ�û�б�ɾ����Ԫ���Ϸ�
 * ���� sta ��Ϊ STA_VOID
 * ��� showGraph �� 1 ������ slideDownBall ������ʾ����Ч��
 * 
 * n, m: �����С
 * map: ����ÿ��Ԫ�ص���ɫ��  sta: ����ÿ��Ԫ�ص�״̬
 * showGraph: �Ƿ���ö���Ч����Ĭ��Ϊ0�������ã�
 * slideDownBall(n, m, x, y, col): ǰ��������n, m���������С�����Ĳ�������������Ԫ�ص����� (x, y)��col Ϊ��Ԫ�ص���ɫ������Ч������ʾ�����λ�õ�Ԫ�������ƶ�һ��Ķ���
 */
void fallBall(int n, int m, int map[][MAP_SIZE], int sta[][MAP_SIZE], bool showGraph = 0, int showBorder = 1, void (*slideDownBall)(int, int, int, int, int, int) = NULL);

/*
 * n*m �ľ�����ɫ���� map �У�״̬���� sta ��
 * ���������ǽ�����״̬�� STA_VOID ��λ����������
 * �����ɫ������ cates
 * ��� showGraph �� 1 �򶯻���ʾ��������̣���Ҫ���� slideDownBall
 * slideDownBall(n, m, x, y, col): ǰ��������n, m���������С�����Ĳ�������������Ԫ�ص����� (x, y)��col Ϊ��Ԫ�ص���ɫ������Ч������ʾ�����λ�õ�Ԫ�������ƶ�һ��Ķ���
 */
int fillVoidBall(int n, int m, int map[][MAP_SIZE], int sta[][MAP_SIZE],
	int cates, void (*slideDownBall)(int, int, int, int, int, int), bool showGraph = 1, int showBorder = 1);

/*
 * pCGI ��������Ϣ
 * map����ɫ��sta��״̬
 * cates���������
 * slideDownBall������һ����ĺ���
 * ��һ��������pCGI���ڶ������������꣬���ĸ���������ɫ
 */
int fillVoidBall(CONSOLE_GRAPHICS_INFO* const pCGI, int map[][MAP_SIZE], int sta[][MAP_SIZE],
	int cates, void (*slideDownBall)(CONSOLE_GRAPHICS_INFO* const, int, int, int));

/*
 * pCGI����������Ϣ
 * map����ɫ��sta��״̬��slideDownBall ����׹һ��ĺ���
 * slideDownBall(CGI* const pCGI, int x, int y, int val)
 * pCGI��������Ϣ����ɴ�(x,y)���µ�һ��׹���������ɫ�� val
 */
void fallBall(CONSOLE_GRAPHICS_INFO* const pCGI, int map[][MAP_SIZE], int sta[][MAP_SIZE], void (*slideDownBall)(CONSOLE_GRAPHICS_INFO* const, int, int, int));

/*
 * ���Ʊ�����ܣ�����Ϊ n*m ��С��showBorder Ϊ 1 ����Ʊ߿�
 * totx �� toty �洢���±仯�����Ļ��С
 * coren �� corem ����ÿ��Ԫ�صĴ�С
 * gap Ϊ����ÿ��Ԫ�غ�ļ��
 */
void drawBackground(int n, int m, bool showBorder, int showFrame, int* totx, int* toty,
	int coren, int corem, StyleCSS style, int gap = 0, int times = 1);

/*
 * �޶���ѡ���µ����ֻ滭������
 * n*m�ľ��� map �� sta ��s Ϊ�����ͷ��
 */
void drawCanvas(int n, int m, const int map[][MAP_SIZE], const int sta[][MAP_SIZE], const char* s, int colorTag = -1);