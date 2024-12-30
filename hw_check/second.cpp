#include "debug.h"
#include "hw_check.h"
#include <iostream>
using namespace std;

static void initPairVector(vector<Pair>& pairs) 
{
	pairs.clear();
	pairs.push_back({"��ȷ", 0});
	pairs.push_back({"δ�ύ", 0});
	pairs.push_back({"Դ�ļ���ʽ����ȷ(��GB����)", 0});
	pairs.push_back({"���в���ע��", 0});
}

// ����0������ȷ�����ط�0������󣬵�16λ��ʾ����λ�ã���16λ��ʾ��������
static int checkHaveMine(const _VS &item, int pos, const tableInfo& table)
{
    string myid = table.stu_no[pos];
    for(int i = 0; i < int(item.size()); i += 2) {
        if(item[i] == myid) // ������а����Լ�
            return (1 << 16) + i;
		if(i + 1 == int(item.size())) // �е�����
			return (1 << 17) + i;
        if(item[i].size() != 7) // ѧ�ų��Ȳ���
            return (1 << 18) + i;
    }
    return 0;
}

static _VS extracter(const _VS &item, int pos, const tableInfo& table)
{
	_VS ret;
	string myid = table.stu_no[pos];
    for(int i = 0; i < int(item.size()); i += 2) {
        const string &s = item[i];
        if(s == myid || i + 1 == int(item.size()) || s.size() != 7)
            break;
		ret.push_back(s);
		ret.push_back(item[i + 1]);
    }
    return ret;
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
	if(checkLineIsComment(path, 2) == -1) {
		result = "���в���ע��";
		return "���в���ע��";
	}
    _VS item = extractItems(trimComment(extractLine(path, 2)));
    // ��·������ȡѧ�� stuNo
	string filename = path.substr(0, path.find_last_of('/'));
	string stuNo = filename.substr(filename.find_last_of('-') + 1);

    int res = checkHaveMine(item, findRowIndexByStuNo(stuNo, table), table);
    int opt = res >> 16;
    int retpos = res & ((1 << 16) - 1);
	result = "��ȷ";
    if(opt == 1)
        return "��[" + to_string(retpos + 1) + "]��д���Լ����������ݺ���";
	if(opt == 2)
		return "��[" + to_string(retpos / 2) + "]��ѧ���������Ϣ��ȫ(ֻ����һ��)���������ݺ���";
    if(opt == 4)
		return "��" + to_string(retpos / 2 + 1) + "λͬѧ��ѧ��[" + item[retpos] + "]����7λ���������ݺ���";
	return "��ȷ";
}

void secondlineMain(const INFO& info, MYSQL* mysql, const string& path) 
{
	tableInfo table;
	table.origin_cno = info.origin_cno;
	// ��ȡ��ѯ����ҵ�ļ���
	vector <_VS> sqlResult = sqlQuery(mysql, generateSQLQueryFromInfo(info, "hw_filename"));
	if(sqlResult.empty()) {
		cout << "δ�ҵ��������" << endl;
		return;
	}
	_VS filenames;
	for(const _VS& row : sqlResult)
		filenames.push_back(row[0]);
	
	// ��ȡ��ѯ��ѧ����Ϣ
	_VS request;
	if(info.cno.size() == 1)
		request = {"stu_cno = " + info.cno[0]}; // ���ݿ��������
	else
		request = {"(stu_cno = " + info.cno[0] + " or stu_cno = " + info.cno[1] + ")"}; // ���ݿ��������
	if(info.stu != "all")
		request.push_back("stu_no = " + info.stu);
	sqlResult = sqlQuery(mysql, generateSQLQuery(0, "stu_no, stu_name, stu_cno", request));
	if(sqlResult.empty()) {
		if(info.printError)
			cout << "δ�ҵ��������" << endl;
		return;
	}
	for(const _VS& row : sqlResult) {
		table.stu_no.push_back(row[0]);
		table.name.push_back(row[1]);
        table.cno.push_back(row[2]);
	}

	// ��ȡ��������stu_cno��ѧ���б�
	if(info.cno.size() == 1)
		sqlResult = sqlQuery(mysql, generateSQLQuery(0, "stu_name", {"stu_cno = " + info.cno[0]}));
	else 
		sqlResult = sqlQuery(mysql, generateSQLQuery(0, "stu_name", {"stu_cno = " + info.cno[0], "stu_cno = " + info.cno[1]}, "or"));
	for(const _VS& row : sqlResult)
		table.allName.push_back(row[0]);

	singlefilePrinter(info, path, filenames[0], table, info.cno, secondLineChecker, initPairVector);
	crossIdentifyPrinter(info, path, filenames[0], table, info.cno, extracter, secondLineChecker);

}