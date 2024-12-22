/* 2351871 ������ �ƿ� */ 

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include "../include/class_cft.h"
#include <cctype>
#include <cstring>
#include <sstream>
using namespace std;

// ��ȡ�ָ���
static char getSeparator(const enum BREAK_CTYPE _ctype)
{
	if (_ctype == BREAK_CTYPE::Equal)
		return '=';
	else if (_ctype == BREAK_CTYPE::Space)
		return ' ';
	else
		return '\0';
}

// ��string��ǰ��׺���ɼ��ַ�ȥ��
static void clearStringProSuf(string& str)
{
	str.erase(0, str.find_first_not_of(" \t\n\r\f\v"));
	str.erase(str.find_last_not_of(" \t\n\r\f\v") + 1);
}

typedef unsigned int u_int;

// ���IP��ַ��ʽ
static bool checkIsIPAddr(string s)
{
	int cnt = 0; // ��¼�������
	int pos[5] = {0}; // ��¼ÿ�����λ��
	for(u_int i = 0; i < s.size(); i++) {
		if(s[i] == '.') {
			pos[++cnt] = i; // ��¼���λ��
			if(cnt > 3) // ��������������3��������false
				return false;
		}
	}
	if(cnt != 3) // ����������������3������false
		return false;
	pos[0] = -1;
	pos[4] = s.size() + 1;
	string subs[4]; // �洢ÿ���ֶ�
	for(int i = 0; i < 4; i++) {
		subs[i] = s.substr(pos[i] + 1, pos[i + 1] - pos[i] - 1); // ��ȡ�ֶ�
		if(subs[i] == "") // ����ֶ�Ϊ�գ�����false
			return false;
		for(u_int j = 0; j < subs[i].size(); j++) {
			if(!isdigit(subs[i][j])) // ����ֶ����з������ַ�������false
				return false;
		}
		int num = atoi(subs[i].c_str()); // ���ֶ�ת��Ϊ����
		if(num < 0 || num > 255) // �����������0��255֮�䣬����false
			return false;
	}
	return true; // ������м�鶼ͨ��������true
}

// IP addr: string->uint
static u_int getIntIPAddr(const string s)
{
	if (!checkIsIPAddr(s)) {
		exit(EXIT_FAILURE);
	}
	int pos = 0, nxtpos = 0;
	u_int ret = 0;
	while ((nxtpos = s.find('.', pos)) != -1) {
		ret = (ret << 8) + atoi(s.substr(pos, nxtpos - pos).c_str());
		pos = nxtpos + 1;
	}
	nxtpos = s.size();
	ret = (ret << 8) + atoi(s.substr(pos, nxtpos - pos).c_str());
	return ret;
}

// ��ȡ�����ļ�
void config_file_tools::read_cfg_file(const string& _cfgname, const enum BREAK_CTYPE _ctype)
{
	cfgname = _cfgname;
	item_separate_character_type = _ctype;
	fstream fs;
	fs.open(cfgname, ios::in);
	if (!fs.is_open()) {
		is_read_succeeded_value = false;
		return;
	}
	is_read_succeeded_value = true;

	string line;
	vector<string> lines;
	char buffer[MAX_LINE];
	while (fs) { // ����ע������
		fs.getline(buffer, MAX_LINE);
		line = buffer;
		size_t pos = line.find('#');
		if (pos != string::npos)
			line = line.substr(0, pos);

		pos = line.find(';');
		if (pos != string::npos)
			line = line.substr(0, pos);

		pos = line.find("//");
		if (pos != string::npos)
			line = line.substr(0, pos);

		clearStringProSuf(line);
		if (!line.empty())
			lines.push_back(line);
	}

	fs.close();

	string currentGroup;
	bool noGroupInit = 1;

	for (const string& line : lines) {
		if (line.front() == '[' && line.back() == ']') { // ��������
			noGroupInit = 0;
			currentGroup = line;
			data.push_back(make_pair(currentGroup, vector<ITEM_TYPE>()));
			lineraw.push_back(make_pair(currentGroup, vector<string>()));
		} else {
			if (noGroupInit) { // ����û�����������
				currentGroup = SIMPLE_GNAME;
				noGroupInit = 0;
				data.push_back(make_pair(currentGroup, vector<ITEM_TYPE>()));
				lineraw.push_back(make_pair(currentGroup, vector<string>()));
			}
			lineraw.back().second.push_back(line);
			size_t separatorPos = line.find(getSeparator(item_separate_character_type));
			if (separatorPos != string::npos) { // ����������ֵ
				string itemName = line.substr(0, separatorPos);
				string itemValue = line.substr(separatorPos + 1);
				clearStringProSuf(itemName);
				clearStringProSuf(itemValue);
				data.back().second.push_back(make_pair(itemName, itemValue));
			}
			else // ����û��ֵ�����
				data.back().second.push_back(make_pair(line, ""));
		}
	}
}

