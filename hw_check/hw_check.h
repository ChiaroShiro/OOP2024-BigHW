#pragma once
#include "../include/class_aat.h"
#include "../include_mariadb_x86/mysql/mysql.h"      // mysql����
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
	bool printNormal, printUnsubmit, printError, printSummary, printSevere; // ������Ϣ/δ�ύ��Ϣ/������Ϣ/������Ϣ/���ش�����Ϣ
};

int argvChecker(args_analyse_tools* args, char* argv[], INFO& info);

bool dataMain(INFO info, string& path, MYSQL*& mysql);
vector <vector <string>> sqlQuery(MYSQL* mysql, string sql);

void baseMain(INFO info);
void firstlineMain(INFO info);
void secondlineMain(INFO info);
