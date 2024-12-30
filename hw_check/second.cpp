#include "debug.h"
#include "hw_check.h"
#include <iostream>
#include <fstream>

using namespace std;

static void initPairVector(vector<Pair>& pairs) {
	pairs.clear();
	pairs.push_back({"正确", 0});
	pairs.push_back({"未提交", 0});
	pairs.push_back({"次行不是注释", 0});
}

static int checkHaveMine(const _VS &item, int pos, const tableInfo& table)
{
    string myid = table.stu_no[pos];
    for(int i = 0; i < int(item.size()); i += 2) {
        const string &s = item[i];
        if(s == myid)
            return (1 << 8) + i;
        if(s.size() != 7)
            return (2 << 8) + i;
    }
    return 0;
}

static string baseChecker(const string& path, string& result, const tableInfo& table)
{
	if(!checkFileExist(path)) {
		result = "未提交";
		return "未提交";
	}
	int commentType = checkLineIsComment(path, 2);
	if(commentType == -1) {
		result = "次行不是注释";
		return "次行不是注释";
	}
    _VS item = extractItems(trimComment(extractLine(path, 2)));
    // 从路径中提取学号 stuNo
	size_t lastSlash = path.find_last_of('/');
	string filename = path.substr(0, lastSlash);
	size_t dashPos = filename.find_last_of('-');
	string stuNo = filename.substr(dashPos + 1);
    int studentIndex = 0;
	for(int i = 0; i < int(table.stu_no.size()); i++) {
		if(stuNo == table.stu_no[i]) {
			studentIndex = i;
			break;
		}
	}
    int res = checkHaveMine(item, studentIndex, table);
    int opt = res >> 8;
    int retpos = res & ((1 << 8) - 1);
    if(opt == 1) {
        result = "第[" + to_string(retpos) + "]项写了自己，后续内容忽略";
    }
    if(opt == 2) {
        result = "第" + to_string(retpos / 2) + "位同学的学号[" + item[retpos] + "]不是7位，后续内容忽略";
    }
	result = "正确";
	return "正确";
}

void secondlineMain(const INFO& info, MYSQL* mysql, const string& path) 
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
	sql = generateSQLQuery(0, "stu_no, stu_name, stu_cno", request);
	__debugPrint(info, sql);
	sqlResult = sqlQuery(mysql, sql);
	if(sqlResult.empty()) {
		cout << "未找到相关数据" << endl;
		return;
	}
	for(const _VS& row : sqlResult) {
		table.stu_no.push_back(row[0]);
		table.name.push_back(row[1]);
        table.cno.push_back(row[2]);
	}

	// 获取所有满足stu_cno的学生列表
	sql = generateSQLQuery(0, "stu_name", {"stu_cno = " + info.cno[0]});
	__debugPrint(info, sql);
	sqlResult = sqlQuery(mysql, sql);
	for(const _VS& row : sqlResult)
		table.allName.push_back(row[0]);

	singlefilePrinter(path, filenames[0], table, info.cno, baseChecker, initPairVector);


}