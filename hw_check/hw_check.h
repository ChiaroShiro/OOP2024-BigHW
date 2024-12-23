#pragma once
#include "../include/class_aat.h"
#include "../include_mariadb_x86/mysql/mysql.h"
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

/**
 * *检查命令行参数
 * @param args: aat处理后的命令行参数
 * @param argv: 原始命令行参数
 * @param info: 存储整合后的信息
 * @return: 返回模式
*/
int argvChecker(args_analyse_tools* args, char* argv[], INFO& info);



/**
 * *数据处理
 * @param info: 命令行信息
 * @param path: 存储学生文件夹路径
 * @param mysql: 存储数据库连接
 * @return: 返回是否成功
*/
bool dataMain(INFO info, string& path, MYSQL*& mysql);

/**
 * *执行sql语句
 * @param mysql: 数据库连接
 * @param sql: 执行的sql语句
 * @return: 返回查询结果
*/
vector <_VS> sqlQuery(MYSQL* mysql, string sql);

/**
 * *生成sql查询语句
 * @param stu_or_hw: 查询哪个表：0-学生，1-作业
 * @param resultList: 要查询哪些数据
 * @param conditions: 查询条件
 * @param connect: 连接逻辑运算符
 * @return: 返回sql查询语句
*/
string generateSQLQuery(int stu_or_hw, string resultList = "*", _VS conditions = _VS(), string connect = "and");

/**
 * *从INFO中生成其对应的sql查询文件的语句
 * @param info: 命令行信息
 * @param resultList: 要查询哪些数据
 * @return: 返回sql查询语句
*/
string generateSQLQueryFromInfo(INFO info, string resultList = "*");



/**
 * *检测文件是否存在
 * @param path: 文件路径
 * @return: 返回是否存在
*/
bool checkFileExist(string path);

/**
 * *获取文件名后缀
 * @param path: 文件路径
 * @return: 返回文件后缀
*/
string getFileSuffix(string path);

/**
 * *检测文件是否为UTF-8编码
 * @param path: 文件路径
 * @return: 返回是否为UTF-8编码
*/
bool checkIsUTF8(string path);

/**
 * *检测文件是否为mac文件
 * @param path: 文件路径
 * @return: 返回是否为mac文件
*/
bool checkIsMacFile(string path);


/**
 * *多文件与单文件模式下的输出函数
 * @param path: 文件路径
 * @param filenames: 文件名列表
 * @param names: 学生姓名列表
 * @param stu_no: 学生学号列表
 * @param cno: 课程号
 * @param checker: 检查函数
 * @param initPairVector: 初始化pair向量函数
*/
void multifilePrinter(string path, _VS filenames, _VS names, _VS stu_no, string cno, CHECKER_FUNC checker, INIT_FUNC initPairVector);
void singlefilePrinter(string path, string filename, _VS names, _VS stu_no, _VS cno, CHECKER_FUNC checker, INIT_FUNC initPairVector);


/**
 * *基础模式、第一行模式、第二行模式
 * @param info: 命令行信息
 * @param mysql: 数据库连接
 * @param path: 文件路径
*/
void baseMain(INFO info, MYSQL* mysql, string path);
void firstlineMain(INFO info, MYSQL* mysql, string path);
void secondlineMain(INFO info, MYSQL* mysql, string path);
