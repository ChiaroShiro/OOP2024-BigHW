#include "debug.h"
#include "hw_check.h"
#include <iostream>
#include <fstream>
using namespace std;

static void initPairVector(vector<Pair>& pairs) {
	pairs.clear();
	pairs.push_back({"��ȷ", 0});
	pairs.push_back({"δ�ύ", 0});
	pairs.push_back({"�ļ�С��4�ֽ�", 0});
	pairs.push_back({"PDF�ļ���ʽ����ȷ", 0}); 
	pairs.push_back({"Դ�ļ���ʽ����ȷ(VS�޷�ʶ��)", 0});
	pairs.push_back({"Դ�ļ���ʽ����ȷ(��GB����)", 0});
}

static string baseChecker(const string& path, string& result, const tableInfo& table)
{
	if(!checkFileExist(path)) {
		result = "δ�ύ";
		return "δ�ύ";
	}
	if(getFileSuffix(path) == "h" || getFileSuffix(path) == "cpp" || getFileSuffix(path) == "c" || getFileSuffix(path) == "hpp") {
		if(checkIsUTF8(path)) {
			result = "Դ�ļ���ʽ����ȷ(��GB����)";
			return "Դ�ļ���ʽ����ȷ(��GB����)";
		}
		if(checkIsMacFile(path)) {
			result = "Դ�ļ���ʽ����ȷ(VS�޷�ʶ��)";
			return "Դ�ļ���ʽ����ȷ(VS�޷�ʶ��)";
		}
	}
	if(getFileSuffix(path) == "pdf") {
		ifstream fin(path, ios::binary | ios::ate);
		if(fin.tellg() < 4) {
			result = "�ļ�С��4�ֽ�";
			return "�ļ�С��4�ֽ�";
		}
		// ���PDF�ļ�ͷ
		fin.seekg(0);
		char header[8];
		fin.read(header, 8);
		if(header[0] != '%' || header[1] != 'P' || header[2] != 'D' || header[3] != 'F' || header[4] != '-' || header[5] != '1' || header[6] != '.') {
			result = "PDF�ļ���ʽ����ȷ";
			return "PDF�ļ���ʽ����ȷ";
		}
	}
	result = "��ȷ";
	return "��ȷ";
}

void baseMain(const INFO& info, MYSQL* mysql, const string& path) 
{
	tableInfo table;

	// ��ȡ��ѯ����ҵ�ļ���
	string sql = generateSQLQueryFromInfo(info, "hw_filename");
	__debugPrint(info, sql);
	vector <_VS> sqlResult = sqlQuery(mysql, sql);
	if(sqlResult.empty()) {
		cout << "δ�ҵ��������" << endl;
		return;
	}
	_VS filenames;
	for(const _VS& row : sqlResult)
		filenames.push_back(row[0]);
	
	// ��ȡ��ѯ��ѧ����Ϣ
	_VS request = {"stu_cno = " + info.cno[0]}; // ���ݿ��������
	if(info.stu != "all")
		request.push_back("stu_no = " + info.stu);
	sql = generateSQLQuery(0, "stu_no, stu_name", request);
	__debugPrint(info, sql);
	sqlResult = sqlQuery(mysql, sql);
	if(sqlResult.empty()) {
		cout << "δ�ҵ��������" << endl;
		return;
	}
	for(const _VS& row : sqlResult) {
		table.stu_no.push_back(row[0]);
		table.name.push_back(row[1]);
	}

	// ��ȡ��������stu_cno��ѧ���б�
	sql = generateSQLQuery(0, "stu_name", {"stu_cno = " + info.cno[0]});
	__debugPrint(info, sql);
	sqlResult = sqlQuery(mysql, sql);
	for(const _VS& row : sqlResult)
		table.allName.push_back(row[0]);

	if(info.file != "all")
		singlefilePrinter(path, filenames[0], table, info.cno, baseChecker, initPairVector);
	else
		multifilePrinter(path, filenames, table, info.cno[0], baseChecker, initPairVector);
}