#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>
#include <fstream>
#include "../include/class_aat.h"
#include "hw_check.h"
#include "debug.h"

using namespace std;

bool checkFileExist(string path)
{
	ifstream fin(path);
#if DEBUG_MODE
    cout << "checkFileExist: " << path << endl;
#endif
	return fin.is_open();
}

string getFileSuffix(string path)
{
	return path.substr(path.find_last_of('.') + 1);
}

static bool IsUTF8(const void* pBuffer, long size) 
{
    bool isUTF8 = true;
    unsigned char* start = (unsigned char*)pBuffer;
    unsigned char* end = start + size;
    bool hasNonAscii = false;

    while (start < end) {
        if (*start < 0x80) { // ASCII�ַ�
            start++;
            continue;
        }
        
        hasNonAscii = true;
        if (*start < 0xC0) { // ��ЧUTF-8�ַ�
            isUTF8 = false;
            break;
        }
        else if (*start < 0xE0) { // 2�ֽ�UTF-8�ַ�
            if (start >= end - 1)
                break;
            if ((start[1] & 0xC0) != 0x80) {
                isUTF8 = false;
                break;
            }
            start += 2;
        }
        else if (*start < 0xF0) { // 3�ֽ�UTF-8�ַ�
            if (start >= end - 2)
                break;
            if ((start[1] & 0xC0) != 0x80 || (start[2] & 0xC0) != 0x80) {
                isUTF8 = false;
                break;
            }
            start += 3;
        }
        else if (*start < 0xF8) { // 4�ֽ�UTF-8�ַ�
            if (start >= end - 3)
                break;
            if ((start[1] & 0xC0) != 0x80 || (start[2] & 0xC0) != 0x80 || (start[3] & 0xC0) != 0x80) {
                isUTF8 = false;
                break;
            }
            start += 4;
        }
        else {
            isUTF8 = false;
            break;
        }
    }

    // ����ļ���ֻ��ASCII�ַ�,�򷵻�false
    return hasNonAscii && isUTF8;
}

bool checkIsUTF8(string path) 
{
    ifstream fin(path, ios::binary);
    if (!fin.is_open())
        return false;
    
    fin.seekg(0, ios::end);
    long size = long(fin.tellg());
    fin.seekg(0, ios::beg);

    char* buffer = new char[size];
	if(buffer == nullptr)
		return false;
    fin.read(buffer, size);
    fin.close();

    // ����Ƿ�ΪUTF-8����
    bool result = IsUTF8(buffer, size);
    delete[] buffer;

    return result;
}

bool checkIsMacFile(string path)
{
    ifstream fin(path, ios::binary);
    if (!fin.is_open())
        return false;
    
    fin.seekg(0, ios::end);
    long size = long(fin.tellg());
    fin.seekg(0, ios::beg);

    char* buffer = new char[size];
    if(buffer == nullptr)
        return false;
    fin.read(buffer, size);
    fin.close();

    // �ȼ���Ƿ���\r
    bool hasCR = false;
    for(long i = 0; i < size; i++) {
        if(buffer[i] == '\r') {
            hasCR = true;
            break;
        }
    }

    // �����\r,�ټ���Ƿ���\n
    if(hasCR) {
        for(long i = 0; i < size; i++) {
            if(buffer[i] == '\n') {
                delete[] buffer;
                return false; // ��\n�ͷ���false
            }
        }
        delete[] buffer;
        return true; // ��\rû��\n����true
    }

    delete[] buffer;
    return false; // û��\r����false
}

