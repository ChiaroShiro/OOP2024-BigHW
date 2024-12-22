#pragma once
#include "../include/class_aat.h"
#include "../include_mariadb_x86/mysql/mysql.h"      // mysql特有
#include <vector>

using namespace std;

// #define DEBUG_MODE 		1

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

struct Pair {
    string str;
    int num;
};

typedef string (*CHECKER_FUNC)(string, string&);
typedef void (*INIT_FUNC)(vector<Pair>&);
#define _VS vector <string>


const string SQL_STU = "select * from view_hwcheck_stulist ";
const string SQL_HW = "select * from view_hwcheck_hwlist ";
const string SQL_STU_ALL = "select * from view_hwcheck_stulist_all;";
const string SQL_HW_ALL = "select * from view_hwcheck_hwlist_all;";

// 检查命令行参数
int argvChecker(args_analyse_tools* args, char* argv[], INFO& info);



// 数据处理
bool dataMain(INFO info, string& path, MYSQL*& mysql);
// 执行sql语句
vector <_VS> sqlQuery(MYSQL* mysql, string sql);
// stu_or_hw: 0-学生，1-作业
// 生成sql查询语句
string generateSQLQuery(int stu_or_hw, string resultList = "*", _VS conditions = _VS(), string connect = "and");
// 从INFO中生成sql查询文件的语句
string generateSQLQueryFromInfo(INFO info, string resultList = "*");



// 检测文件是否存在
bool checkFileExist(string path);
// 获取文件名后缀
string getFileSuffix(string path);

// 检测文件是否为UTF-8编码
bool checkIsUTF8(string path);
// 检测文件是否为mac文件
bool checkIsMacFile(string path);


void multifilePrinter(string path, _VS filenames, _VS names, _VS stu_no, string cno, CHECKER_FUNC checker, INIT_FUNC initPairVector);



void baseMain(INFO info, MYSQL* mysql, string path);
void firstlineMain(INFO info, MYSQL* mysql, string path);
void secondlineMain(INFO info, MYSQL* mysql, string path);
