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

template <size_t _SZ>
class Color {
public:
	char data[_SZ];
	Color();

	int oneColorBits();
	int colorBits();
	int exBits();
};

class Picture {

};

class bitmap {
private:
	//�����������Ҫ�����ݳ�Ա����Ա����������������
	int w; // ͼƬ���
	int h; // ͼƬ�߶�
	int bpp; // ÿһ�������ж���λ
	bool haveColp; // �Ƿ��е�ɫ��
	Color <32> colp[256]; // ��ɫ��
	
public:
	//���в��ֲ���������κ�����
	bitmap(const char* const filename);
	~bitmap();
	int show(const int top_left_x, const int top_left_y, const int angle, const bool is_mirror, 
				void (*draw_point)(const int x, const int y, const unsigned char red, const unsigned char green, const unsigned char blue)) const;
};
