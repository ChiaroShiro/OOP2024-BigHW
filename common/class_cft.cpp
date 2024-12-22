/* 2351871 郎若谷 计科 */ 

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include "../include/class_cft.h"
#include <cctype>
#include <cstring>
#include <sstream>
using namespace std;

// 获取分隔符
static char getSeparator(const enum BREAK_CTYPE _ctype)
{
	if (_ctype == BREAK_CTYPE::Equal)
		return '=';
	else if (_ctype == BREAK_CTYPE::Space)
		return ' ';
	else
		return '\0';
}

// 把string的前后缀不可见字符去除
static void clearStringProSuf(string& str)
{
	str.erase(0, str.find_first_not_of(" \t\n\r\f\v"));
	str.erase(str.find_last_not_of(" \t\n\r\f\v") + 1);
}

typedef unsigned int u_int;

// 检查IP地址格式
static bool checkIsIPAddr(string s)
{
	int cnt = 0; // 记录点的数量
	int pos[5] = {0}; // 记录每个点的位置
	for(u_int i = 0; i < s.size(); i++) {
		if(s[i] == '.') {
			pos[++cnt] = i; // 记录点的位置
			if(cnt > 3) // 如果点的数量超过3个，返回false
				return false;
		}
	}
	if(cnt != 3) // 如果点的数量不等于3，返回false
		return false;
	pos[0] = -1;
	pos[4] = s.size() + 1;
	string subs[4]; // 存储每个分段
	for(int i = 0; i < 4; i++) {
		subs[i] = s.substr(pos[i] + 1, pos[i + 1] - pos[i] - 1); // 提取分段
		if(subs[i] == "") // 如果分段为空，返回false
			return false;
		for(u_int j = 0; j < subs[i].size(); j++) {
			if(!isdigit(subs[i][j])) // 如果分段中有非数字字符，返回false
				return false;
		}
		int num = atoi(subs[i].c_str()); // 将分段转换为整数
		if(num < 0 || num > 255) // 如果整数不在0到255之间，返回false
			return false;
	}
	return true; // 如果所有检查都通过，返回true
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

// 读取配置文件
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
	while (fs) { // 处理注释内容
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
		if (line.front() == '[' && line.back() == ']') { // 处理组名
			noGroupInit = 0;
			currentGroup = line;
			data.push_back(make_pair(currentGroup, vector<ITEM_TYPE>()));
			lineraw.push_back(make_pair(currentGroup, vector<string>()));
		} else {
			if (noGroupInit) { // 处理没有组名的情况
				currentGroup = SIMPLE_GNAME;
				noGroupInit = 0;
				data.push_back(make_pair(currentGroup, vector<ITEM_TYPE>()));
				lineraw.push_back(make_pair(currentGroup, vector<string>()));
			}
			lineraw.back().second.push_back(line);
			size_t separatorPos = line.find(getSeparator(item_separate_character_type));
			if (separatorPos != string::npos) { // 处理项名和值
				string itemName = line.substr(0, separatorPos);
				string itemValue = line.substr(separatorPos + 1);
				clearStringProSuf(itemName);
				clearStringProSuf(itemValue);
				data.back().second.push_back(make_pair(itemName, itemValue));
			}
			else // 处理没有值的情况
				data.back().second.push_back(make_pair(line, ""));
		}
	}
}

// 不改变字符串，用于区分大小写的情景
static string transferNoChange(const string& str)
{
	return str;
}

// 将字符串全部转为小写，用于不区分大小写的情景
static string transferToLower(const string& str)
{
	string ret = str;
	for (char& ch : ret)
		ch = tolower(ch);
	return ret;
}

typedef string (*TRANS_TYPE)(const string&);

// 从字符串中读取值
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

// 获取默认值
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

// 获取字符串的默认值，针对string特化
template <>
static string getDefaultValue()
{
	return DEFAULT_STRING_VALUE;
}

// 对于默认值的通用处理逻辑
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

// 针对char*的特化
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
  函数名称：
  功    能：判断读配置文件是否成功
  输入参数：
  返 回 值：true - 成功，已读入所有的组/项
		   false - 失败，文件某行超长/文件全部是注释语句
  说    明：
***************************************************************************/
bool config_file_tools::is_read_succeeded() const
{
	return is_read_succeeded_value;
}

