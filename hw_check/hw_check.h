#pragma once

#include "../include/class_aat.h"
#include "../include_mariadb_x86/mysql/mysql.h"
#include <vector>

using namespace std;

#define DEBUG_MODE 		0

typedef vector <string> _VS;

class INFO {
public:
	_VS cno;
	string stu;
	string file;
	int chapter;
	int week;
	string cfgfile;
	bool debug;
	bool printNormal, printUnsubmit, printError, printSummary, printSevere; // 正常信息/未提交信息/错误信息/汇总信息/严重错误信息
	INFO() 
	{
		debug = false;
	}
	INFO(int __debug) 
	{
		cno = _VS();
		stu = "";
		file = "";
		chapter = -1;
		week = -1;
		cfgfile = "";
		printNormal = true;
		printUnsubmit = true;
		printError = true;
		printSummary = true;
		printSevere = true;
		debug = __debug;
	}
};

#if DEBUG_MODE
const INFO __DEBUG_INFO(1);
#else 
const INFO __DEBUG_INFO(0);
#endif

struct Pair {
    string str;
    int num;
};

class tableInfo {
public:
	_VS name; // 学生姓名
	_VS allName; // 为满足demo特性专门放的数据
	_VS stu_no; // 学生学号
	_VS classc; // 班级全称
	_VS classb; // 班级简称
	_VS cno; // 课号
};

typedef string (*CHECKER_FUNC)(const string&, string&, const tableInfo&);
typedef void (*INIT_FUNC)(vector<Pair>&);
typedef _VS (*EXTRACT_FUNC)(const _VS &, int, const tableInfo&);

// const string SQL_STU = "select * from view_hwcheck_stulist ";
// const string SQL_HW = "select * from view_hwcheck_hwlist ";
// const string SQL_STU_ALL = "select * from view_hwcheck_stulist_all;";
// const string SQL_HW_ALL = "select * from view_hwcheck_hwlist_all;";

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
bool dataMain(const INFO& info, string& path, MYSQL*& mysql);

/**
 * *执行sql语句
 * @param mysql: 数据库连接
 * @param sql: 执行的sql语句
 * @return: 返回查询结果
*/
vector <_VS> sqlQuery(MYSQL* mysql, const string& sql);

/**
 * *生成sql查询语句
 * @param stu_or_hw: 查询哪个表：0-学生，1-作业
 * @param resultList: 要查询哪些数据
 * @param conditions: 查询条件
 * @param connect: 连接逻辑运算符
 * @return: 返回sql查询语句
*/
string generateSQLQuery(int stu_or_hw, const string& resultList = "*", const _VS& conditions = _VS(), const string& connect = "and");

/**
 * *从INFO中生成其对应的sql查询文件的语句
 * @param info: 命令行信息
 * @param resultList: 要查询哪些数据
 * @return: 返回sql查询语句
*/
string generateSQLQueryFromInfo(const INFO& info, const string& resultList = "*");



/**
 * *检测文件是否存在
 * @param path: 文件路径
 * @return: 返回是否存在
*/
bool checkFileExist(const string& path);

/**
 * *获取文件名后缀
 * @param path: 文件路径
 * @return: 返回文件后缀
*/
string getFileSuffix(const string& path);

/**
 * *检测文件是否为UTF-8编码
 * @param path: 文件路径
 * @return: 返回是否为UTF-8编码
*/
bool checkIsUTF8(const string& path);

/**
 * *检测文件是否为mac文件
 * @param path: 文件路径
 * @return: 返回是否为mac文件
*/
bool checkIsMacFile(const string& path);

/**
 * *提取文件的第line行	
 * @param path: 文件路径
 * @param line: 行号
 * @return: 返回第line行
*/
string extractLine(const string& path, int line);

/**
 * *检测文件是否为注释行
 * @param path: 文件路径
 * @param line: 行号，取值只有 1/2
 * @return: -1: 不是注释行; -2: 是错误的多行注释; 1: 是单行注释行; 2: 是正确多行注释
*/	
int checkLineIsComment(const string& path, int line);

/**
 * *去掉一行字符串前后空格以及注释
 * @param str: 输入字符串
 * @return: 返回处理后的字符串
*/
string trimComment(const string& str);

/**
 * *去掉字符串前后空格和制表符
 * @param str: 输入字符串
 * @return: 返回处理后的字符串
*/
string trim(const string& str);

/**
 * *判断字符串被空格或制表符分隔的项数
 * @param str: 输入字符串
 * @return: 返回项数
*/
int countItems(const string& str);

/**
 * *将字符串用空格隔开的若干项取出
 * @param str: 字符串
 * @return: 分割结果
 */
_VS extractItems(const string &s);

/**
 * *根据学号在table中查找对应行的所有数据
 * @param stuNo: 学号
 * @param table: 表格信息
 * @return: 该学号对应的所有数据: stu_no, stu_name, classb, classc, cno
 */
_VS findRowByStuNo(const string& stuNo, const tableInfo& table);



/**
 * *多文件与单文件模式下的输出函数
 * @param path: 文件路径
 * @param filenames: 文件名列表
 * @param table: 表信息
 * @param cno: 课程号 （多课程下是一个字符串，单课程下是多个字符串）
 * @param checker: 检查函数
 * @param initPairVector: 初始化pair向量函数
*/
void multifilePrinter(const string& path, const _VS& filenames, const tableInfo& table, const string& cno, CHECKER_FUNC checker, INIT_FUNC initPairVector);
void singlefilePrinter(const string& path, const string& filename, const tableInfo& table, const _VS& cno, CHECKER_FUNC checker, INIT_FUNC initPairVector);


/**
 * *基础模式、第一行模式、第二行模式
 * @param info: 命令行信息
 * @param mysql: 数据库连接
 * @param path: 文件路径
*/
void baseMain(const INFO& info, MYSQL* mysql, const string& path);
void firstlineMain(const INFO& info, MYSQL* mysql, const string& path);
void secondlineMain(const INFO& info, MYSQL* mysql, const string& path);
