#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>
#include "../include/class_aat.h"
#include "../include_mariadb_x86/mysql/mysql.h"
#include "hw_check.h"
#include "debug.h"

using namespace std;

static void updatePairVector(vector<Pair>& pairs, const string& result) 
{
	bool found = 0;
	for(Pair& p : pairs) {
		if(p.str == result) {
			p.num++;
			found = 1;
			break;
		}
	}
	if(!found)
		pairs.push_back({result, 1});
}

static void printSummary(vector<Pair>& pairs, string title, char separator = '=') 
{
	int maxLen = 0;
	// 删除数量为0的项
	vector<Pair> newPairs;
	for(Pair& p : pairs) {
		if(p.num > 0)
			newPairs.push_back(p);
	}
	pairs.clear();
	pairs.insert(pairs.begin(), newPairs.begin(), newPairs.end());

	for(Pair& p : pairs)
		maxLen = max(maxLen, int(p.str.length()));
	maxLen += 2;
	string sepLine(maxLen + 10, separator);

	cout << sepLine << endl;
	cout << title << endl;
	cout << sepLine << endl;

	for(Pair& p : pairs)
		cout << setw(maxLen) << right << p.str << " : " << p.num << endl;
	cout << sepLine << endl;
}

static string replaceChineseDot(string str) {
	string result;
	for(size_t i = 0; i < str.length(); i++) {
		// 检查是否为中文点号 (0xA1 0xA4 in GB2312)
		if(i + 1 < str.length() && static_cast<unsigned char>(str[i]) == 0xA1 && static_cast<unsigned char>(str[i+1]) == 0xA4) {
			result += '.';
			i++;
		} 
		else
			result += str[i];
	}
	return result;
}

/** 
 * *多文件模式下的输出函数
 * @param path : 文件路径
 * @param filenames : 检查作业文件名列表
 * @param table : 表信息
 * @param cno : 课程号（只有一个）
 * @param checker : 检查函数
 *  *返回一个用于输出的字符串，将检查的结果类型存到result中
 * 	参数：
 * 		@param path : 文件路径
 * 		@param result : 检查结果
 * 		@param table : 表信息
 * @param initPairVector : 初始化pair向量函数
 * 	*按照demo里的顺序初始化vector<Pair>
 * 	参数：
 * 		@param pairs : vector<Pair>类似于std::map的东西
*/
void multifilePrinter(const string& path, const _VS& filenames, const tableInfo& table, const string& cno, CHECKER_FUNC checker, INIT_FUNC initPairVector)
{
	const _VS &names = table.name;
	const _VS &stu_no = table.stu_no;
	int lineWidth = 0;
	for (int i = 0; i < int(filenames.size()); i++)
		lineWidth = max(lineWidth, int(filenames[i].size()));
	++lineWidth;
	vector <Pair> res;
	initPairVector(res);
	int totalFiles = 0, totalPassed = 0;
	for (int i = 0; i < int(names.size()); i++) {
		cout << setw(3) << left << i + 1 << ": " << "学号-" << stu_no[i] << " 姓名-" << replaceChineseDot(names[i]) << " 课号-" << cno << " 文件数量-" << filenames.size() << endl;
		vector <Pair> singleRes;
		initPairVector(singleRes);
		string path_stu = path + "/" + cno + '-' + stu_no[i];
		for(const string& filename : filenames) {
			cout << "  " << setw(lineWidth) << left << filename << ": ";
			string result;
			cout << checker(path_stu + "/" + filename, result, table) << endl;
			updatePairVector(res, result);
			updatePairVector(singleRes, result);
		}
		int total = 0, passed = 0;
		for(const Pair& p : singleRes) {
			total += p.num;
			if(p.str == "正确")
				passed = p.num;
		}
		totalFiles += total;
		totalPassed += passed;
		if(passed == total)
			cout << "全部通过" << passed << "/" << total << "个文件，本次通过" << passed << "个" << endl;
		else 
			cout << "检查通过" << passed << "/" << total << "个文件，本次通过" << passed << "个" << endl;
		printSummary(singleRes, "学生详细信息", '-');
		cout << endl;
	}
	cout << endl;
	cout << "共完成" << names.size() << "个学生的检查，文件总数:" << totalFiles << "，通过总数:" << totalPassed << "，本次通过" << totalPassed << "个" << endl;
	printSummary(res, "整体详细信息", '=');
	cout << endl;
}

