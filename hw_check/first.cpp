#include "debug.h"
#include "hw_check.h"
#include <iostream>
#include <sstream>
using namespace std;

static void initPairVector(vector<Pair>& pairs) 
{
	pairs.clear();
	pairs.push_back({"��ȷ", 0});
	pairs.push_back({"δ�ύ", 0});
    pairs.push_back({"Դ�ļ���ʽ����ȷ(VS�޷�ʶ��)", 0});
	pairs.push_back({"Դ�ļ���ʽ����ȷ(��GB����)", 0});
	pairs.push_back({"���ж���ע�͸�ʽ����ȷ", 0});
	pairs.push_back({"���в���ע����", 0});
	pairs.push_back({"���в�������", 0});
	pairs.push_back({"���м�����", 0});
}

// ������������Ƿ�ƥ��
static bool checkNameClassStuID(const string& stuNo, const string& str, const tableInfo& table, string& errorMsg)
{
	stringstream ss(str);
	string item;
	_VS items;
	for(int i = 0; i < 3; i++) {
		ss >> item;
		items.push_back(item);
	}
	bool hasName = 0, hasClass = 0, hasStuID = 0;
	string errors;
	// ����ѧ�Ų��Ҷ�Ӧ��ѧ����Ϣ
	_VS studentInfo = findRowByStuNo(stuNo, table);
	if(studentInfo.empty())
		return 0;
		
	// ��������Ƿ�ƥ��
	for(const string& item : items) {
		if(item == studentInfo[ROWPLACE_STU_NO]) // ���ѧ��
			hasStuID = 1;
		else if(item == studentInfo[ROWPLACE_STU_NAME]) // ������� 
			hasName = 1;
		else if(item == studentInfo[ROWPLACE_CLASSC]) // ���༶ȫ��
			hasClass = 1;
		else {
			string shortName = studentInfo[ROWPLACE_CLASSB];
			size_t pos = 0;
			while((pos = shortName.find('|')) != string::npos) { // ���༶���
				if(item == shortName.substr(0, pos)) {
					hasClass = 1;
					break;
				}
				shortName = shortName.substr(pos + 1);
			}
			if(item == shortName)
				hasClass = 1;
		}
	}
	errors.clear();
	if(!hasStuID) 
		errors += "ѧ�Ų�ƥ��";
	if(!hasName) 
		errors += "������ƥ��";
	if(!hasClass) 
		errors += "�༶��ƥ��";
	if(!errors.empty()) {
		errorMsg = "����" + errors;
		return 0;
	}
	return 1;
}

static string firstlineChecker(const string& path, string &result, const tableInfo& table) 
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
	string line = extractLine(path, 1);
	int commentType = checkLineIsComment(path, 1);
	if(commentType == -1) {
		result = "���в���ע����";
		return "���в���ע���� [" + line + "]";
	}
	if(commentType == -2) {
		result = "���ж���ע�͸�ʽ����ȷ";
		return "���в��Ƿ���Ҫ���/* */��ʽ [" + line + "]";
	}
	string newline = trimComment(line);
	int count = countItems(newline);
	if(count != 3) {
		result = "���в�������";
		return "���в������� [" + line + "]";
	}
	// ��·������ȡѧ�� stuNo
	string filename = path.substr(0, path.find_last_of('/'));
	string stuNo = filename.substr(filename.find_last_of('-') + 1);

	string errorMsg;
	if(!checkNameClassStuID(stuNo, newline, table, errorMsg)) {
		result = "���м�����";
		return errorMsg + " [" + line + "]";
	}
	result = "��ȷ";
	return "��ȷ";
}

void firstlineMain(const INFO& info, MYSQL* mysql, const string& path) 
{
	tableInfo table;
	_VS filenames;
	table.origin_cno = info.origin_cno;
	vector <_VS> sqlResult = sqlQuery(mysql, generateSQLQueryFromInfo(info, "hw_filename")); // ��ҵ�ļ���
	if(sqlResult.empty()) {
		cout << "δ�ҵ��������" << endl;
		return;
	}
	for(const _VS& row : sqlResult) {
		string suffix = getFileSuffix(row[0]);
		if(suffix == "c" || suffix == "cpp" || suffix == "hpp" || suffix == "h")
			filenames.push_back(row[0]); // �ļ���
	}
	// ��ȡ��ѯ��ѧ����Ϣ
	_VS request = {"stu_cno = " + info.cno[0]}; // ���ݿ��������
	if(info.stu != "all")
		request.push_back("stu_no = " + info.stu);
	sqlResult = sqlQuery(mysql, generateSQLQuery(0, "stu_no, stu_name, stu_class_sname, stu_class_fname", request)); // ѧ����
	if(sqlResult.empty()) {
		if(info.printError)
			cout << "δ�ҵ��������" << endl;
		return;
	}
	for(const _VS& row : sqlResult) {
		table.stu_no.push_back(row[0]); // ѧ��
		table.name.push_back(row[1]); // ����
		table.classb.push_back(row[2]); // �༶���
		table.classc.push_back(row[3]); // �༶ȫ��
	}

	// ��ȡ��������stu_cno��ѧ���б�
	sqlResult = sqlQuery(mysql, generateSQLQuery(0, "stu_name", {"stu_cno = " + info.cno[0]}));
	for(const _VS& row : sqlResult)
		table.allName.push_back(row[0]);

	if(info.file != "all")
		singlefilePrinter(info, path, filenames[0], table, info.cno, firstlineChecker, initPairVector);
	else
		multifilePrinter(info, path, filenames, table, info.cno[0], firstlineChecker, initPairVector);
}
