#include "debug.h"
#include <iostream>
#include <string>
#include "hw_check.h"

using namespace std;

static bool checkCanDebug(INFO info) {
    if(info.debug) {
        return true;
    }
    return false;
}

void __debugInfo(INFO info) 
{
    if(checkCanDebug(info)) {
		cout << "�κ�: ";
		for(const string& c : info.cno)
			cout << c << " ";
		cout << endl;
		cout << "ѧ��: " << info.stu << endl;
		cout << "�ļ�: " << info.file << endl;
		cout << "�½�: " << info.chapter << endl;
		cout << "�ܴ�: " << info.week << endl;
		cout << "�����ļ�: " << info.cfgfile << endl;
		cout << "����ģʽ: " << (info.debug ? "����" : "�ر�") << endl;
		cout << "��ʾѡ��: " << endl;
		cout << "  ������Ϣ: " << (info.printNormal ? "��ʾ" : "����ʾ") << endl;
		cout << "  δ�ύ��Ϣ: " << (info.printUnsubmit ? "��ʾ" : "����ʾ") << endl;
		cout << "  ������Ϣ: " << (info.printError ? "��ʾ" : "����ʾ") << endl;
		cout << "  ������Ϣ: " << (info.printSummary ? "��ʾ" : "����ʾ") << endl;
		cout << "  ���ش�����Ϣ: " << (info.printSevere ? "��ʾ" : "����ʾ") << endl;
	}
}

void __debugMySQL(string dbserver, string dbuser, string dbpasswd, string dbname, INFO info)
{
	if(checkCanDebug(info)) {
		cout << "���ݿ������: " << dbserver << endl;
		cout << "���ݿ��û���: " << dbuser << endl; 
		cout << "���ݿ�����: " << dbpasswd << endl;
		cout << "���ݿ�����: " << dbname << endl;
	}
}

void __debugSQLQueryAll(MYSQL* mysql, INFO info) 
{
	if(checkCanDebug(info)) {
		cout << "��ѯ������ҵ��Ϣ" << endl;
		vector<vector<string>> res = sqlQuery(mysql, "select * from view_hwcheck_hwlist");
		for(vector<string>& row : res) {
			for(string& col : row) {
				cout << col << " ";
			}
			cout << endl;
		}
	}
}
