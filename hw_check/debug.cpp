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
		cerr << "�κ�: ";
		for(const string& c : info.cno)
			cerr << c << " ";
		cerr << endl;
		cerr << "ѧ��: " << info.stu << endl;
		cerr << "�ļ�: " << info.file << endl;
		cerr << "�½�: " << info.chapter << endl;
		cerr << "�ܴ�: " << info.week << endl;
		cerr << "�����ļ�: " << info.cfgfile << endl;
		cerr << "����ģʽ: " << (info.debug ? "����" : "�ر�") << endl;
		cerr << "��ʾѡ��: " << endl;
		cerr << "  ������Ϣ: " << (info.printNormal ? "��ʾ" : "����ʾ") << endl;
		cerr << "  δ�ύ��Ϣ: " << (info.printUnsubmit ? "��ʾ" : "����ʾ") << endl;
		cerr << "  ������Ϣ: " << (info.printError ? "��ʾ" : "����ʾ") << endl;
		cerr << "  ������Ϣ: " << (info.printSummary ? "��ʾ" : "����ʾ") << endl;
		cerr << "  ���ش�����Ϣ: " << (info.printSevere ? "��ʾ" : "����ʾ") << endl;
	}
}

void __debugMySQL(string dbserver, string dbuser, string dbpasswd, string dbname, INFO info)
{
	if(checkCanDebug(info)) {
		cerr << "���ݿ������: " << dbserver << endl;
		cerr << "���ݿ��û���: " << dbuser << endl; 
		cerr << "���ݿ�����: " << dbpasswd << endl;
		cerr << "���ݿ�����: " << dbname << endl;
	}
}

void __debugSQLQueryAll(MYSQL* mysql, INFO info) 
{
	if(checkCanDebug(info)) {
		int cnt = 0;
		cerr << "��ѯ������ҵ��Ϣ" << endl;
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