/** 
 * *单文件模式下的输出函数
 * @param path : 文件路径
 * @param filename : 检查作业文件名
 * @param table : 表信息
 * @param cno : 课程号
 * @param checker : 检查函数
 *  *返回一个用于输出的字符串，将检查的结果类型存到result中
 * 	参数：
 * 		@param path : 文件路径
 * 		@param result : 检查结果
 * 		@param table : 表信息
 * @param initPairVector : 初始化pair向量函数
 * 	*按照demo里的顺序初始化vector<Pair>
 * 	参数：
 * 		@param pairs : vector<Pair>类似于std::map的东西
*/
void singlefilePrinter(const string& path, const string& filename, const tableInfo& table, const _VS& cno, CHECKER_FUNC checker, INIT_FUNC initPairVector)
{
	const _VS &names = table.name;
	const _VS &allNames = table.allName;
	const _VS &stu_no = table.stu_no;
	cout << "课号 : ";
	for(int i = 0; i < int(cno.size()); i++)
		cout << cno[i] << (i == int(cno.size()) - 1 ? "" : ",");
	cout << " 学生数量 : " << names.size() << " 源文件名 : " << filename << endl;

	int maxNameLen = 0;
	for(const string& name : allNames)
		maxNameLen = max(maxNameLen, int(name.size()));
	++maxNameLen;
	vector<Pair> res;
	initPairVector(res);
	int totalFiles = 0, totalPassed = 0;

	for(int i = 0; i < int(names.size()); i++) {
		cout << setw(3) << left << i + 1 << ": " << stu_no[i] << "/" << setw(maxNameLen) << left << replaceChineseDot(names[i]) << ": ";
		string result;
		string path_stu = path + "/" + cno[0] + "-" + stu_no[i] + "/" + filename;
		if(cno.size() == 2)
			path_stu = path + "/" + findRowByStuNo(stu_no[i], table)[ROWPLACE_CNO] + "-" + stu_no[i] + "/" + filename;
		cout << checker(path_stu, result, table) << endl;
		updatePairVector(res, result);
		totalFiles++;
		if(result == "正确")
			totalPassed++;
	}

	cout << endl;
	if(totalPassed == names.size())
		cout << "全部通过" << totalPassed << "/" << names.size() << "个学生，本次通过" << totalPassed << "个" << endl;
	else
		cout << "检查通过" << totalPassed << "/" << names.size() << "个学生，本次通过" << totalPassed << "个" << endl;
	printSummary(res, "详细信息", '=');
	cout << endl;
}

static bool checkIDExist(const string& id, const tableInfo& table) {
	return findRowByStuNo(id, table) != _VS();
}

static bool checkNameRight(const string& id, const string& name, const tableInfo& table) {
	return findRowByStuNo(id, table)[ROWPLACE_STU_NAME] == name;
}

// stu_no是否在id的列表中
static bool checkInList(const string& id, const string& stu_no, const vector<_VS>& items, const tableInfo& table) {
	for(int i = 0; i < int(table.stu_no.size()); i++) {
		if(table.stu_no[i] == id) {
			for(int j = 0; j < int(items[i].size()); j += 2) {
				if(items[i][j] == stu_no)
					return 1;
			}
		}
	}
	return 0;
}

// id的列表中学号为stu_no的姓名是否为name
static bool checkNameWrittenRight(const string& id, const string& stu_no, const string& name, const vector<_VS>& items, const tableInfo& table) {
	for(int i = 0; i < int(table.stu_no.size()); i++) {
		if(table.stu_no[i] == id) {
			for(int j = 0; j < int(items[i].size()); j += 2) {
				if(items[i][j] == stu_no)
					return items[i][j + 1] == name;
			}
		}
	}
	return 0;
}


/**
 * *交叉检查打印
 * @param path: 文件路径
 * @param filename: 文件名
 * @param table: 表信息
 * @param cno: 课程号 （多课程下是一个字符串，单课程下是多个字符串）
 * @param extracter: 提取合法项函数
 *  *提取一个学生的第二行合法项并作为_VS类型返回
 * 	参数：
 * 		@param item : 第二行所有的项
 * 		@param pos : 学生序号
 * 		@param table : 表信息
 * @param checker: 检查函数
 *  *返回一个用于输出的字符串，将检查的结果类型存到result中
 * 	参数：
 * 		@param path : 文件路径
 * 		@param result : 检查结果
 * 		@param table : 表信息
*/
void crossIdentifyPrinter(const string& path, const string& filename, const tableInfo& table, const _VS& cno, EXTRACT_FUNC extracter, CHECKER_FUNC checker)
{
	const _VS &names = table.name;
	const _VS &stu_no = table.stu_no;
	const _VS &cnos = table.cno;
	cout << "交叉检查结果：\n";
	vector<_VS> items; // items: 所有学生的所有项
	for(int i = 0; i < int(names.size()); i++) {
		string path_stu = path + "/" + cno[0] + "-" + stu_no[i] + "/" + filename;
		if(cno.size() == 2)
			path_stu = path + "/" + findRowByStuNo(stu_no[i], table)[ROWPLACE_CNO] + "-" + stu_no[i] + "/" + filename;
		string result;
		checker(path_stu, result, table);
		if(result != "正确") {
			items.push_back(_VS());
			continue;
		}
		_VS item = extracter(extractItems(trimComment(extractLine(path_stu, 2))), i, table); // 提取对应的文件的第二行中合法的项
		items.push_back(item);
	}
	for(int i = 0; i < int(names.size()); i++) {
		cout << setw(3) << left << i + 1 << ": " << findRowByStuNo(stu_no[i], table)[ROWPLACE_CNO] << "-" << stu_no[i] << "-" << names[i] << endl;
		for(int j = 0; j < int(items[i].size()); j += 2) {
			const string &id = items[i][j];
			const string &name = items[i][j + 1];
			cout << "\t" << id << " " << name;
			if(checkIDExist(id, table) == 0)
				cout << "\t对方学号不存在";
			else if(checkNameRight(id, name, table) == 0)
				cout << "\t对方姓名不正确";
			else if(checkInList(id, stu_no[i], items, table) == 0)
				cout << "\t抛弃了你";
			else if(checkNameWrittenRight(id, stu_no[i], names[i], items, table) == 0)
				cout << "\t没写对你名字";
			cout << endl;
		}
	}
	cout << endl;
}