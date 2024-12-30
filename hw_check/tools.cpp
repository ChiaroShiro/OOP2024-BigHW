#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include "../include/class_aat.h"
#include "hw_check.h"
#include "debug.h"
using namespace std;

bool checkFileExist(const string& path)
{
	ifstream fin(path);
	return fin.is_open();
}

string getFileSuffix(const string& path)
{
	return path.substr(path.find_last_of('.') + 1);
}

static bool IsUTF8(const void* pBuffer, long size) 
{
	bool isUTF8 = 1;
	unsigned char* start = (unsigned char*)pBuffer;
	unsigned char* end = start + size;
	bool hasNonAscii = 0;

	while (start < end) {
		if (*start < 0x80) { // ASCII�ַ�
			start++;
			continue;
		}
		hasNonAscii = 1;
		if (*start < 0xC0) { // ��ЧUTF-8�ַ�
			isUTF8 = 0;
			break;
		}
		else if (*start < 0xE0) { // 2�ֽ�UTF-8�ַ�
			if (start >= end - 1)
				break;
			if ((start[1] & 0xC0) != 0x80) {
				isUTF8 = 0;
				break;
			}
			start += 2;
		}
		else if (*start < 0xF0) { // 3�ֽ�UTF-8�ַ�
			if (start >= end - 2)
				break;
			if ((start[1] & 0xC0) != 0x80 || (start[2] & 0xC0) != 0x80) {
				isUTF8 = 0;
				break;
			}
			start += 3;
		}
		else if (*start < 0xF8) { // 4�ֽ�UTF-8�ַ�
			if (start >= end - 3)
				break;
			if ((start[1] & 0xC0) != 0x80 || (start[2] & 0xC0) != 0x80 || (start[3] & 0xC0) != 0x80) {
				isUTF8 = 0;
				break;
			}
			start += 4;
		}
		else {
			isUTF8 = 0;
			break;
		}
	}
	return hasNonAscii && isUTF8;
}

bool checkIsUTF8(const string& path) 
{
	ifstream fin(path, ios::binary);
	if (!fin.is_open())
		return 0;
	
	fin.seekg(0, ios::end);
	long size = long(fin.tellg());
	fin.seekg(0, ios::beg);

	char* buffer = new char[size];
	if(buffer == nullptr)
		return 0;
	fin.read(buffer, size);
	fin.close();

	bool result = IsUTF8(buffer, size);
	delete[] buffer;

	return result;
}

bool checkIsMacFile(const string& path)
{
	ifstream fin(path, ios::binary);
	if (!fin.is_open())
		return 0;
	
	fin.seekg(0, ios::end);
	long size = long(fin.tellg());
	fin.seekg(0, ios::beg);

	char* buffer = new char[size];
	if(buffer == nullptr)
		return 0;
	fin.read(buffer, size);
	fin.close();
	// ����Ƿ���\r
	bool hasCR = 0;
	for(long i = 0; i < size; i++) {
		if(buffer[i] == '\r') {
			hasCR = 1;
			break;
		}
	}
	// ����Ƿ���\n
	if(hasCR) {
		for(long i = 0; i < size; i++) {
			if(buffer[i] == '\n') {
				delete[] buffer;
				return 0; // ��\n�ͷ���false
			}
		}
		delete[] buffer;
		return 1; // ��\rû��\n����true
	}
	delete[] buffer;
	return 0; // û��\r����false
}

/**
 * ��ȡ�ļ���ȥ���к�ĵ�line��
 * @param path: �ļ�·��
 * @param line: �к�
 * @return: ���ص�line��
*/
string extractLine(const string& path, int line) 
{
	ifstream fin(path);
	if(!fin.is_open())
		return "";
	string str;
	int count = 0;
	while(getline(fin, str)) {
		if(str.find_first_not_of(" \t") == string::npos) // ��������
			continue;
		count++;
		if(count == line)
			return trim(str);
	}
	return "";
}

