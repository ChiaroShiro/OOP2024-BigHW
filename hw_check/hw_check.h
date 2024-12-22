#pragma once
#include "../include/class_aat.h"
#include "../include_mariadb_x86/mysql/mysql.h"      // mysql特有
#include <vector>

using namespace std;

class INFO {
public:
	vector<string> cno;
	string stu;
	string file;
	int chapter;
	int week;
	string cfgfile;
	bool debug;
	bool printNormal, printUnsubmit, printError, printSummary, printSevere; // 正常信息/未提交信息/错误信息/汇总信息/严重错误信息
};

int argvChecker(args_analyse_tools* args, char* argv[], INFO& info);

bool dataMain(INFO info, string& path, MYSQL*& mysql);
vector <vector <string>> sqlQuery(MYSQL* mysql, string sql);

void baseMain(INFO info);
void firstlineMain(INFO info);
void secondlineMain(INFO info);
