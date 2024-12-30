#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>
#include "../include/class_aat.h"
#include "../include_mariadb_x86/mysql/mysql.h"
#include "hw_check.h"
#include "debug.h"

using namespace std;

static void updatePairVector(vector<Pair>& pairs, const string& result) 
{
	bool found = 0;
	for(Pair& p : pairs) {
		if(p.str == result) {
			p.num++;
			found = 1;
			break;
		}
	}
	if(!found)
		pairs.push_back({result, 1});
}

static void printSummary(vector<Pair>& pairs, string title, char separator = '=') 
{
	int maxLen = 0;
	// ɾ������Ϊ0����
	vector<Pair> newPairs;
	for(Pair& p : pairs) {
		if(p.num > 0)
			newPairs.push_back(p);
	}
	pairs.clear();
	pairs.insert(pairs.begin(), newPairs.begin(), newPairs.end());

	for(Pair& p : pairs)
		maxLen = max(maxLen, int(p.str.length()));
	maxLen += 2;
	string sepLine(maxLen + 10, separator);

	cout << sepLine << endl;
	cout << title << endl;
	cout << sepLine << endl;

	for(Pair& p : pairs)
		cout << setw(maxLen) << right << p.str << " : " << p.num << endl;
	cout << sepLine << endl;
}

static string replaceChineseDot(string str) {
	string result;
	for(size_t i = 0; i < str.length(); i++) {
		// ����Ƿ�Ϊ���ĵ�� (0xA1 0xA4 in GB2312)
		if(i + 1 < str.length() && static_cast<unsigned char>(str[i]) == 0xA1 && static_cast<unsigned char>(str[i+1]) == 0xA4) {
			result += '.';
			i++;
		} 
		else
			result += str[i];
	}
	return result;
}

/** 
 * *���ļ�ģʽ�µ��������
 * @param path : �ļ�·��
 * @param filenames : �����ҵ�ļ����б�
 * @param table : ����Ϣ
 * @param cno : �γ̺ţ�ֻ��һ����
 * @param checker : ��麯��
 *  *����һ������������ַ����������Ľ�����ʹ浽result��
 * 	������
 * 		@param path : �ļ�·��
 * 		@param result : �����
 * 		@param table : ����Ϣ
 * @param initPairVector : ��ʼ��pair��������
 * 	*����demo���˳���ʼ��vector<Pair>
 * 	������
 * 		@param pairs : vector<Pair>������std::map�Ķ���
*/
void multifilePrinter(const string& path, const _VS& filenames, const tableInfo& table, const string& cno, CHECKER_FUNC checker, INIT_FUNC initPairVector)
{
	const _VS &names = table.name;
	const _VS &stu_no = table.stu_no;
	int lineWidth = 0;
	for (int i = 0; i < int(filenames.size()); i++)
		lineWidth = max(lineWidth, int(filenames[i].size()));
	++lineWidth;
	vector <Pair> res;
	initPairVector(res);
	int totalFiles = 0, totalPassed = 0;
	for (int i = 0; i < int(names.size()); i++) {
		cout << setw(3) << left << i + 1 << ": " << "ѧ��-" << stu_no[i] << " ����-" << replaceChineseDot(names[i]) << " �κ�-" << cno << " �ļ�����-" << filenames.size() << endl;
		vector <Pair> singleRes;
		initPairVector(singleRes);
		string path_stu = path + "/" + cno + '-' + stu_no[i];
		for(const string& filename : filenames) {
			cout << "  " << setw(lineWidth) << left << filename << ": ";
			string result;
			cout << checker(path_stu + "/" + filename, result, table) << endl;
			updatePairVector(res, result);
			updatePairVector(singleRes, result);
		}
		int total = 0, passed = 0;
		for(const Pair& p : singleRes) {
			total += p.num;
			if(p.str == "��ȷ")
				passed = p.num;
		}
		totalFiles += total;
		totalPassed += passed;
		if(passed == total)
			cout << "ȫ��ͨ��" << passed << "/" << total << "���ļ�������ͨ��" << passed << "��" << endl;
		else 
			cout << "���ͨ��" << passed << "/" << total << "���ļ�������ͨ��" << passed << "��" << endl;
		printSummary(singleRes, "ѧ����ϸ��Ϣ", '-');
		cout << endl;
	}
	cout << endl;
	cout << "�����" << names.size() << "��ѧ���ļ�飬�ļ�����:" << totalFiles << "��ͨ������:" << totalPassed << "������ͨ��" << totalPassed << "��" << endl;
	printSummary(res, "������ϸ��Ϣ", '=');
	cout << endl;
}

