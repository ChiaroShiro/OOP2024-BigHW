/* 2351871 ������ �ƿ� */
#pragma once

//����������κ�ͷ�ļ����ر���<Windows.h>���鵽����0�������ǵ���-20 ������

//���в���������ϣ��˽������⼸��Ԥ������������ã����ܷ�������ҵ�������㣡����
//#pragma pack(show) //�Ծ�����Ϣ����ʽ��ʾ��ǰ�ֽڶ����ֵ
//#pragma pack(push) //����ǰ�ֽڶ���ֵѹ�������ջ��ջ��
//#pragma pack(push, 4) //����ǰ�ֽڶ���ֵѹ�������ջ��ջ����Ȼ���ٽ�4���õ�ǰֵ
//#pragma pack(pop)  //��������ջջ�����ֽڶ���ֵ����������Ϊ��ǰֵ
//#pragma pack() //���������ǻָ�Ĭ��ֵ

//������������Ҫ�Ľṹ�壨�ࣩ����������������

class Color {
public:
	char data[4];
	int sz;
	Color();

	int red();
	int green();
	int blue();
};

class bitmap {
private:
	//�����������Ҫ�����ݳ�Ա����Ա����������������
	int w; // ͼƬ���
	int h; // ͼƬ�߶�
	int bpp; // ÿһ�������ж���λ
	int cpsz; // ��ɫ���С
	Color colp[300]; // ��ɫ��
	Color** pic; // ����
	
public:
	//���в��ֲ���������κ�����
	bitmap(const char* const filename);
	~bitmap();
	int show(const int top_left_x, const int top_left_y, const int angle, const bool is_mirror, 
				void (*draw_point)(const int x, const int y, const unsigned char red, const unsigned char green, const unsigned char blue)) const;
};