/**
 * @param path �ļ�·��
 * @param line �кţ�ȡֵֻ�� 1/2
 * @return: -1: ����ע����; -2: �Ǵ���Ķ���ע��; 1: �ǵ���ע����; 2: ����ȷ����ע��
 */
int checkLineIsComment(const string& path, int line) 
{
	string str = extractLine(path, line);
	if(str.empty())
		return -1;
	str = trim(str);
	if(str.substr(0, 2) == "//")
		return 1;
	if(str.substr(0, 2) == "/*") {
		if(str.substr(str.length() - 2) == "*/")
			return 2;
		else
			return -2;
	}
	return -1;
}

/**
 * ȥ���ַ���ǰ��ո���Ʊ��
 * @param str: �����ַ���
 * @return: ���ش������ַ���
*/
string trim(const string& str)
{
	if(str.empty())
		return "";
	string result = str;
	result.erase(0, result.find_first_not_of(" \t"));
	result.erase(result.find_last_not_of(" \t") + 1);
	return result;
}


/**
 * ȥ��һ���ַ���ǰ��ո��Լ�ע��
 * @param str: �����ַ���
 * @return: ���ش������ַ���
*/
string trimComment(const string& str)
{
	if(str.empty())
		return "";
	string result = trim(str);
	if(result.substr(0, 2) == "//") {
		result = result.substr(2);
		return trim(result);
	}
	if(result.substr(0, 2) == "/*" && result.substr(result.length() - 2) == "*/") {
		result = result.substr(2, result.length() - 4);
		return trim(result);
	}
	return result;
}

/**
 * �ж��ַ������ո���Ʊ���ָ�������
 * @param str: �����ַ���
 * @return: ��������
*/
int countItems(const string& str) 
{
	if(str.empty())
		return 0;
	int count = 0;
	bool inItem = 0;
	for(char c : str) {
		if(c == ' ' || c == '\t')
			inItem = 0;
		else if(!inItem) {
			inItem = 1;
			count++;
		}
	}
	return count;
}

/**
 * ���ַ����ÿո������������ȡ��
 * @param str: �ַ���
 * @return: �ָ���
 */
_VS extractItems(const string &s)
{
    _VS ret;
    int start = 0;
    bool inItem = 0;
	for (int i = 0; i < int(s.length()); i++) {
        if(s[i] == ' ' || s[i] == '\t') {
            if(inItem) {
                ret.push_back(s.substr(start, i - start));
                inItem = 0;
            }
        }
        else if(!inItem) {
            start = i;
            inItem = 1;
        }
    }
    if(inItem)
        ret.push_back(s.substr(start));   
    return ret;
}

/**
 * ����ѧ����table����Ҷ�Ӧ�����±�
 * @param stuNo: ѧ��
 * @param table: �����Ϣ
 * @return: ��ѧ�Ŷ�Ӧ�����±�,δ�ҵ�����-1
 */
int findRowIndexByStuNo(const string& stuNo, const tableInfo& table)
{
    for(int i = 0; i < int(table.stu_no.size()); i++) {
        if(table.stu_no[i] == stuNo)
            return i;
    }
    return -1;
}


/**
 * ����ѧ����table�в��Ҷ�Ӧ�е���������
 * @param stuNo: ѧ��
 * @param table: �����Ϣ
 * @return: ��ѧ�Ŷ�Ӧ����������: stu_no, stu_name, classb, classc, cno
 */
_VS findRowByStuNo(const string& stuNo, const tableInfo& table)
{
    _VS ret;
    int index = findRowIndexByStuNo(stuNo, table);
    if(index == -1)
        return ret;
    ret.push_back(table.stu_no[index]);
    ret.push_back(table.name[index]);
    ret.push_back(!table.classb.empty() ? table.classb[index] : "");
    ret.push_back(!table.classc.empty() ? table.classc[index] : "");
    ret.push_back(!table.cno.empty() ? table.cno[index] : "");
    return ret;
}
