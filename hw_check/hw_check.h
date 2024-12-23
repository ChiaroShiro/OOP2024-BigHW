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

/**
 * *��������в���
 * @param args: aat�����������в���
 * @param argv: ԭʼ�����в���
 * @param info: �洢���Ϻ����Ϣ
 * @return: ����ģʽ
*/
int argvChecker(args_analyse_tools* args, char* argv[], INFO& info);



/**
 * *���ݴ���
 * @param info: ��������Ϣ
 * @param path: �洢ѧ���ļ���·��
 * @param mysql: �洢���ݿ�����
 * @return: �����Ƿ�ɹ�
*/
bool dataMain(INFO info, string& path, MYSQL*& mysql);

/**
 * *ִ��sql���
 * @param mysql: ���ݿ�����
 * @param sql: ִ�е�sql���
 * @return: ���ز�ѯ���
*/
vector <_VS> sqlQuery(MYSQL* mysql, string sql);

/**
 * *����sql��ѯ���
 * @param stu_or_hw: ��ѯ�ĸ���0-ѧ����1-��ҵ
 * @param resultList: Ҫ��ѯ��Щ����
 * @param conditions: ��ѯ����
 * @param connect: �����߼������
 * @return: ����sql��ѯ���
*/
string generateSQLQuery(int stu_or_hw, string resultList = "*", _VS conditions = _VS(), string connect = "and");

/**
 * *��INFO���������Ӧ��sql��ѯ�ļ������
 * @param info: ��������Ϣ
 * @param resultList: Ҫ��ѯ��Щ����
 * @return: ����sql��ѯ���
*/
string generateSQLQueryFromInfo(INFO info, string resultList = "*");



/**
 * *����ļ��Ƿ����
 * @param path: �ļ�·��
 * @return: �����Ƿ����
*/
bool checkFileExist(string path);

/**
 * *��ȡ�ļ�����׺
 * @param path: �ļ�·��
 * @return: �����ļ���׺
*/
string getFileSuffix(string path);

/**
 * *����ļ��Ƿ�ΪUTF-8����
 * @param path: �ļ�·��
 * @return: �����Ƿ�ΪUTF-8����
*/
bool checkIsUTF8(string path);

/**
 * *����ļ��Ƿ�Ϊmac�ļ�
 * @param path: �ļ�·��
 * @return: �����Ƿ�Ϊmac�ļ�
*/
bool checkIsMacFile(string path);


/**
 * *���ļ��뵥�ļ�ģʽ�µ��������
 * @param path: �ļ�·��
 * @param filenames: �ļ����б�
 * @param names: ѧ�������б�
 * @param stu_no: ѧ��ѧ���б�
 * @param cno: �γ̺�
 * @param checker: ��麯��
 * @param initPairVector: ��ʼ��pair��������
*/
void multifilePrinter(string path, _VS filenames, _VS names, _VS stu_no, string cno, CHECKER_FUNC checker, INIT_FUNC initPairVector);
void singlefilePrinter(string path, string filename, _VS names, _VS stu_no, _VS cno, CHECKER_FUNC checker, INIT_FUNC initPairVector);


/**
 * *����ģʽ����һ��ģʽ���ڶ���ģʽ
 * @param info: ��������Ϣ
 * @param mysql: ���ݿ�����
 * @param path: �ļ�·��
*/
void baseMain(INFO info, MYSQL* mysql, string path);
void firstlineMain(INFO info, MYSQL* mysql, string path);
void secondlineMain(INFO info, MYSQL* mysql, string path);