/** 
 * *���ļ�ģʽ�µ��������
 * @param path : �ļ�·��
 * @param filename : �����ҵ�ļ���
 * @param table : ����Ϣ
 * @param cno : �γ̺�
 * @param checker : ��麯��
 *  *����һ������������ַ����������Ľ�����ʹ浽result��
 * 	������
 * 		@param path : �ļ�·��
 * 		@param result : �����
 * 		@param table : ����Ϣ
 * @param initPairVector : ��ʼ��pair��������
 * 	*����demo���˳���ʼ��vector<Pair>
 * 	������
 * 		@param pairs : vector<Pair>������std::map�Ķ���
*/
void singlefilePrinter(const string& path, const string& filename, const tableInfo& table, const _VS& cno, CHECKER_FUNC checker, INIT_FUNC initPairVector)
{
	const _VS &names = table.name;
	const _VS &allNames = table.allName;
	const _VS &stu_no = table.stu_no;
	cout << "�κ� : ";
	for(int i = 0; i < int(cno.size()); i++)
		cout << cno[i] << (i == int(cno.size()) - 1 ? "" : ",");
	cout << " ѧ������ : " << names.size() << " Դ�ļ��� : " << filename << endl;

	int maxNameLen = 0;
	for(const string& name : allNames)
		maxNameLen = max(maxNameLen, int(name.size()));
	++maxNameLen;
	vector<Pair> res;
	initPairVector(res);
	int totalFiles = 0, totalPassed = 0;

	for(int i = 0; i < int(names.size()); i++) {
		cout << setw(3) << left << i + 1 << ": " << stu_no[i] << "/" << setw(maxNameLen) << left << replaceChineseDot(names[i]) << ": ";
		string result;
		string path_stu = path + "/" + cno[0] + "-" + stu_no[i] + "/" + filename;
		if(cno.size() == 2)
			path_stu = path + "/" + findRowByStuNo(stu_no[i], table)[ROWPLACE_CNO] + "-" + stu_no[i] + "/" + filename;
		cout << checker(path_stu, result, table) << endl;
		updatePairVector(res, result);
		totalFiles++;
		if(result == "��ȷ")
			totalPassed++;
	}

	cout << endl;
	if(totalPassed == names.size())
		cout << "ȫ��ͨ��" << totalPassed << "/" << names.size() << "��ѧ��������ͨ��" << totalPassed << "��" << endl;
	else
		cout << "���ͨ��" << totalPassed << "/" << names.size() << "��ѧ��������ͨ��" << totalPassed << "��" << endl;
	printSummary(res, "��ϸ��Ϣ", '=');
	cout << endl;
}

static bool checkIDExist(const string& id, const tableInfo& table) {
	return findRowByStuNo(id, table) != _VS();
}

static bool checkNameRight(const string& id, const string& name, const tableInfo& table) {
	return findRowByStuNo(id, table)[ROWPLACE_STU_NAME] == name;
}

// stu_no�Ƿ���id���б���
static bool checkInList(const string& id, const string& stu_no, const vector<_VS>& items, const tableInfo& table) {
	for(int i = 0; i < int(table.stu_no.size()); i++) {
		if(table.stu_no[i] == id) {
			for(int j = 0; j < int(items[i].size()); j += 2) {
				if(items[i][j] == stu_no)
					return 1;
			}
		}
	}
	return 0;
}

// id���б���ѧ��Ϊstu_no�������Ƿ�Ϊname
static bool checkNameWrittenRight(const string& id, const string& stu_no, const string& name, const vector<_VS>& items, const tableInfo& table) {
	for(int i = 0; i < int(table.stu_no.size()); i++) {
		if(table.stu_no[i] == id) {
			for(int j = 0; j < int(items[i].size()); j += 2) {
				if(items[i][j] == stu_no)
					return items[i][j + 1] == name;
			}
		}
	}
	return 0;
}


/**
 * *�������ӡ
 * @param path: �ļ�·��
 * @param filename: �ļ���
 * @param table: ����Ϣ
 * @param cno: �γ̺� ����γ�����һ���ַ��������γ����Ƕ���ַ�����
 * @param extracter: ��ȡ�Ϸ����
 *  *��ȡһ��ѧ���ĵڶ��кϷ����Ϊ_VS���ͷ���
 * 	������
 * 		@param item : �ڶ������е���
 * 		@param pos : ѧ�����
 * 		@param table : ����Ϣ
 * @param checker: ��麯��
 *  *����һ������������ַ����������Ľ�����ʹ浽result��
 * 	������
 * 		@param path : �ļ�·��
 * 		@param result : �����
 * 		@param table : ����Ϣ
*/
void crossIdentifyPrinter(const string& path, const string& filename, const tableInfo& table, const _VS& cno, EXTRACT_FUNC extracter, CHECKER_FUNC checker)
{
	const _VS &names = table.name;
	const _VS &stu_no = table.stu_no;
	const _VS &cnos = table.cno;
	cout << "����������\n";
	vector<_VS> items; // items: ����ѧ����������
	for(int i = 0; i < int(names.size()); i++) {
		string path_stu = path + "/" + cno[0] + "-" + stu_no[i] + "/" + filename;
		if(cno.size() == 2)
			path_stu = path + "/" + findRowByStuNo(stu_no[i], table)[ROWPLACE_CNO] + "-" + stu_no[i] + "/" + filename;
		string result;
		checker(path_stu, result, table);
		if(result != "��ȷ") {
			items.push_back(_VS());
			continue;
		}
		_VS item = extracter(extractItems(trimComment(extractLine(path_stu, 2))), i, table); // ��ȡ��Ӧ���ļ��ĵڶ����кϷ�����
		items.push_back(item);
	}
	for(int i = 0; i < int(names.size()); i++) {
		cout << setw(3) << left << i + 1 << ": " << findRowByStuNo(stu_no[i], table)[ROWPLACE_CNO] << "-" << stu_no[i] << "-" << names[i] << endl;
		for(int j = 0; j < int(items[i].size()); j += 2) {
			const string &id = items[i][j];
			const string &name = items[i][j + 1];
			cout << "\t" << id << " " << name;
			if(checkIDExist(id, table) == 0)
				cout << "\t�Է�ѧ�Ų�����";
			else if(checkNameRight(id, name, table) == 0)
				cout << "\t�Է���������ȷ";
			else if(checkInList(id, stu_no[i], items, table) == 0)
				cout << "\t��������";
			else if(checkNameWrittenRight(id, stu_no[i], names[i], items, table) == 0)
				cout << "\tûд��������";
			cout << endl;
		}
	}
	cout << endl;
}