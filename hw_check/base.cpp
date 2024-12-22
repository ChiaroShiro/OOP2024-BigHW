#include "debug.h"
#include "hw_check.h"
#include <iostream>
#include <fstream>

using namespace std;


void initPairVector(vector<Pair>& pairs) {
    pairs.clear();
    pairs.push_back({"��ȷ", 0});
    pairs.push_back({"δ�ύ", 0});
    pairs.push_back({"�ļ�С��4�ֽ�", 0});
    pairs.push_back({"PDF�ļ���ʽ����ȷ", 0}); 
    pairs.push_back({"Դ�ļ���ʽ����ȷ(VS�޷�ʶ��)", 0});
    pairs.push_back({"Դ�ļ���ʽ����ȷ(��GB����)", 0});
}

string baseChecker(string path, string& result)
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

void baseMain(INFO info, MYSQL* mysql, string path) 
{
	string sql = generateSQLQueryFromInfo(info, "hw_filename");
	__debugPrint(info, sql);
	vector <_VS> sqlResult = sqlQuery(mysql, sql);
	_VS filenames, names, stu_no;
	for(_VS row : sqlResult) {
		filenames.push_back(row[0]);
	}
	sql = generateSQLQuery(0, "stu_no, stu_name", _VS {"stu_cno = " + info.cno[0]});
	sqlResult = sqlQuery(mysql, sql);
	for(_VS row : sqlResult) {
		stu_no.push_back(row[0]);
		names.push_back(row[1]);
	}

	multifilePrinter(path, filenames, names, stu_no, info.cno[0], baseChecker, initPairVector);
}