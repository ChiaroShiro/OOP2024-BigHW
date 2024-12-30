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
		if (*start < 0x80) { // ASCII字符
			start++;
			continue;
		}
		hasNonAscii = 1;
		if (*start < 0xC0) { // 无效UTF-8字符
			isUTF8 = 0;
			break;
		}
		else if (*start < 0xE0) { // 2字节UTF-8字符
			if (start >= end - 1)
				break;
			if ((start[1] & 0xC0) != 0x80) {
				isUTF8 = 0;
				break;
			}
			start += 2;
		}
		else if (*start < 0xF0) { // 3字节UTF-8字符
			if (start >= end - 2)
				break;
			if ((start[1] & 0xC0) != 0x80 || (start[2] & 0xC0) != 0x80) {
				isUTF8 = 0;
				break;
			}
			start += 3;
		}
		else if (*start < 0xF8) { // 4字节UTF-8字符
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
	// 检查是否有\r
	bool hasCR = 0;
	for(long i = 0; i < size; i++) {
		if(buffer[i] == '\r') {
			hasCR = 1;
			break;
		}
	}
	// 检查是否有\n
	if(hasCR) {
		for(long i = 0; i < size; i++) {
			if(buffer[i] == '\n') {
				delete[] buffer;
				return 0; // 有\n就返回false
			}
		}
		delete[] buffer;
		return 1; // 有\r没有\n返回true
	}
	delete[] buffer;
	return 0; // 没有\r返回false
}

/**
 * 提取文件除去空行后的第line行
 * @param path: 文件路径
 * @param line: 行号
 * @return: 返回第line行
*/
string extractLine(const string& path, int line) 
{
	ifstream fin(path);
	if(!fin.is_open())
		return "";
	string str;
	int count = 0;
	while(getline(fin, str)) {
		if(str.find_first_not_of(" \t") == string::npos) // 跳过空行
			continue;
		count++;
		if(count == line)
			return trim(str);
	}
	return "";
}

/**
 * @param path 文件路径
 * @param line 行号，取值只有 1/2
 * @return: -1: 不是注释行; -2: 是错误的多行注释; 1: 是单行注释行; 2: 是正确多行注释
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
 * 去掉字符串前后空格和制表符
 * @param str: 输入字符串
 * @return: 返回处理后的字符串
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
 * 去掉一行字符串前后空格以及注释
 * @param str: 输入字符串
 * @return: 返回处理后的字符串
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
 * 判断字符串被空格或制表符分隔的项数
 * @param str: 输入字符串
 * @return: 返回项数
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
 * 将字符串用空格隔开的若干项取出
 * @param str: 字符串
 * @return: 分割结果
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
 * 根据学号在table里查找对应的行下标
 * @param stuNo: 学号
 * @param table: 表格信息
 * @return: 该学号对应的行下标,未找到返回-1
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
 * 根据学号在table中查找对应行的所有数据
 * @param stuNo: 学号
 * @param table: 表格信息
 * @return: 该学号对应的所有数据: stu_no, stu_name, classb, classc, cno
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