/***************************************************************************
  函数名称：
  功    能：返回配置文件中的所有组
  输入参数：vector <string>& ret : vector 中每项为一个组名
  返 回 值：读到的组的数量（简单配置文件的组数量为1，组名为"）
  说    明：
***************************************************************************/
int config_file_tools::get_all_group(vector <string>& ret)
{
	ret.clear();
	for (const DATA_TYPE& group : data)
		ret.push_back(group.first);
	return ret.size();
}

/***************************************************************************
  函数名称：
  功    能：查找指定组的所有项并返回项的原始内容
  输入参数：const string& group_name：组名
		   vector <string>& ret：vector 中每项为一个项的原始内容
		   const bool is_case_sensitive = false : 组名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
  返 回 值：项的数量，0表示空
  说    明：
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
  函数名称：
  功    能：取某项的原始内容（=后的所有字符，string方式）
  输入参数：const char* const group_name
		   const char* const item_name
		   string &ret
		   const bool group_is_case_sensitive = false : 组名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
		   const bool item_is_case_sensitive = false  : 项名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
  返 回 值：
  说    明：
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
  函数名称：
  功    能：取某项的内容，返回类型为char型
  输入参数：const char* const group_name               ：组名
		   const char* const item_name                ：项名
		   const bool group_is_case_sensitive = false : 组名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
		   const bool item_is_case_sensitive = false  : 项名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
  返 回 值：1 - 该项的项名存在
		   0 - 该项的项名不存在
  说    明：
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
  函数名称：
  功    能：取某项的内容，返回类型为char型
  输入参数：const char* const group_name               ：组名
		   const char* const item_name                ：项名
		   char& value                                ：读到的char的值（返回1时可信，返回0则不可信）
		   const char* const choice_set = nullptr     ：合法的char的集合（例如："YyNn"表示合法输入为Y/N且不分大小写，该参数有默认值nullptr，表示全部字符，即不检查）
		   const char def_value = DEFAULT_CHAR_VALUE  ：读不到/读到非法的情况下的默认值，该参数有默认值DEFAULT_CHAR_VALUE，分两种情况
															当值是   DEFAULT_CHAR_VALUE 时，返回0（值不可信）
															当值不是 DEFAULT_CHAR_VALUE 时，令value=def_value并返回1
		   const bool group_is_case_sensitive = false : 组名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
		   const bool item_is_case_sensitive = false  : 项名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
  返 回 值：1 - 取到正确值
			   未取到值/未取到正确值，设置了缺省值（包括设为缺省值）
		   0 - 未取到（只有为未指定默认值的情况下才会返回0）
  说    明：
***************************************************************************/
int config_file_tools::item_get_char(const char* const group_name, const char* const item_name, char& value,
						const char* const choice_set, const char def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	string item_value;
	if (item_get_raw(group_name, item_name, item_value, group_is_case_sensitive, item_is_case_sensitive) == 0)
		return changeValueAndReturn <char> (value, def_value); // 没找到
	if (readFromString(item_value, value) == 0)
		return changeValueAndReturn <char> (value, def_value); // 读取失败
	if (choice_set != nullptr && strchr(choice_set, value) == nullptr)
		return changeValueAndReturn <char> (value, def_value); // 不在字符集之内
	return 1;
}

int config_file_tools::item_get_char(const string& group_name, const string& item_name, char& value,
						const char* const choice_set, const char def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	return this->item_get_char(group_name.c_str(), item_name.c_str(), value, choice_set, def_value, group_is_case_sensitive, item_is_case_sensitive);
}

/***************************************************************************
  函数名称：
  功    能：取某项的内容，返回类型为int型
  输入参数：const char* const group_name               ：组名
		   const char* const item_name                ：项名
		   int& value                                 ：读到的int的值（返回1时可信，返回0则不可信）
		   const int min_value = INT_MIN              : 期望数据范围的下限，默认为INT_MIN
		   const int max_value = INT_MAX              : 期望数据范围的上限，默认为INT_MAX
		   const int def_value = DEFAULT_INT_VALUE    ：读不到/读到非法的情况下的默认值，该参数有默认值 DEFAULT_INT_VALUE，分两种情况
															当值是   DEFAULT_INT_VALUE 时，返回0（值不可信）
															当值不是 DEFAULT_INT_VALUE 时，令value=def_value并返回1
		   const bool group_is_case_sensitive = false : 组名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
		   const bool item_is_case_sensitive = false  : 项名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
  返 回 值：
  说    明：
***************************************************************************/
int config_file_tools::item_get_int(const char* const group_name, const char* const item_name, int& value,
						const int min_value, const int max_value, const int def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	string item_value;
	if (item_get_raw(group_name, item_name, item_value, group_is_case_sensitive, item_is_case_sensitive) == 0)
		return changeValueAndReturn <int> (value, def_value); // 没找到
	if (readFromString(item_value, value) == 0) 
		return changeValueAndReturn <int> (value, def_value); // 读取失败
	if (value < min_value || value > max_value)
		return changeValueAndReturn <int> (value, def_value); // 超出范围
	return 1;
}

