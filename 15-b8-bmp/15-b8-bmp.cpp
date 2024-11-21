/* 2351871 郎若谷 计科 */
#include <iostream>
#include <fstream>
//不再允许加入任何头文件，特别是<Windows.h>，查到就是0分甚至是倒扣-20!!!!!
using namespace std;

#include "15-b8-bmp.h"

template <typename _T>
static _T convertStr(char* s, int len)
{
	_T ret(0);
	memcpy(&ret, s, len);
	return ret;
}

Color::Color()
{
	memset(this, 0, sizeof(*this));
}

int Color::red()
{
	return convertStr <int>(this->data, 1);
}

int Color::green()
{
	return convertStr <int>(this->data + 1, 1);
}

int Color::blue()
{
	return convertStr <int>(this->data + 2, 1);
}

static Color** buildMatrix(int h, int w)
{
	Color** ret = new Color * [h];
	if (ret == NULL)
		return NULL;
	for (int i = 0; i < h; i++) {
		ret[i] = new Color[w];
		if (ret[i] == NULL)
			return NULL;
	}
	return ret;
}

static void destoryMatrix(Color** p, int h, int w)
{
	for (int i = 0; i < h; i++)
		delete[] p[i];
	delete[] p;
}

bitmap::bitmap(const char *const filename)
{
	memset(this, 0, sizeof(*this));
	fstream fin;
	fin.open(filename, ios::binary | ios::in);
	if (fin.is_open() == 0) {
		cout << "打开文件[" << filename << "]失败.\n";
		return;
	}
	int sz, databeg;
	fin.seekg(2, ios::cur);
	fin.read((char*)&sz, 4);

	fin.seekg(4, ios::cur);
	fin.read((char*)&databeg, 4);
	this->cpsz = (databeg - 54) / 4;

	fin.seekg(4, ios::cur);
	fin.read((char*)&this->w, 4);
	fin.read((char*)&this->h, 4);

	fin.seekg(2, ios::cur);
	fin.read((char*)&this->bpp, 2);

	if (this->cpsz) {
		fin.seekg(54, ios::beg);
		for (int i = 0; i < this->cpsz; i++)
			fin.read((char*)&this->colp[i], 4);
	}
	pic = buildMatrix(h, w);
	fin.seekg(databeg, ios::beg);

	for (int i = 0; i < h; i++) {
		int movbyt = 0;
		for (int j = 0; j < w;) {
			if (bpp <= 8) {
				char c;
				fin.read((char*)&c, 1);
				for (int k = 0; k < 8; k += bpp) {
					if (j == w)
						break;
					int num = (((unsigned int)c) >> (8 - k - bpp)) & ((1 << bpp) - 1);
					pic[i][j] = colp[num];
					++j;
				}
				movbyt++;
			}
			else {
				fin.read((char*)&pic[i][j].data, bpp / 8);
				pic[i][j].sz = bpp;
				j++;
				movbyt += bpp / 8;
			}
		}
		if(movbyt % 4)
			fin.seekg(4 - movbyt % 4, ios::cur);
	}
}

bitmap::~bitmap()
{
	destoryMatrix(pic, h, w);
}

static Color** rotate(Color** mat, int &h, int &w)
{
	Color** ret = buildMatrix(w, h);
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++)
			ret[j][h - i - 1] = mat[i][j];
	}
	destoryMatrix(mat, h, w);
	swap(h, w);
	return ret;
}

int bitmap::show(const int top_left_x, const int top_left_y, const int angle, const bool is_mirror,
	void (*draw_point)(const int x, const int y, const unsigned char red, const unsigned char green, const unsigned char blue)) const 
{
	int rh = h, rw = w;
	Color** mat = buildMatrix(rh, rw);
	for (int i = 0; i < rh; i++) {
		for (int j = 0; j < rw; j++)
			mat[i][j] = pic[i][j];
	}

	if (is_mirror) {
		for (int i = 0; i < rh; i++) {
			for (int j = 0; j < rw / 2; j++)
				swap(mat[i][j], mat[i][rw - j - 1]);
		}
	}

	for (int i = 90; i <= angle; i += 90)
		mat = rotate(mat, rh, rw);

	for (int i = 0; i < rh; i++) {
		for (int j = 0; j < rw; j++) {
			draw_point(j + top_left_x, rh - i - 1 + top_left_y, mat[i][j].blue(), mat[i][j].green(), mat[i][j].red());
		}
	}
	destoryMatrix(mat, rh, rw);
	return 0;
}