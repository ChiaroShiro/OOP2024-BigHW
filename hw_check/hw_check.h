#pragma once
#include "../include/class_aat.h"
#include "../include_mariadb_x86/mysql/mysql.h"      // mysql����
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
	bool printNormal, printUnsubmit, printError, printSummary, printSevere; // ������Ϣ/δ�ύ��Ϣ/������Ϣ/������Ϣ/���ش�����Ϣ
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

// ��������в���
int argvChecker(args_analyse_tools* args, char* argv[], INFO& info);



// ���ݴ���
bool dataMain(INFO info, string& path, MYSQL*& mysql);
// ִ��sql���
vector <_VS> sqlQuery(MYSQL* mysql, string sql);
// stu_or_hw: 0-ѧ����1-��ҵ
// ����sql��ѯ���
string generateSQLQuery(int stu_or_hw, string resultList = "*", _VS conditions = _VS(), string connect = "and");
// ��INFO������sql��ѯ�ļ������
string generateSQLQueryFromInfo(INFO info, string resultList = "*");



// ����ļ��Ƿ����
bool checkFileExist(string path);
// ��ȡ�ļ�����׺
string getFileSuffix(string path);

// ����ļ��Ƿ�ΪUTF-8����
bool checkIsUTF8(string path);
// ����ļ��Ƿ�Ϊmac�ļ�
bool checkIsMacFile(string path);


void multifilePrinter(string path, _VS filenames, _VS names, _VS stu_no, string cno, CHECKER_FUNC checker, INIT_FUNC initPairVector);



void baseMain(INFO info, MYSQL* mysql, string path);
void firstlineMain(INFO info, MYSQL* mysql, string path);
void secondlineMain(INFO info, MYSQL* mysql, string path);
