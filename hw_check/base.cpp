#include "debug.h"
#include "hw_check.h"
#include <iostream>
#include <fstream>
using namespace std;

static void initPairVector(vector<Pair>& pairs) {
	pairs.clear();
	pairs.push_back({"正确", 0});
	pairs.push_back({"未提交", 0});
	pairs.push_back({"文件小于4字节", 0});
	pairs.push_back({"PDF文件格式不正确", 0}); 
	pairs.push_back({"源文件格式不正确(VS无法识别)", 0});
	pairs.push_back({"源文件格式不正确(非GB编码)", 0});
}

static string baseChecker(const string& path, string& result, const tableInfo& table)
{
	if(!checkFileExist(path)) {
		result = "未提交";
		return "未提交";
	}
	if(getFileSuffix(path) == "h" || getFileSuffix(path) == "cpp" || getFileSuffix(path) == "c" || getFileSuffix(path) == "hpp") {
		if(checkIsUTF8(path)) {
			result = "源文件格式不正确(非GB编码)";
			return "源文件格式不正确(非GB编码)";
		}
		if(checkIsMacFile(path)) {
			result = "源文件格式不正确(VS无法识别)";
			return "源文件格式不正确(VS无法识别)";
		}
	}
	if(getFileSuffix(path) == "pdf") {
		ifstream fin(path, ios::binary | ios::ate);
		if(fin.tellg() < 4) {
			result = "文件小于4字节";
			return "文件小于4字节";
		}
		// 检查PDF文件头
		fin.seekg(0);
		char header[8];
		fin.read(header, 8);
		if(header[0] != '%' || header[1] != 'P' || header[2] != 'D' || header[3] != 'F' || header[4] != '-' || header[5] != '1' || header[6] != '.') {
			result = "PDF文件格式不正确";
			return "PDF文件格式不正确";
		}
	}
	result = "正确";
	return "正确";
}

void baseMain(const INFO& info, MYSQL* mysql, const string& path) 
{
	tableInfo table;

	// 获取查询的作业文件名
	string sql = generateSQLQueryFromInfo(info, "hw_filename");
	__debugPrint(info, sql);
	vector <_VS> sqlResult = sqlQuery(mysql, sql);
	if(sqlResult.empty()) {
		cout << "未找到相关数据" << endl;
		return;
	}
	_VS filenames;
	for(const _VS& row : sqlResult)
		filenames.push_back(row[0]);
	
	// 获取查询的学生信息
	_VS request = {"stu_cno = " + info.cno[0]}; // 数据库检索条件
	if(info.stu != "all")
		request.push_back("stu_no = " + info.stu);
	sql = generateSQLQuery(0, "stu_no, stu_name", request);
	__debugPrint(info, sql);
	sqlResult = sqlQuery(mysql, sql);
	if(sqlResult.empty()) {
		cout << "未找到相关数据" << endl;
		return;
	}
	for(const _VS& row : sqlResult) {
		table.stu_no.push_back(row[0]);
		table.name.push_back(row[1]);
	}

	// 获取所有满足stu_cno的学生列表
	sql = generateSQLQuery(0, "stu_name", {"stu_cno = " + info.cno[0]});
	__debugPrint(info, sql);
	sqlResult = sqlQuery(mysql, sql);
	for(const _VS& row : sqlResult)
		table.allName.push_back(row[0]);

	if(info.file != "all")
		singlefilePrinter(path, filenames[0], table, info.cno, baseChecker, initPairVector);
	else
		multifilePrinter(path, filenames, table, info.cno[0], baseChecker, initPairVector);
}