// ���ı��ַ������������ִ�Сд���龰
static string transferNoChange(const string& str)
{
	return str;
}

// ���ַ���ȫ��תΪСд�����ڲ����ִ�Сд���龰
static string transferToLower(const string& str)
{
	string ret = str;
	for (char& ch : ret)
		ch = tolower(ch);
	return ret;
}

typedef string (*TRANS_TYPE)(const string&);

// ���ַ����ж�ȡֵ
template <typename T>
static int readFromString(const string& str, T& value)
{
	if (str.length() == 0)
		return 0;
    istringstream iss(str);
    if (!(iss >> value))
        return 0;
    return 1;
}

// ��ȡĬ��ֵ
template <typename T>
static T getDefaultValue()
{
	if (typeid(T) == typeid(char))
		return T(DEFAULT_CHAR_VALUE);
	else if (typeid(T) == typeid(int))
		return T(DEFAULT_INT_VALUE);
	else if (typeid(T) == typeid(double))
		return T(DEFAULT_DOUBLE_VALUE);
	else if (typeid(T) == typeid(unsigned int))
		return T(DEFAULT_IPADDR_VALUE);
	return T();
}

// ��ȡ�ַ�����Ĭ��ֵ�����string�ػ�
template <>
static string getDefaultValue()
{
	return DEFAULT_STRING_VALUE;
}

// ����Ĭ��ֵ��ͨ�ô����߼�
template <typename T>
static int changeValueAndReturn(T& value, const T& def_value)
{
	if (getDefaultValue <T> () == def_value)
		return 0;
	else {
		value = def_value;
		return 1;
	}
}

// ���char*���ػ�
static int changeValueAndReturn(char* const value, const char* def_value) 
{
    if (strcmp(DEFAULT_CSTRING_VALUE, def_value) == 0)
        return 0;
    else {
        strcpy(value, def_value);
        return 1;
    }
}

config_file_tools::config_file_tools(const char* const _cfgname, const enum BREAK_CTYPE _ctype)
{
	this->read_cfg_file(_cfgname, _ctype);
}

config_file_tools::config_file_tools(const string& _cfgname, const enum BREAK_CTYPE _ctype)
{
	this->read_cfg_file(_cfgname, _ctype);
}