int config_file_tools::item_get_int(const string& group_name, const string& item_name, int& value,
						const int min_value, const int max_value, const int def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	return this->item_get_int(group_name.c_str(), item_name.c_str(), value, min_value, max_value, def_value, group_is_case_sensitive, item_is_case_sensitive);
}

/***************************************************************************
  函数名称：
  功    能：取某项的内容，返回类型为double型
  输入参数：const char* const group_name                  ：组名
		   const char* const item_name                   ：项名
		   double& value                                 ：读到的int的值（返回1时可信，返回0则不可信）
		   const double min_value = __DBL_MIN__          : 期望数据范围的下限，默认为INT_MIN
		   const double max_value = __DBL_MAX__          : 期望数据范围的上限，默认为INT_MAX
		   const double def_value = DEFAULT_DOUBLE_VALUE ：读不到/读到非法的情况下的默认值，该参数有默认值DEFAULT_DOUBLE_VALUE，分两种情况
																当值是   DEFAULT_DOUBLE_VALUE 时，返回0（值不可信）
																当值不是 DEFAULT_DOUBLE_VALUE 时，令value=def_value并返回1
		   const bool group_is_case_sensitive = false     : 组名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
		   const bool item_is_case_sensitive = false      : 项名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
  返 回 值：
  说    明：
***************************************************************************/
int config_file_tools::item_get_double(const char* const group_name, const char* const item_name, double& value,
						const double min_value, const double max_value, const double def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	string item_value;
	if (item_get_raw(group_name, item_name, item_value, group_is_case_sensitive, item_is_case_sensitive) == 0)
		return changeValueAndReturn (value, def_value); // 没找到
	if (readFromString(item_value, value) == 0)
		return changeValueAndReturn (value, def_value); // 读取失败
	if (value < min_value - DOUBLE_EPS || value > max_value + DOUBLE_EPS)
		return changeValueAndReturn (value, def_value); // 超出范围
	return 1;
}

int config_file_tools::item_get_double(const string& group_name, const string& item_name, double& value,
						const double min_value, const double max_value, const double def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	return this->item_get_double(group_name.c_str(), item_name.c_str(), value, min_value, max_value, def_value, group_is_case_sensitive, item_is_case_sensitive);
}

/***************************************************************************
  函数名称：
  功    能：取某项的内容，返回类型为char * / char []型
  输入参数：const char* const group_name                  ：组名
		   const char* const item_name                   ：项名
		   char *const value                             ：读到的C方式的字符串（返回1时可信，返回0则不可信）
		   const int str_maxlen                          ：指定要读的最大长度（含尾零）
																如果<1则返回空串(不是DEFAULT_CSTRING_VALUE，虽然现在两者相同，但要考虑default值可能会改)
																如果>MAX_STRLEN 则上限为MAX_STRLEN
		   const char* const def_str                     ：读不到情况下的默认值，该参数有默认值DEFAULT_CSTRING_VALUE，分两种情况
																当值是   DEFAULT_CSTRING_VALUE 时，返回0（值不可信）
																当值不是 DEFAULT_CSTRING_VALUE 时，令value=def_value并返回1（注意，不是直接=）
		   const bool group_is_case_sensitive = false : 组名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
		   const bool item_is_case_sensitive = false  : 项名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
  返 回 值：
  说    明：1、为简化，未对\"等做转义处理，均按普通字符
		   2、含尾零的最大长度为str_maxlen，调用时要保证有足够空间
		   3、如果 str_maxlen 超过了系统预设的上限 MAX_STRLEN，则按 MAX_STRLEN 取
***************************************************************************/
int config_file_tools::item_get_cstring(const char* const group_name, const char* const item_name, char* const value,
						const int str_maxlen, const char* const def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	string item_value;
	if (item_get_raw(group_name, item_name, item_value, group_is_case_sensitive, item_is_case_sensitive) == 0)	
		return changeValueAndReturn (value, def_value); // 没找到
	if (str_maxlen < 1) {
		value[0] = '\0';
		return 0;
	}

	int max_len = (str_maxlen > MAX_STRLEN) ? MAX_STRLEN : str_maxlen;
	if (int(item_value.length()) > max_len - 1)
		item_value = item_value.substr(0, max_len - 1);

	istringstream iss(item_value);
	if (!(iss >> value))
		return changeValueAndReturn (value, def_value); // 读取失败

	return 1;
}

