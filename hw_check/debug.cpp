#include "debug.h"
#include <iostream>
#include <string>
#include "hw_check.h"

using namespace std;

static bool checkCanDebug(INFO info) {
#if DEBUG_MODE
	return 1;
#else
    if(info.debug)
        return 1;
    return 0;
#endif
}

void __debugInfo(INFO info) 
{
    if(checkCanDebug(info)) {
		cerr << "课号: ";
		for(const string& c : info.cno)
			cerr << c << " ";
		cerr << endl;
		cerr << "学号: " << info.stu << endl;
		cerr << "文件: " << info.file << endl;
		cerr << "章节: " << info.chapter << endl;
		cerr << "周次: " << info.week << endl;
		cerr << "配置文件: " << info.cfgfile << endl;
		cerr << "调试模式: " << (info.debug ? "开启" : "关闭") << endl;
		cerr << "显示选项: " << endl;
		cerr << "  正常信息: " << (info.printNormal ? "显示" : "不显示") << endl;
		cerr << "  未提交信息: " << (info.printUnsubmit ? "显示" : "不显示") << endl;
		cerr << "  错误信息: " << (info.printError ? "显示" : "不显示") << endl;
		cerr << "  汇总信息: " << (info.printSummary ? "显示" : "不显示") << endl;
		cerr << "  严重错误信息: " << (info.printSevere ? "显示" : "不显示") << endl;
	}
}

void __debugMySQL(string dbserver, string dbuser, string dbpasswd, string dbname, INFO info)
{
	if(checkCanDebug(info)) {
		cerr << "数据库服务器: " << dbserver << endl;
		cerr << "数据库用户名: " << dbuser << endl; 
		cerr << "数据库密码: " << dbpasswd << endl;
		cerr << "数据库名称: " << dbname << endl;
	}
}

void __debugSQLQueryAll(MYSQL* mysql, INFO info) 
{
	if(checkCanDebug(info)) {
		int cnt = 0;
		cerr << "查询所有作业信息" << endl;
		vector<_VS> res = sqlQuery(mysql, "select * from view_hwcheck_hwlist");
		for(_VS& row : res) {
			for(string& col : row)
				cerr << col << " ";
			if(++cnt == 5) {
				cerr << "..." << endl;
				break;
			}
			cerr << endl;
		}
	}
}

void __debugPrint(INFO info, string str) {
	if(checkCanDebug(info)) {
		cerr << str << endl;
	}
}
