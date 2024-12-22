#include "debug.h"
#include <iostream>
#include <string>
#include "hw_check.h"

using namespace std;

static bool checkCanDebug(INFO info) {
    if(info.debug) {
        return true;
    }
    return false;
}

void __debugInfo(INFO info) 
{
    if(checkCanDebug(info)) {
		cout << "课号: ";
		for(const string& c : info.cno)
			cout << c << " ";
		cout << endl;
		cout << "学号: " << info.stu << endl;
		cout << "文件: " << info.file << endl;
		cout << "章节: " << info.chapter << endl;
		cout << "周次: " << info.week << endl;
		cout << "配置文件: " << info.cfgfile << endl;
		cout << "调试模式: " << (info.debug ? "开启" : "关闭") << endl;
		cout << "显示选项: " << endl;
		cout << "  正常信息: " << (info.printNormal ? "显示" : "不显示") << endl;
		cout << "  未提交信息: " << (info.printUnsubmit ? "显示" : "不显示") << endl;
		cout << "  错误信息: " << (info.printError ? "显示" : "不显示") << endl;
		cout << "  汇总信息: " << (info.printSummary ? "显示" : "不显示") << endl;
		cout << "  严重错误信息: " << (info.printSevere ? "显示" : "不显示") << endl;
	}
}

void __debugMySQL(string dbserver, string dbuser, string dbpasswd, string dbname, INFO info)
{
	if(checkCanDebug(info)) {
		cout << "数据库服务器: " << dbserver << endl;
		cout << "数据库用户名: " << dbuser << endl; 
		cout << "数据库密码: " << dbpasswd << endl;
		cout << "数据库名称: " << dbname << endl;
	}
}

void __debugSQLQueryAll(MYSQL* mysql, INFO info) 
{
	if(checkCanDebug(info)) {
		cout << "查询所有作业信息" << endl;
		vector<vector<string>> res = sqlQuery(mysql, "select * from view_hwcheck_hwlist");
		for(vector<string>& row : res) {
			for(string& col : row) {
				cout << col << " ";
			}
			cout << endl;
		}
	}
}
