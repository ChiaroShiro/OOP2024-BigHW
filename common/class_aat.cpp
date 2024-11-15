/* 2351871 郎若谷 计科*/
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <cstring>
#include "../include/class_aat.h"
//如有必要，可以加入其它头文件
using namespace std;

#if !ENABLE_LIB_COMMON_TOOLS //不使用lib才有效

/* ---------------------------------------------------------------
	 允许加入其它需要static函数（内部工具用）
   ---------------------------------------------------------------- */

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：null
 ***************************************************************************/
args_analyse_tools::args_analyse_tools()
{
	memset(this, 0, sizeof(*this));
	args_name = string("");
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：boolean
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const ST_EXTARGS_TYPE type, const int ext_num, const bool def)
{
	memset(this, 0, sizeof(*this));
	this->args_name = string(name);
	this->extargs_type = type;
	this->extargs_num = ext_num;
	this->args_existed = 0;

	this->extargs_bool_default = def;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：int_with_default、int_with_error
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const ST_EXTARGS_TYPE type, const int ext_num, const int def, const int _min, const int _max)
{
	memset(this, 0, sizeof(*this));
	this->args_name = string(name);
	this->extargs_type = type;
	this->extargs_num = ext_num;
	this->args_existed = 0;

	extargs_int_default = def;
	extargs_int_min = _min;
	extargs_int_max = _max;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：int_with_set_default、int_with_set_error
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const enum ST_EXTARGS_TYPE type, const int ext_num, const int def_of_set_pos, const int* const set)
{
	memset(this, 0, sizeof(*this));
	this->args_name = string(name);
	this->extargs_type = type;
	this->extargs_num = ext_num;
	this->args_existed = 0;

	extargs_int_default = set[def_of_set_pos];
	int cnt = 1;
	const int* p = set;
	while (*p != INVALID_INT_VALUE_OF_SET) {
		++cnt;
		p++;
	}
	extargs_int_set = new int[cnt];
	memcpy(extargs_int_set, set, cnt * sizeof(int));
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：str、ipaddr_with_default、ipaddr_with_error
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const ST_EXTARGS_TYPE type, const int ext_num, const string def)
{
	memset(this, 0, sizeof(*this));
	this->args_name = string(name);
	this->extargs_type = type;
	this->extargs_num = ext_num;
	this->args_existed = 0;

	extargs_string_default = string(def);
	//cout << "find STR GOUZAO: " << def << " :: " << extargs_string_default << endl;
	if (type == ST_EXTARGS_TYPE::ipaddr_with_default || type == ST_EXTARGS_TYPE::ipaddr_with_error) {
		//cout << "GET in!!\n";
		extargs_ipaddr_default = get_int_ipaddr();
	}
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：str_with_set_default、str_with_set_error
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const ST_EXTARGS_TYPE type, const int ext_num, const int def_of_set_pos, const string* const set)
{
	memset(this, 0, sizeof(*this));
	this->args_name = string(name);
	this->extargs_type = type;
	this->extargs_num = ext_num;
	this->args_existed = 0;

	extargs_string_default = set[def_of_set_pos];
	int cnt = 1;
	const string* p = set;
	while (*p != string("")) {
		++cnt;
		p++;
	}
	extargs_string_set = new string[cnt];
	p = set;
	for (int i = 0; i < cnt; i++)
		extargs_string_set[i] = string(set[i]);
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：double_with_default、double_with_error
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const ST_EXTARGS_TYPE type, const int ext_num, const double	def, const double _min, const double _max)
{
	memset(this, 0, sizeof(*this));
	this->args_name = string(name);
	this->extargs_type = type;
	this->extargs_num = ext_num;
	this->args_existed = 0;

	extargs_double_default = def;
	extargs_double_min = _min;
	extargs_double_max = _max;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：double_with_set_default、double_with_set_error
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const enum ST_EXTARGS_TYPE type, const int ext_num, const int def_of_set_pos, const double* const set)
{
	memset(this, 0, sizeof(*this));
	this->args_name = string(name);
	this->extargs_type = type;
	this->extargs_num = ext_num;
	this->args_existed = 0;

	extargs_double_default = set[def_of_set_pos];
	int cnt = 1;
	const double* p = set;
	while (*p != INVALID_DOUBLE_VALUE_OF_SET) {
		++cnt;
		p++;
	}
	extargs_double_set = new double[cnt];
	memcpy(extargs_double_set, set, cnt * sizeof(double));
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
 ***************************************************************************/
args_analyse_tools::~args_analyse_tools()
{
	if (this->extargs_int_set != NULL)
		delete[] this->extargs_int_set;
	if (this->extargs_double_set != NULL)
		delete[] this->extargs_double_set;
	if (this->extargs_string_set != NULL)
		delete[] this->extargs_string_set;
}

/* ---------------------------------------------------------------
	 允许AAT中自定义成员函数的实现（private）
   ---------------------------------------------------------------- */

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：已实现，不要动
 ***************************************************************************/
const string args_analyse_tools::get_name() const
{
	return this->args_name;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：加!!后，只能是0/1
			已实现，不要动
 ***************************************************************************/
const int args_analyse_tools::existed() const
{
	return !!this->args_existed;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：已实现，不要动
 ***************************************************************************/
const int args_analyse_tools::get_int() const
{
	return this->extargs_int_value;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：已实现，不要动
 ***************************************************************************/
const double args_analyse_tools::get_double() const
{
	return this->extargs_double_value;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：已实现，不要动
 ***************************************************************************/
const string args_analyse_tools::get_string() const
{
	return this->extargs_string_value;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：已实现，不要动
 ***************************************************************************/
const unsigned int args_analyse_tools::get_ipaddr() const
{
	return this->extargs_ipaddr_value;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：将 extargs_ipaddr_value 的值从 0x7f000001 转为 "127.0.0.1"
 ***************************************************************************/
const string args_analyse_tools::get_str_ipaddr() const
{
	string ret;
	int ipval = extargs_ipaddr_value;
	for (int i = 0; i < 4; i++) {
		ret = to_string(ipval & ((1u << 8) - 1)) + ret;
		if (i != 3)
			ret = "." + ret;
		ipval >>= 8;
	}
	return ret; //此句根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：将 extargs_string_default 的值从 "127.0.0.1" 转为 0x7f000001
 ***************************************************************************/
const u_int args_analyse_tools::get_int_ipaddr() const
{
	int pos = 0, nxtpos = 0;
	u_int ret = 0;
	while ((nxtpos = extargs_string_default.find('.', pos)) != -1) {
		//cout << "pos = " << pos << ", nextpos = " << nxtpos << endl;
		ret = (ret << 8) + atoi(extargs_string_default.substr(pos, nxtpos - pos).c_str());
		pos = nxtpos + 1;
	}
	nxtpos = extargs_string_default.size();
	ret = (ret << 8) + atoi(extargs_string_default.substr(pos, nxtpos - pos).c_str());

	//cout << "Tester: s = " << extargs_string_default << ", val = " << hex << ret << dec << endl;
	//extargs_ipaddr_value = ret;
	//cout << "reS = " << get_str_ipaddr() << endl;

	return ret;
}

void args_analyse_tools::giveInitValue()
{
	extargs_int_value = extargs_int_default;
	extargs_double_value = extargs_double_default;
	extargs_string_value = extargs_string_default;
	extargs_ipaddr_value = extargs_ipaddr_default;
}

static args_analyse_tools* findOption(args_analyse_tools* const args, const char* const str)
{
	args_analyse_tools* ptr = args;
	while(ptr->get_name() != string("")) {
		if(string(str) == ptr->get_name()) {
			return ptr;
		}
		++ptr;
	}
	return NULL;
}

static bool checkIsCommand(const char* const cmd) 
{
	if(strlen(cmd) < 3)
		return 0;
	return cmd[0] == '-' && cmd[1] == '-';
}

static string printTypeInfo(const args_analyse_tools x)
{
	static string TYPE_NAME[] = {"bool", "int", "double", "string"};
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：follow_up_args：是否有后续参数
			0  ：无后续参数
			1  ：有后续参数
  返 回 值：
  说    明：友元函数
***************************************************************************/
int args_analyse_process(const int argc, const char* const *const argv, args_analyse_tools* const args, const int follow_up_args)
{
	int retcnt = 0;
	for(retcnt = 1; retcnt < argc; retcnt++) {
		if(checkIsCommand(argv[retcnt]) == 0) {
			cout << "参数[" << argv[retcnt] << "]格式非法(不是--开头的有效内容)\n";
		}
		args_analyse_tools *opt = findOption(args, argv[retcnt]);
		if(opt == NULL) // 找不到这个参数
			break;
		opt->args_existed = 1;
		if(opt->extargs_num == 0)
			continue;
		if(retcnt + 1 >= argc) {
			cout << "参数[" << argv[retcnt] << "]缺少附加参数. (";
			printTypeInfo(*opt);
			cout << ")\n";
		}
	}
	if(retcnt == argc || follow_up_args)
		return retcnt;
	cout << "参数[" << argv[retcnt] << "]非法\n";
	return -1; //此句根据需要修改
}


/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：友元函数
***************************************************************************/
int args_analyse_print(const args_analyse_tools* const args)
{
	int* ptr = args[4].extargs_int_set;
	while (*ptr != INVALID_INT_VALUE_OF_SET) {
		cout << *ptr << ", ";
		ptr++;
	}
	cout << endl;
	return 0; //此句根据需要修改
}

#endif // !ENABLE_LIB_COMMON_TOOLS
