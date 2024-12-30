#include "debug.h"
#include "hw_check.h"
#include <iostream>
using namespace std;

static void initPairVector(vector<Pair>& pairs) 
{
	pairs.clear();
	pairs.push_back({"正确", 0});
	pairs.push_back({"未提交", 0});
	pairs.push_back({"源文件格式不正确(非GB编码)", 0});
	pairs.push_back({"次行不是注释", 0});
}

// 返回0代表正确，返回非0代表错误，低16位表示错误位置，高16位表示错误类型
static int checkHaveMine(const _VS &item, int pos, const tableInfo& table)
{
    string myid = table.stu_no[pos];
    for(int i = 0; i < int(item.size()); i += 2) {
        if(item[i] == myid) // 检查者中包含自己
            return (1 << 16) + i;
		if(i + 1 == int(item.size())) // 有单独项
			return (1 << 17) + i;
        if(item[i].size() != 7) // 学号长度不对
            return (1 << 18) + i;
    }
    return 0;
}

static _VS extracter(const _VS &item, int pos, const tableInfo& table)
{
	_VS ret;
	string myid = table.stu_no[pos];
    for(int i = 0; i < int(item.size()); i += 2) {
        const string &s = item[i];
        if(s == myid || i + 1 == int(item.size()) || s.size() != 7)
            break;
		ret.push_back(s);
		ret.push_back(item[i + 1]);
    }
    return ret;
}

static string secondLineChecker(const string& path, string& result, const tableInfo& table)
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
	}
	if(checkLineIsComment(path, 2) == -1) {
		result = "次行不是注释";
		return "次行不是注释";
	}
    _VS item = extractItems(trimComment(extractLine(path, 2)));
    // 从路径中提取学号 stuNo
	string filename = path.substr(0, path.find_last_of('/'));
	string stuNo = filename.substr(filename.find_last_of('-') + 1);

    int res = checkHaveMine(item, findRowIndexByStuNo(stuNo, table), table);
    int opt = res >> 16;
    int retpos = res & ((1 << 16) - 1);
	result = "正确";
    if(opt == 1)
        return "第[" + to_string(retpos + 1) + "]项写了自己，后续内容忽略";
	if(opt == 2)
		return "第[" + to_string(retpos / 2) + "]个学生后面的信息不全(只读到一项)，后续内容忽略";
    if(opt == 4)
		return "第" + to_string(retpos / 2 + 1) + "位同学的学号[" + item[retpos] + "]不是7位，后续内容忽略";
	return "正确";
}

void secondlineMain(const INFO& info, MYSQL* mysql, const string& path) 
{
	tableInfo table;
	table.origin_cno = info.origin_cno;
	// 获取查询的作业文件名
	vector <_VS> sqlResult = sqlQuery(mysql, generateSQLQueryFromInfo(info, "hw_filename"));
	if(sqlResult.empty()) {
		cout << "未找到相关数据" << endl;
		return;
	}
	_VS filenames;
	for(const _VS& row : sqlResult)
		filenames.push_back(row[0]);
	
	// 获取查询的学生信息
	_VS request;
	if(info.cno.size() == 1)
		request = {"stu_cno = " + info.cno[0]}; // 数据库检索条件
	else
		request = {"(stu_cno = " + info.cno[0] + " or stu_cno = " + info.cno[1] + ")"}; // 数据库检索条件
	if(info.stu != "all")
		request.push_back("stu_no = " + info.stu);
	sqlResult = sqlQuery(mysql, generateSQLQuery(0, "stu_no, stu_name, stu_cno", request));
	if(sqlResult.empty()) {
		if(info.printError)
			cout << "未找到相关数据" << endl;
		return;
	}
	for(const _VS& row : sqlResult) {
		table.stu_no.push_back(row[0]);
		table.name.push_back(row[1]);
        table.cno.push_back(row[2]);
	}

	// 获取所有满足stu_cno的学生列表
	if(info.cno.size() == 1)
		sqlResult = sqlQuery(mysql, generateSQLQuery(0, "stu_name", {"stu_cno = " + info.cno[0]}));
	else 
		sqlResult = sqlQuery(mysql, generateSQLQuery(0, "stu_name", {"stu_cno = " + info.cno[0], "stu_cno = " + info.cno[1]}, "or"));
	for(const _VS& row : sqlResult)
		table.allName.push_back(row[0]);

	singlefilePrinter(info, path, filenames[0], table, info.cno, secondLineChecker, initPairVector);
	crossIdentifyPrinter(info, path, filenames[0], table, info.cno, extracter, secondLineChecker);

}