config_file_tools::~config_file_tools()
{
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��ж϶������ļ��Ƿ�ɹ�
  ���������
  �� �� ֵ��true - �ɹ����Ѷ������е���/��
		   false - ʧ�ܣ��ļ�ĳ�г���/�ļ�ȫ����ע�����
  ˵    ����
***************************************************************************/
bool config_file_tools::is_read_succeeded() const
{
	return is_read_succeeded_value;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����������ļ��е�������
  ���������vector <string>& ret : vector ��ÿ��Ϊһ������
  �� �� ֵ������������������������ļ���������Ϊ1������Ϊ"��
  ˵    ����
***************************************************************************/
int config_file_tools::get_all_group(vector <string>& ret)
{
	ret.clear();
	for (const DATA_TYPE& group : data)
		ret.push_back(group.first);
	return ret.size();
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�����ָ�����������������ԭʼ����
  ���������const string& group_name������
		   vector <string>& ret��vector ��ÿ��Ϊһ�����ԭʼ����
		   const bool is_case_sensitive = false : �����Ƿ��Сд���У�true-��Сд���� / Ĭ��false-��Сд������
  �� �� ֵ�����������0��ʾ��
  ˵    ����
***************************************************************************/
int config_file_tools::get_all_item(const string& group_name, vector <string>& ret, const bool is_case_sensitive)
{
	TRANS_TYPE transfer = is_case_sensitive ? transferNoChange : transferToLower;
	string group_name_str = transfer(group_name);
	for (const LINERAW_TYPE& group : lineraw) {
		if (transfer(group.first) == group_name_str) {
			for (const string& item : group.second)
				ret.push_back(item);
		}
	}
	return ret.size();
}

int config_file_tools::get_all_item(const char* const group_name, vector <string>& ret, const bool is_case_sensitive)
{
	return get_all_item(string(group_name), ret, is_case_sensitive);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�ȡĳ���ԭʼ���ݣ�=��������ַ���string��ʽ��
  ���������const char* const group_name
		   const char* const item_name
		   string &ret
		   const bool group_is_case_sensitive = false : �����Ƿ��Сд���У�true-��Сд���� / Ĭ��false-��Сд������
		   const bool item_is_case_sensitive = false  : �����Ƿ��Сд���У�true-��Сд���� / Ĭ��false-��Сд������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int config_file_tools::item_get_raw(const char* const group_name, const char* const item_name, string& ret, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	TRANS_TYPE group_transfer = group_is_case_sensitive ? transferNoChange : transferToLower;
	TRANS_TYPE item_transfer = item_is_case_sensitive ? transferNoChange : transferToLower;
	string group_name_str = group_transfer(group_name);
	string item_name_str = item_transfer(item_name);

	for (const DATA_TYPE& group : data) {
		if (group_transfer(group.first) == group_name_str) {
			for (const ITEM_TYPE& item : group.second) {
				if (item_transfer(item.first) == item_name_str) {
					ret = item.second;
					return 1;
				}
			}
		}
	}
	return 0;
}

int config_file_tools::item_get_raw(const string& group_name, const string& item_name, string& ret, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	return this->item_get_raw(group_name.c_str(), item_name.c_str(), ret, group_is_case_sensitive, item_is_case_sensitive);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�ȡĳ������ݣ���������Ϊchar��
  ���������const char* const group_name               ������
		   const char* const item_name                ������
		   const bool group_is_case_sensitive = false : �����Ƿ��Сд���У�true-��Сд���� / Ĭ��false-��Сд������
		   const bool item_is_case_sensitive = false  : �����Ƿ��Сд���У�true-��Сд���� / Ĭ��false-��Сд������
  �� �� ֵ��1 - �������������
		   0 - ���������������
  ˵    ����
***************************************************************************/
int config_file_tools::item_get_null(const char* const group_name, const char* const item_name, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	TRANS_TYPE group_transfer = group_is_case_sensitive ? transferNoChange : transferToLower;
	TRANS_TYPE item_transfer = item_is_case_sensitive ? transferNoChange : transferToLower;
	string group_name_str = group_transfer(group_name);
	string item_name_str = item_transfer(item_name);

	for (const DATA_TYPE& group : data) {
		if (group_transfer(group.first) == group_name_str) {
			for (const ITEM_TYPE& item : group.second) {
				if (item_transfer(item.first) == item_name_str)
					return 1;
			}
		}
	}
	return 0;
}

int config_file_tools::item_get_null(const string& group_name, const string& item_name, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	return this->item_get_null(group_name.c_str(), item_name.c_str(), group_is_case_sensitive, item_is_case_sensitive);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�ȡĳ������ݣ���������Ϊchar��
  ���������const char* const group_name               ������
		   const char* const item_name                ������
		   char& value                                ��������char��ֵ������1ʱ���ţ�����0�򲻿��ţ�
		   const char* const choice_set = nullptr     ���Ϸ���char�ļ��ϣ����磺"YyNn"��ʾ�Ϸ�����ΪY/N�Ҳ��ִ�Сд���ò�����Ĭ��ֵnullptr����ʾȫ���ַ���������飩
		   const char def_value = DEFAULT_CHAR_VALUE  ��������/�����Ƿ�������µ�Ĭ��ֵ���ò�����Ĭ��ֵDEFAULT_CHAR_VALUE�����������
															��ֵ��   DEFAULT_CHAR_VALUE ʱ������0��ֵ�����ţ�
															��ֵ���� DEFAULT_CHAR_VALUE ʱ����value=def_value������1
		   const bool group_is_case_sensitive = false : �����Ƿ��Сд���У�true-��Сд���� / Ĭ��false-��Сд������
		   const bool item_is_case_sensitive = false  : �����Ƿ��Сд���У�true-��Сд���� / Ĭ��false-��Сд������
  �� �� ֵ��1 - ȡ����ȷֵ
			   δȡ��ֵ/δȡ����ȷֵ��������ȱʡֵ��������Ϊȱʡֵ��
		   0 - δȡ����ֻ��Ϊδָ��Ĭ��ֵ������²Ż᷵��0��
  ˵    ����
***************************************************************************/
int config_file_tools::item_get_char(const char* const group_name, const char* const item_name, char& value,
						const char* const choice_set, const char def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	string item_value;
	if (item_get_raw(group_name, item_name, item_value, group_is_case_sensitive, item_is_case_sensitive) == 0)
		return changeValueAndReturn <char> (value, def_value); // û�ҵ�
	if (readFromString(item_value, value) == 0)
		return changeValueAndReturn <char> (value, def_value); // ��ȡʧ��
	if (choice_set != nullptr && strchr(choice_set, value) == nullptr)
		return changeValueAndReturn <char> (value, def_value); // �����ַ���֮��
	return 1;
}

int config_file_tools::item_get_char(const string& group_name, const string& item_name, char& value,
						const char* const choice_set, const char def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	return this->item_get_char(group_name.c_str(), item_name.c_str(), value, choice_set, def_value, group_is_case_sensitive, item_is_case_sensitive);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�ȡĳ������ݣ���������Ϊint��
  ���������const char* const group_name               ������
		   const char* const item_name                ������
		   int& value                                 ��������int��ֵ������1ʱ���ţ�����0�򲻿��ţ�
		   const int min_value = INT_MIN              : �������ݷ�Χ�����ޣ�Ĭ��ΪINT_MIN
		   const int max_value = INT_MAX              : �������ݷ�Χ�����ޣ�Ĭ��ΪINT_MAX
		   const int def_value = DEFAULT_INT_VALUE    ��������/�����Ƿ�������µ�Ĭ��ֵ���ò�����Ĭ��ֵ DEFAULT_INT_VALUE�����������
															��ֵ��   DEFAULT_INT_VALUE ʱ������0��ֵ�����ţ�
															��ֵ���� DEFAULT_INT_VALUE ʱ����value=def_value������1
		   const bool group_is_case_sensitive = false : �����Ƿ��Сд���У�true-��Сд���� / Ĭ��false-��Сд������
		   const bool item_is_case_sensitive = false  : �����Ƿ��Сд���У�true-��Сд���� / Ĭ��false-��Сд������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int config_file_tools::item_get_int(const char* const group_name, const char* const item_name, int& value,
						const int min_value, const int max_value, const int def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	string item_value;
	if (item_get_raw(group_name, item_name, item_value, group_is_case_sensitive, item_is_case_sensitive) == 0)
		return changeValueAndReturn <int> (value, def_value); // û�ҵ�
	if (readFromString(item_value, value) == 0) 
		return changeValueAndReturn <int> (value, def_value); // ��ȡʧ��
	if (value < min_value || value > max_value)
		return changeValueAndReturn <int> (value, def_value); // ������Χ
	return 1;
}

int config_file_tools::item_get_int(const string& group_name, const string& item_name, int& value,
						const int min_value, const int max_value, const int def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	return this->item_get_int(group_name.c_str(), item_name.c_str(), value, min_value, max_value, def_value, group_is_case_sensitive, item_is_case_sensitive);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�ȡĳ������ݣ���������Ϊdouble��
  ���������const char* const group_name                  ������
		   const char* const item_name                   ������
		   double& value                                 ��������int��ֵ������1ʱ���ţ�����0�򲻿��ţ�
		   const double min_value = __DBL_MIN__          : �������ݷ�Χ�����ޣ�Ĭ��ΪINT_MIN
		   const double max_value = __DBL_MAX__          : �������ݷ�Χ�����ޣ�Ĭ��ΪINT_MAX
		   const double def_value = DEFAULT_DOUBLE_VALUE ��������/�����Ƿ�������µ�Ĭ��ֵ���ò�����Ĭ��ֵDEFAULT_DOUBLE_VALUE�����������
																��ֵ��   DEFAULT_DOUBLE_VALUE ʱ������0��ֵ�����ţ�
																��ֵ���� DEFAULT_DOUBLE_VALUE ʱ����value=def_value������1
		   const bool group_is_case_sensitive = false     : �����Ƿ��Сд���У�true-��Сд���� / Ĭ��false-��Сд������
		   const bool item_is_case_sensitive = false      : �����Ƿ��Сд���У�true-��Сд���� / Ĭ��false-��Сд������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int config_file_tools::item_get_double(const char* const group_name, const char* const item_name, double& value,
						const double min_value, const double max_value, const double def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	string item_value;
	if (item_get_raw(group_name, item_name, item_value, group_is_case_sensitive, item_is_case_sensitive) == 0)
		return changeValueAndReturn (value, def_value); // û�ҵ�
	if (readFromString(item_value, value) == 0)
		return changeValueAndReturn (value, def_value); // ��ȡʧ��
	if (value < min_value - DOUBLE_EPS || value > max_value + DOUBLE_EPS)
		return changeValueAndReturn (value, def_value); // ������Χ
	return 1;
}

int config_file_tools::item_get_double(const string& group_name, const string& item_name, double& value,
						const double min_value, const double max_value, const double def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	return this->item_get_double(group_name.c_str(), item_name.c_str(), value, min_value, max_value, def_value, group_is_case_sensitive, item_is_case_sensitive);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�ȡĳ������ݣ���������Ϊchar * / char []��
  ���������const char* const group_name                  ������
		   const char* const item_name                   ������
		   char *const value                             ��������C��ʽ���ַ���������1ʱ���ţ�����0�򲻿��ţ�
		   const int str_maxlen                          ��ָ��Ҫ������󳤶ȣ���β�㣩
																���<1�򷵻ؿմ�(����DEFAULT_CSTRING_VALUE����Ȼ����������ͬ����Ҫ����defaultֵ���ܻ��)
																���>MAX_STRLEN ������ΪMAX_STRLEN
		   const char* const def_str                     ������������µ�Ĭ��ֵ���ò�����Ĭ��ֵDEFAULT_CSTRING_VALUE�����������
																��ֵ��   DEFAULT_CSTRING_VALUE ʱ������0��ֵ�����ţ�
																��ֵ���� DEFAULT_CSTRING_VALUE ʱ����value=def_value������1��ע�⣬����ֱ��=��
		   const bool group_is_case_sensitive = false : �����Ƿ��Сд���У�true-��Сд���� / Ĭ��false-��Сд������
		   const bool item_is_case_sensitive = false  : �����Ƿ��Сд���У�true-��Сд���� / Ĭ��false-��Сд������
  �� �� ֵ��
  ˵    ����1��Ϊ�򻯣�δ��\"����ת�崦��������ͨ�ַ�
		   2����β�����󳤶�Ϊstr_maxlen������ʱҪ��֤���㹻�ռ�
		   3����� str_maxlen ������ϵͳԤ������� MAX_STRLEN���� MAX_STRLEN ȡ
***************************************************************************/
int config_file_tools::item_get_cstring(const char* const group_name, const char* const item_name, char* const value,
						const int str_maxlen, const char* const def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	string item_value;
	if (item_get_raw(group_name, item_name, item_value, group_is_case_sensitive, item_is_case_sensitive) == 0)	
		return changeValueAndReturn (value, def_value); // û�ҵ�
	if (str_maxlen < 1) {
		value[0] = '\0';
		return 0;
	}

	int max_len = (str_maxlen > MAX_STRLEN) ? MAX_STRLEN : str_maxlen;
	if (int(item_value.length()) > max_len - 1)
		item_value = item_value.substr(0, max_len - 1);

	istringstream iss(item_value);
	if (!(iss >> value))
		return changeValueAndReturn (value, def_value); // ��ȡʧ��

	return 1;
}

int config_file_tools::item_get_cstring(const string& group_name, const string& item_name, char* const value,
						const int str_maxlen, const char* const def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)

{
	return item_get_cstring(group_name.c_str(), item_name.c_str(), value, str_maxlen, def_value, group_is_case_sensitive, item_is_case_sensitive);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�ȡĳ������ݣ���������Ϊ string ��
  ���������const char* const group_name               ������
		   const char* const item_name                ������
		   string &value                              ��������string��ʽ���ַ���������1ʱ���ţ�����0�򲻿��ţ�
		   const string &def_value                    ������������µ�Ĭ��ֵ���ò�����Ĭ��ֵDEFAULT_STRING_VALUE�����������
															��ֵ��   DEFAULT_STRING_VALUE ʱ������0��ֵ�����ţ�
															��ֵ���� DEFAULT_STRING_VALUE ʱ����value=def_value������1
		   const bool group_is_case_sensitive = false : �����Ƿ��Сд���У�true-��Сд���� / Ĭ��false-��Сд������
		   const bool item_is_case_sensitive = false  : �����Ƿ��Сд���У�true-��Сд���� / Ĭ��false-��Сд������
  �� �� ֵ��
  ˵    ����Ϊ�򻯣�δ��\"����ת�崦��������ͨ�ַ�
***************************************************************************/
int config_file_tools::item_get_string(const char* const group_name, const char* const item_name, string& value,
						const string& def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	string item_value;
	if (item_get_raw(group_name, item_name, item_value, group_is_case_sensitive, item_is_case_sensitive) == 0)
		return changeValueAndReturn (value, def_value); // û�ҵ�
	if (readFromString(item_value, value) == 0)
		return changeValueAndReturn (value, def_value); // ��ȡʧ��
	return 1;
}

int config_file_tools::item_get_string(const string& group_name, const string& item_name, string& value,
						const string& def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	return this->item_get_string(group_name.c_str(), item_name.c_str(), value, def_value, group_is_case_sensitive, item_is_case_sensitive);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�ȡĳ������ݣ���������Ϊ IPv4 ��ַ��32bit���ͣ�������
  ���������const char* const group_name               ������
		   const char* const item_name                ������
		   unsigned int &value                        ��������IP��ַ��32λ���ͷ�ʽ������1ʱ���ţ�����0�򲻿��ţ�
		   const unsigned int &def_value              ������������µ�Ĭ��ֵ���ò�����Ĭ��ֵDEFAULT_IPADDR_VALUE�����������
															��ֵ��   DEFAULT_IPADDR_VALUE ʱ������0��ֵ�����ţ�
															��ֵ���� DEFAULT_IPADDR_VALUE ʱ����value=def_value������1
		   const bool group_is_case_sensitive = false : �����Ƿ��Сд���У�true-��Сд���� / Ĭ��false-��Сд������
		   const bool item_is_case_sensitive = false  : �����Ƿ��Сд���У�true-��Сд���� / Ĭ��false-��Сд������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int config_file_tools::item_get_ipaddr(const char* const group_name, const char* const item_name, unsigned int& value,
						const unsigned int& def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	string item_value;
	if (item_get_raw(group_name, item_name, item_value, group_is_case_sensitive, item_is_case_sensitive) == 0)
		return changeValueAndReturn <u_int> (value, def_value); // û�ҵ�
	if (!checkIsIPAddr(item_value))
		return changeValueAndReturn <u_int> (value, def_value); // ���ǺϷ���IP��ַ
	value = getIntIPAddr(item_value);
	return 1;
}

int config_file_tools::item_get_ipaddr(const string& group_name, const string& item_name, unsigned int& value,
						const unsigned int& def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	return this->item_get_ipaddr(group_name.c_str(), item_name.c_str(), value, def_value, group_is_case_sensitive, item_is_case_sensitive);
}