int config_file_tools::item_get_cstring(const string& group_name, const string& item_name, char* const value,
						const int str_maxlen, const char* const def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)

{
	return item_get_cstring(group_name.c_str(), item_name.c_str(), value, str_maxlen, def_value, group_is_case_sensitive, item_is_case_sensitive);
}

/***************************************************************************
  函数名称：
  功    能：取某项的内容，返回类型为 string 型
  输入参数：const char* const group_name               ：组名
		   const char* const item_name                ：项名
		   string &value                              ：读到的string方式的字符串（返回1时可信，返回0则不可信）
		   const string &def_value                    ：读不到情况下的默认值，该参数有默认值DEFAULT_STRING_VALUE，分两种情况
															当值是   DEFAULT_STRING_VALUE 时，返回0（值不可信）
															当值不是 DEFAULT_STRING_VALUE 时，令value=def_value并返回1
		   const bool group_is_case_sensitive = false : 组名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
		   const bool item_is_case_sensitive = false  : 项名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
  返 回 值：
  说    明：为简化，未对\"等做转义处理，均按普通字符
***************************************************************************/
int config_file_tools::item_get_string(const char* const group_name, const char* const item_name, string& value,
						const string& def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	string item_value;
	if (item_get_raw(group_name, item_name, item_value, group_is_case_sensitive, item_is_case_sensitive) == 0)
		return changeValueAndReturn (value, def_value); // 没找到
	if (readFromString(item_value, value) == 0)
		return changeValueAndReturn (value, def_value); // 读取失败
	return 1;
}

int config_file_tools::item_get_string(const string& group_name, const string& item_name, string& value,
						const string& def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	return this->item_get_string(group_name.c_str(), item_name.c_str(), value, def_value, group_is_case_sensitive, item_is_case_sensitive);
}

/***************************************************************************
  函数名称：
  功    能：取某项的内容，返回类型为 IPv4 地址的32bit整型（主机序）
  输入参数：const char* const group_name               ：组名
		   const char* const item_name                ：项名
		   unsigned int &value                        ：读到的IP地址，32位整型方式（返回1时可信，返回0则不可信）
		   const unsigned int &def_value              ：读不到情况下的默认值，该参数有默认值DEFAULT_IPADDR_VALUE，分两种情况
															当值是   DEFAULT_IPADDR_VALUE 时，返回0（值不可信）
															当值不是 DEFAULT_IPADDR_VALUE 时，令value=def_value并返回1
		   const bool group_is_case_sensitive = false : 组名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
		   const bool item_is_case_sensitive = false  : 项名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
  返 回 值：
  说    明：
***************************************************************************/
int config_file_tools::item_get_ipaddr(const char* const group_name, const char* const item_name, unsigned int& value,
						const unsigned int& def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	string item_value;
	if (item_get_raw(group_name, item_name, item_value, group_is_case_sensitive, item_is_case_sensitive) == 0)
		return changeValueAndReturn <u_int> (value, def_value); // 没找到
	if (!checkIsIPAddr(item_value))
		return changeValueAndReturn <u_int> (value, def_value); // 不是合法的IP地址
	value = getIntIPAddr(item_value);
	return 1;
}

int config_file_tools::item_get_ipaddr(const string& group_name, const string& item_name, unsigned int& value,
						const unsigned int& def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	return this->item_get_ipaddr(group_name.c_str(), item_name.c_str(), value, def_value, group_is_case_sensitive, item_is_case_sensitive);
}
