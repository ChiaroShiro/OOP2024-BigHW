#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>
#include "../include/class_aat.h"
#include "../include_mariadb_x86/mysql/mysql.h"      // mysql特有
#include "hw_check.h"
#include "debug.h"

using namespace std;

const string action_set[] = {"ERROR", "base", "firstline", "secondline", "help"};

int main(int argc, char* argv[])
{
	args_analyse_tools args[] = {
		args_analyse_tools("--help",     ST_EXTARGS_TYPE::boolean,            0, false),
		args_analyse_tools("--debug",    ST_EXTARGS_TYPE::boolean,            0, false),
		args_analyse_tools("--action",   ST_EXTARGS_TYPE::str_with_set_error, 1, 0, action_set),	//参数-1无意义，表示无默认，必须指定
		args_analyse_tools("--cno",      ST_EXTARGS_TYPE::str,                1, string("error")),
		args_analyse_tools("--stu",      ST_EXTARGS_TYPE::str,                1, string("error")),
		args_analyse_tools("--file",     ST_EXTARGS_TYPE::str,                1, string("error")),
		args_analyse_tools("--chapter",  ST_EXTARGS_TYPE::int_with_error,     1, -1, -1, 99), //参数-1表示不进行章节选择
		args_analyse_tools("--week",     ST_EXTARGS_TYPE::int_with_error,     1, -1, -1, 20), //参数-1表示不进行周次选择
		args_analyse_tools("--display",  ST_EXTARGS_TYPE::str,                1, string("11111")),
		args_analyse_tools("--cfgfile",  ST_EXTARGS_TYPE::str,                1, string("hw_check.conf")),
		args_analyse_tools()  //最后一个，用于结束
	};
	int cur_argc = 0;
	if ((cur_argc = args_analyse_process(argc, argv, args, 0)) < 0)
		return -1;
	
	// 解析命令行参数
	INFO info;
	int mode = argvChecker(args, argv, info);
	if(mode == 0)
		return 0;
	__debugInfo(info);

	// 连接数据库、数据处理
	string path;
	MYSQL* mysql;
	if(!dataMain(info, path, mysql)) {
		cout << "数据初始化失败" << endl;
		return -1;
	}
	__debugSQLQueryAll(mysql, info);

	// 开始处理
	if(mode == 1)
		baseMain(info, mysql, path);
	else if(mode == 2)
		firstlineMain(info, mysql, path);
	else if(mode == 3)
		secondlineMain(info, mysql, path);
	return 0;
}


