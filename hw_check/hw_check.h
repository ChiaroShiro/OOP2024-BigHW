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
	bool printNormal, printUnsubmit, printError, printSummary, printSevere; // ������Ϣ/δ�ύ��Ϣ/������Ϣ/������Ϣ/���ش�����Ϣ
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
	_VS name; // ѧ������
	_VS allName; // Ϊ����demo����ר�ŷŵ�����
	_VS stu_no; // ѧ��ѧ��
	_VS classc; // �༶ȫ��
	_VS classb; // �༶���
	_VS cno; // �κ�
};

typedef string (*CHECKER_FUNC)(const string&, string&, const tableInfo&);
typedef void (*INIT_FUNC)(vector<Pair>&);
typedef _VS (*EXTRACT_FUNC)(const _VS &, int, const tableInfo&);

// const string SQL_STU = "select * from view_hwcheck_stulist ";
// const string SQL_HW = "select * from view_hwcheck_hwlist ";
// const string SQL_STU_ALL = "select * from view_hwcheck_stulist_all;";
// const string SQL_HW_ALL = "select * from view_hwcheck_hwlist_all;";

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
bool dataMain(const INFO& info, string& path, MYSQL*& mysql);

/**
 * *ִ��sql���
 * @param mysql: ���ݿ�����
 * @param sql: ִ�е�sql���
 * @return: ���ز�ѯ���
*/
vector <_VS> sqlQuery(MYSQL* mysql, const string& sql);

/**
 * *����sql��ѯ���
 * @param stu_or_hw: ��ѯ�ĸ���0-ѧ����1-��ҵ
 * @param resultList: Ҫ��ѯ��Щ����
 * @param conditions: ��ѯ����
 * @param connect: �����߼������
 * @return: ����sql��ѯ���
*/
string generateSQLQuery(int stu_or_hw, const string& resultList = "*", const _VS& conditions = _VS(), const string& connect = "and");

/**
 * *��INFO���������Ӧ��sql��ѯ�ļ������
 * @param info: ��������Ϣ
 * @param resultList: Ҫ��ѯ��Щ����
 * @return: ����sql��ѯ���
*/
string generateSQLQueryFromInfo(const INFO& info, const string& resultList = "*");



/**
 * *����ļ��Ƿ����
 * @param path: �ļ�·��
 * @return: �����Ƿ����
*/
bool checkFileExist(const string& path);

/**
 * *��ȡ�ļ�����׺
 * @param path: �ļ�·��
 * @return: �����ļ���׺
*/
string getFileSuffix(const string& path);

/**
 * *����ļ��Ƿ�ΪUTF-8����
 * @param path: �ļ�·��
 * @return: �����Ƿ�ΪUTF-8����
*/
bool checkIsUTF8(const string& path);

/**
 * *����ļ��Ƿ�Ϊmac�ļ�
 * @param path: �ļ�·��
 * @return: �����Ƿ�Ϊmac�ļ�
*/
bool checkIsMacFile(const string& path);

/**
 * *��ȡ�ļ��ĵ�line��	
 * @param path: �ļ�·��
 * @param line: �к�
 * @return: ���ص�line��
*/
string extractLine(const string& path, int line);

/**
 * *����ļ��Ƿ�Ϊע����
 * @param path: �ļ�·��
 * @param line: �кţ�ȡֵֻ�� 1/2
 * @return: -1: ����ע����; -2: �Ǵ���Ķ���ע��; 1: �ǵ���ע����; 2: ����ȷ����ע��
*/	
int checkLineIsComment(const string& path, int line);

/**
 * *ȥ��һ���ַ���ǰ��ո��Լ�ע��
 * @param str: �����ַ���
 * @return: ���ش������ַ���
*/
string trimComment(const string& str);

/**
 * *ȥ���ַ���ǰ��ո���Ʊ��
 * @param str: �����ַ���
 * @return: ���ش������ַ���
*/
string trim(const string& str);

/**
 * *�ж��ַ������ո���Ʊ���ָ�������
 * @param str: �����ַ���
 * @return: ��������
*/
int countItems(const string& str);

/**
 * *���ַ����ÿո������������ȡ��
 * @param str: �ַ���
 * @return: �ָ���
 */
_VS extractItems(const string &s);

/**
 * *����ѧ����table�в��Ҷ�Ӧ�е���������
 * @param stuNo: ѧ��
 * @param table: �����Ϣ
 * @return: ��ѧ�Ŷ�Ӧ����������: stu_no, stu_name, classb, classc, cno
 */
_VS findRowByStuNo(const string& stuNo, const tableInfo& table);



/**
 * *���ļ��뵥�ļ�ģʽ�µ��������
 * @param path: �ļ�·��
 * @param filenames: �ļ����б�
 * @param table: ����Ϣ
 * @param cno: �γ̺� ����γ�����һ���ַ��������γ����Ƕ���ַ�����
 * @param checker: ��麯��
 * @param initPairVector: ��ʼ��pair��������
*/
void multifilePrinter(const string& path, const _VS& filenames, const tableInfo& table, const string& cno, CHECKER_FUNC checker, INIT_FUNC initPairVector);
void singlefilePrinter(const string& path, const string& filename, const tableInfo& table, const _VS& cno, CHECKER_FUNC checker, INIT_FUNC initPairVector);


/**
 * *����ģʽ����һ��ģʽ���ڶ���ģʽ
 * @param info: ��������Ϣ
 * @param mysql: ���ݿ�����
 * @param path: �ļ�·��
*/
void baseMain(const INFO& info, MYSQL* mysql, const string& path);
void firstlineMain(const INFO& info, MYSQL* mysql, const string& path);
void secondlineMain(const INFO& info, MYSQL* mysql, const string& path);
