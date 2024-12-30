#include "debug.h"
#include "hw_check.h"
#include <iostream>
#include <fstream>

using namespace std;

static void initPairVector(vector<Pair>& pairs) {
	pairs.clear();
	pairs.push_back({"��ȷ", 0});
	pairs.push_back({"δ�ύ", 0});
	pairs.push_back({"Դ�ļ���ʽ����ȷ(��GB����)", 0});
	pairs.push_back({"���в���ע��", 0});
}

static int checkHaveMine(const _VS &item, int pos, const tableInfo& table)
{
    string myid = table.stu_no[pos];
    for(int i = 0; i < int(item.size()); i += 2) {
        const string &s = item[i];
		__debugPrint(__DEBUG_INFO, "s: " + s);
		__debugPrint(__DEBUG_INFO, "s.size(): " + to_string(s.size()));
        if(s == myid)
            return (1 << 8) + i;
		if(i + 1 == int(item.size()))
			return (1 << 9) + i;
        if(s.size() != 7)
            return (1 << 10) + i;
    }
    return 0;
}

static string secondLineChecker(const string& path, string& result, const tableInfo& table)
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
	}
	int commentType = checkLineIsComment(path, 2);
	if(commentType == -1) {
		result = "���в���ע��";
		return "���в���ע��";
	}
    _VS item = extractItems(trimComment(extractLine(path, 2)));
	__debugPrintContainer(__DEBUG_INFO, item, "item");
    // ��·������ȡѧ�� stuNo
	size_t lastSlash = path.find_last_of('/');
	string filename = path.substr(0, lastSlash);
	size_t dashPos = filename.find_last_of('-');
	string stuNo = filename.substr(dashPos + 1);
	__debugPrint(__DEBUG_INFO, "stuNo: " + stuNo);
    int studentIndex = 0;
	for(int i = 0; i < int(table.stu_no.size()); i++) {
		if(stuNo == table.stu_no[i]) {
			studentIndex = i;
			break;
		}
	}
    int res = checkHaveMine(item, studentIndex, table);
    int opt = res >> 8;
    int retpos = res & ((1 << 8) - 1);
	__debugPrint(__DEBUG_INFO, "opt: " + to_string(opt));
	__debugPrint(__DEBUG_INFO, "retpos: " + to_string(retpos));
	result = "��ȷ";
    if(opt == 1) {
        return "��[" + to_string(retpos + 1) + "]��д���Լ����������ݺ���";
    }
	if(opt == 2) {
		return "��[" + to_string(retpos / 2) + "]��ѧ���������Ϣ��ȫ(ֻ����һ��)���������ݺ���";
	}
    if(opt == 4) {
        return "��" + to_string(retpos / 2 + 1) + "λͬѧ��ѧ��[" + item[retpos] + "]����7λ���������ݺ���";
    }
	return "��ȷ";
}

void secondlineMain(const INFO& info, MYSQL* mysql, const string& path) 
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
	_VS request;
	if(info.cno.size() == 1) {
		request = {"stu_cno = " + info.cno[0]}; // ���ݿ��������
	}
	else {
		request = {"(stu_cno = " + info.cno[0] + " or stu_cno = " + info.cno[1] + ")"}; // ���ݿ��������
	}
	if(info.stu != "all")
		request.push_back("stu_no = " + info.stu);
	sql = generateSQLQuery(0, "stu_no, stu_name, stu_cno", request);
	__debugPrint(info, sql);
	sqlResult = sqlQuery(mysql, sql);
	if(sqlResult.empty()) {
		cout << "δ�ҵ��������" << endl;
		return;
	}
	for(const _VS& row : sqlResult) {
		table.stu_no.push_back(row[0]);
		table.name.push_back(row[1]);
        table.cno.push_back(row[2]);
	}

	// ��ȡ��������stu_cno��ѧ���б�
	if(info.cno.size() == 1) {
		sql = generateSQLQuery(0, "stu_name", {"stu_cno = " + info.cno[0]});
	}
	else {
		sql = generateSQLQuery(0, "stu_name", {"(stu_cno = " + info.cno[0] + " or stu_cno = " + info.cno[1] + ")"});
	}
	__debugPrint(info, sql);
	sqlResult = sqlQuery(mysql, sql);
	for(const _VS& row : sqlResult)
		table.allName.push_back(row[0]);

	singlefilePrinter(path, filenames[0], table, info.cno, secondLineChecker, initPairVector);


}