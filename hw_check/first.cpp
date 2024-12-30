#include "debug.h"
#include "hw_check.h"
#include <iostream>
#include <sstream>
using namespace std;

static void initPairVector(vector<Pair>& pairs) 
{
	pairs.clear();
	pairs.push_back({"正确", 0});
	pairs.push_back({"未提交", 0});
    pairs.push_back({"源文件格式不正确(VS无法识别)", 0});
	pairs.push_back({"源文件格式不正确(非GB编码)", 0});
	pairs.push_back({"首行多行注释格式不正确", 0});
	pairs.push_back({"首行不是注释行", 0});
	pairs.push_back({"首行不是三项", 0});
	pairs.push_back({"首行检查出错", 0});
}

// 检查首行三项是否匹配
static bool checkNameClassStuID(const string& stuNo, const string& str, const tableInfo& table, string& errorMsg)
{
	stringstream ss(str);
	string item;
	_VS items;
	for(int i = 0; i < 3; i++) {
		ss >> item;
		items.push_back(item);
	}
	bool hasName = 0, hasClass = 0, hasStuID = 0;
	string errors;
	// 根据学号查找对应的学生信息
	_VS studentInfo = findRowByStuNo(stuNo, table);
	if(studentInfo.empty())
		return 0;
		
	// 检查三项是否匹配
	for(const string& item : items) {
		if(item == studentInfo[ROWPLACE_STU_NO]) // 检查学号
			hasStuID = 1;
		else if(item == studentInfo[ROWPLACE_STU_NAME]) // 检查姓名 
			hasName = 1;
		else if(item == studentInfo[ROWPLACE_CLASSC]) // 检查班级全称
			hasClass = 1;
		else {
			string shortName = studentInfo[ROWPLACE_CLASSB];
			size_t pos = 0;
			while((pos = shortName.find('|')) != string::npos) { // 检查班级简称
				if(item == shortName.substr(0, pos)) {
					hasClass = 1;
					break;
				}
				shortName = shortName.substr(pos + 1);
			}
			if(item == shortName)
				hasClass = 1;
		}
	}
	errors.clear();
	if(!hasStuID) 
		errors += "学号不匹配";
	if(!hasName) 
		errors += "姓名不匹配";
	if(!hasClass) 
		errors += "班级不匹配";
	if(!errors.empty()) {
		errorMsg = "首行" + errors;
		return 0;
	}
	return 1;
}

static string firstlineChecker(const string& path, string &result, const tableInfo& table) 
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
	string line = extractLine(path, 1);
	int commentType = checkLineIsComment(path, 1);
	if(commentType == -1) {
		result = "首行不是注释行";
		return "首行不是注释行 [" + line + "]";
	}
	if(commentType == -2) {
		result = "首行多行注释格式不正确";
		return "首行不是符合要求的/* */格式 [" + line + "]";
	}
	string newline = trimComment(line);
	int count = countItems(newline);
	if(count != 3) {
		result = "首行不是三项";
		return "首行不是三项 [" + line + "]";
	}
	// 从路径中提取学号 stuNo
	string filename = path.substr(0, path.find_last_of('/'));
	string stuNo = filename.substr(filename.find_last_of('-') + 1);

	string errorMsg;
	if(!checkNameClassStuID(stuNo, newline, table, errorMsg)) {
		result = "首行检查出错";
		return errorMsg + " [" + line + "]";
	}
	result = "正确";
	return "正确";
}

void firstlineMain(const INFO& info, MYSQL* mysql, const string& path) 
{
	tableInfo table;
	_VS filenames;
	table.origin_cno = info.origin_cno;
	vector <_VS> sqlResult = sqlQuery(mysql, generateSQLQueryFromInfo(info, "hw_filename")); // 作业文件表
	if(sqlResult.empty()) {
		cout << "未找到相关数据" << endl;
		return;
	}
	for(const _VS& row : sqlResult) {
		string suffix = getFileSuffix(row[0]);
		if(suffix == "c" || suffix == "cpp" || suffix == "hpp" || suffix == "h")
			filenames.push_back(row[0]); // 文件名
	}
	// 获取查询的学生信息
	_VS request = {"stu_cno = " + info.cno[0]}; // 数据库检索条件
	if(info.stu != "all")
		request.push_back("stu_no = " + info.stu);
	sqlResult = sqlQuery(mysql, generateSQLQuery(0, "stu_no, stu_name, stu_class_sname, stu_class_fname", request)); // 学生表
	if(sqlResult.empty()) {
		if(info.printError)
			cout << "未找到相关数据" << endl;
		return;
	}
	for(const _VS& row : sqlResult) {
		table.stu_no.push_back(row[0]); // 学号
		table.name.push_back(row[1]); // 姓名
		table.classb.push_back(row[2]); // 班级简称
		table.classc.push_back(row[3]); // 班级全称
	}

	// 获取所有满足stu_cno的学生列表
	sqlResult = sqlQuery(mysql, generateSQLQuery(0, "stu_name", {"stu_cno = " + info.cno[0]}));
	for(const _VS& row : sqlResult)
		table.allName.push_back(row[0]);

	if(info.file != "all")
		singlefilePrinter(info, path, filenames[0], table, info.cno, firstlineChecker, initPairVector);
	else
		multifilePrinter(info, path, filenames, table, info.cno[0], firstlineChecker, initPairVector);
}
