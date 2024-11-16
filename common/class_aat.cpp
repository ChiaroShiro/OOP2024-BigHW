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

template <class _A, class _B>
static _A max(const _A &a, const _B &b)
{
	return a > (_A)b ? a : b;
}

template <class _T>
static _T invalidVal() // 返回对应类型的末尾数值
{
	if (typeid(_T) == typeid(int))
		return (_T)(INVALID_INT_VALUE_OF_SET);
	else
		return _T();
}

template <>
static double invalidVal() // 不能模板元所以要特化double
{
	return INVALID_DOUBLE_VALUE_OF_SET;
}

template <>
static string invalidVal() // 不能模板元所以要特化string
{
	return string("");
}

// IP addr: uint->string
static string getStringIPAddr(const u_int ip)
{

	string ret;
	u_int ipval = ip;
	for (int i = 0; i < 4; i++) {
		ret = to_string(ipval & ((1u << 8) - 1)) + ret;
		if (i != 3)
			ret = "." + ret;
		ipval >>= 8;
	}
	return ret;
}

// IP addr: string->uint
static u_int getIntIPAddr(const string s)
{
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
	if(extargs_int_set == NULL)
		return;
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
	if (type == ST_EXTARGS_TYPE::ipaddr_with_default || type == ST_EXTARGS_TYPE::ipaddr_with_error) {
		extargs_ipaddr_default = getIntIPAddr(extargs_string_default);
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

	extargs_string_default = string(set[def_of_set_pos]);
	int cnt = 1;
	const string* p = set;
	while (*p != string("")) {
		++cnt;
		p++;
	}
	extargs_string_set = new string[cnt];
	if(extargs_string_set == NULL) 
		return;
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
	if(extargs_double_set == NULL)
		return;
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
	return getStringIPAddr(extargs_ipaddr_value);
}

void args_analyse_tools::giveInitValue()
{
	extargs_int_value = extargs_int_default;
	extargs_double_value = extargs_double_default;
	extargs_string_value = string(extargs_string_default);
	extargs_ipaddr_value = extargs_ipaddr_default;
}

static args_analyse_tools* findOption(args_analyse_tools* const args, const char* const str)
{
	args_analyse_tools* ptr = args;
	while(ptr->get_name() != invalidVal <string> ()) {
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

static string getTypeInChinese(const ST_EXTARGS_TYPE t)
{
	if(t == ST_EXTARGS_TYPE::boolean)
		return "布尔";
	if(t == ST_EXTARGS_TYPE::int_with_default || t == ST_EXTARGS_TYPE::int_with_error || t == ST_EXTARGS_TYPE::int_with_set_default || t == ST_EXTARGS_TYPE::int_with_set_error)
		return "整数";
	if(t == ST_EXTARGS_TYPE::double_with_default || t == ST_EXTARGS_TYPE::double_with_error || t == ST_EXTARGS_TYPE::double_with_set_default || t == ST_EXTARGS_TYPE::double_with_set_error)
		return "浮点数";
	if(t == ST_EXTARGS_TYPE::str || t == ST_EXTARGS_TYPE::str_with_set_default || t == ST_EXTARGS_TYPE::str_with_set_error)
		return "字符串";
	if(t == ST_EXTARGS_TYPE::ipaddr_with_default || t == ST_EXTARGS_TYPE::ipaddr_with_error)
		return "IP地址";
	return "类型错误!";
}

static string getType(const ST_EXTARGS_TYPE t)
{
	if(t == ST_EXTARGS_TYPE::boolean)
		return "bool";
	if(t == ST_EXTARGS_TYPE::int_with_default || t == ST_EXTARGS_TYPE::int_with_error || t == ST_EXTARGS_TYPE::int_with_set_default || t == ST_EXTARGS_TYPE::int_with_set_error)
		return "int";
	if(t == ST_EXTARGS_TYPE::double_with_default || t == ST_EXTARGS_TYPE::double_with_error || t == ST_EXTARGS_TYPE::double_with_set_default || t == ST_EXTARGS_TYPE::double_with_set_error)
		return "double";
	if(t == ST_EXTARGS_TYPE::str || t == ST_EXTARGS_TYPE::str_with_set_default || t == ST_EXTARGS_TYPE::str_with_set_error)
		return "string";
	if(t == ST_EXTARGS_TYPE::ipaddr_with_default || t == ST_EXTARGS_TYPE::ipaddr_with_error)
		return "IP地址";
	return "TypeError!";
}

static bool isDefualt(const ST_EXTARGS_TYPE t, const string strde)
{
	if(t == ST_EXTARGS_TYPE::int_with_default || t == ST_EXTARGS_TYPE::int_with_set_default)
		return 1;
	if(t == ST_EXTARGS_TYPE::double_with_default || t == ST_EXTARGS_TYPE::double_with_set_default)
		return 1;
	if(t == ST_EXTARGS_TYPE::str_with_set_default || t == ST_EXTARGS_TYPE::ipaddr_with_default)
		return 1;
	if(t == ST_EXTARGS_TYPE::str || t == ST_EXTARGS_TYPE::ipaddr_with_error) {
		if(strde == "")
			return 0;
		else 
			return 1;
	}
	if(t == ST_EXTARGS_TYPE::boolean)
		return 1;
	return 0;
}

static bool isSet(const ST_EXTARGS_TYPE t)
{
	if(t == ST_EXTARGS_TYPE::int_with_set_default || t == ST_EXTARGS_TYPE::int_with_set_error)
		return 1;
	if(t == ST_EXTARGS_TYPE::double_with_set_default || t == ST_EXTARGS_TYPE::double_with_set_error)
		return 1;
	if(t == ST_EXTARGS_TYPE::str_with_set_default || t == ST_EXTARGS_TYPE::str_with_set_error)
		return 1;
	return 0;
}

static bool isRange(const ST_EXTARGS_TYPE t)
{
	if(t == ST_EXTARGS_TYPE::int_with_default || t == ST_EXTARGS_TYPE::int_with_error)
		return 1;
	if(t == ST_EXTARGS_TYPE::double_with_default || t == ST_EXTARGS_TYPE::double_with_error)
		return 1;
	return 0;
}

static bool checkIsIPAddr(string s)
{
	int cnt = 0;
	int pos[5] = {0};
	for(u_int i = 0; i < s.size(); i++) {
		cnt += (s[i] == '.');
		pos[cnt] = i;
		if(cnt > 3)
			return 0;
	}
	if(cnt != 3)
		return 0;
	pos[4] = s.size();
	string subs[4];
	for(int i = 0; i < 4; i++) {
		subs[i] = s.substr(pos[i], pos[i + 1] - pos[i]);
		if(subs[i] == "")
			return 0;
		for(u_int j = 0; j < subs[i].size(); j++) {
			if(isdigit(subs[i][j]) == 0)
				return 0;
		}
		if(atoi(subs[i].c_str()) < 0 || atoi(subs[i].c_str()) > 255)
			return 0;
	}
	return 1;
}

template <class _T>
static string generateList(_T* ptr) // 打印 a/b/c/d 这种东西
{
	string ret;
	stringstream ssm;
	_T *p = ptr;
	bool flag = 1;
	while(*p != invalidVal <_T> ()) {
		if(flag)
			flag = 0;
		else 
			ssm << "/";
		ssm << fixed << setprecision(6) << *p;
		p++;
	}
	ssm >> ret;
	return ret;
}

template <class _T>
static string generateRange(_T mn, _T mx)
{
	string ret;
	stringstream ssm;
	ssm << "[" << fixed << setprecision(6) << mn << ".." << fixed << setprecision(6) << mx << "]";
	ssm >> ret;
	return ret;
}

template <class _T>
static bool checkInputTypeError_attachPart(_T &x, stringstream &sin)
{
	string ts;
	bool flag1, flag2;
	sin >> x;
	flag1 = sin.fail();
	sin >> ts;
	flag2 = sin.good();
	return flag1 || flag2; // 第一次失败或第二次成功都是错误
}

static bool checkInputTypeError(string s, string type)
{
	stringstream sin;
	sin.str(s);
	if(type == "int") {
		int x;
		return checkInputTypeError_attachPart(x, sin);
	}
	if(type == "double") {
		double x;
		return checkInputTypeError_attachPart(x, sin);
	}
	if(type == "string") {
		return 0;
	}
	if(type == "IP地址") {
		return !checkIsIPAddr(s);
	}
	return 0;
}

template <class _T>
static bool IsInList(string s, _T *li)
{
	stringstream sin;
	sin.str(s);
	_T x, *ptr = li;
	sin >> x;
	while(*ptr != invalidVal <_T> ()) {
		if(*ptr == x)
			return 1;
		ptr++;
	}
	return 0;
}

#define LIST_BOOL_POS 	0
#define LIST_INT_POS  	1
#define LIST_DOUBLE_POS 2
#define LIST_STRING_POS 3
#define LIST_IP_POS 	4

static bool checkNotInSet(string s, string type, void** setlist)
{
	if(type == "int") {
		int* setl = (int*)setlist[LIST_INT_POS];
		return !IsInList(s, setl);
	}
	if(type == "double") {
		double* setl = (double*)setlist[LIST_DOUBLE_POS];
		return !IsInList(s, setl);
	}
	if(type == "string") {
		string* setl = (string*)setlist[LIST_STRING_POS];
		return !IsInList(s, setl);
	}
	return 0;
}

static bool checkNotInRange(string s, string type, void** rangelist)
{
	stringstream sin(s);
	if (type == "int") {
		int x;
		sin >> x;
		int mn = **(int**)rangelist;
		int mx = *(*(int**)rangelist + 1);
		return x >= mn && x <= mx;
	}
	if (type == "double") {
		double x;
		sin >> x;
		double mn = **(double**)rangelist;
		double mx = *(*(double**)rangelist + 1);
		return x >= mn - DOUBLE_DELTA && x <= mx + DOUBLE_DELTA;
	}
	return 0;
}

static bool needPrintTypeInfo(int retcnt, int argc, string arg, ST_EXTARGS_TYPE opttype, void** setlist, void** rangelist, string strde)
{
	string type = getType(opttype);
	if(retcnt + 1 >= argc) // 后面没东西了
		return 1;
	if(checkIsCommand(arg.c_str())) // 后面是个指令
		return 1;
	if(checkInputTypeError(arg, type)) // 输入类型错误
		return 1;
	if(isSet(opttype) && !isDefualt(opttype, strde) && checkNotInSet(arg, type, setlist)) // 不在 set 内
		return 1;
	if(isRange(opttype) && !isDefualt(opttype, strde) && checkNotInRange(arg, type, rangelist)) // 不在范围内
		return 1;
	return 0;
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
			return -1;
		}
		args_analyse_tools *opt = findOption(args, argv[retcnt]);
		if(opt == NULL) // 找不到这个参数
			break;
		opt->args_existed = 1;
		if(opt->extargs_num == 0)
			continue;
		void* setlist[] = {NULL, opt->extargs_int_set, opt->extargs_double_set, opt->extargs_string_set};
		void* rangelist[] = {NULL, &opt->extargs_int_min, &opt->extargs_double_min};
		
		string type = getType(opt->extargs_type);
		if(needPrintTypeInfo(retcnt, argc, argv[retcnt + 1], opt->extargs_type, setlist, rangelist, opt->extargs_string_default)) { // 参数不足时错误处理，只有这个函数有友元不知道有什么更简明的实现方式了
			// 输出报错信息
			if(retcnt + 1 >= argc) 
				cout << "参数[" << argv[retcnt] << "]的附加参数不足. (";
			else if(checkIsCommand(argv[retcnt + 1]))
				cout << "参数[" << argv[retcnt] << "]缺少附加参数. (";
			else if(checkInputTypeError(argv[retcnt + 1], type)) {
				if(type == "IP地址" && !checkIsIPAddr(argv[retcnt + 1])) { // IP 地址错误直接返回即可
					cout << "参数[--iperr]的附加参数值(1)非法. (类型:IP地址)";
					return -1;
				}
				cout << "参数[" << argv[retcnt] << "]的附加参数不是" << getTypeInChinese(opt->extargs_type) << ". (";
			}
			else {
				if(type == "int") {
					int x = atoi(argv[retcnt + 1]);
					cout << "参数[" << argv[retcnt] << "]的附加参数值(" << x << ")非法. (";
				}
				else if(type == "double") {
					double x = stod(argv[retcnt + 1]);
					cout << "参数[" << argv[retcnt] << "]的附加参数值(" << x << ")非法. (";
				}
				else if(type == "string")
					cout << "参数[" << argv[retcnt] << "]的附加参数值(" << argv[retcnt + 1] << ")非法. (";
				else 
					cout << "<Error Output ID: FHIOQJS>\n";
			}
			
			// 输出类型信息
			cout << "类型:" << type;
			if(isSet(opt->extargs_type) || isRange(opt->extargs_type) || isDefualt(opt->extargs_type, opt->extargs_string_default))
				cout << ",";
			if(isSet(opt->extargs_type)) {
				cout << " 可取值";
				if(type == "int") 
					cout << generateList (opt->extargs_int_set);
				if(type == "double")
					cout << generateList (opt->extargs_double_set);
				if(type == "string")
					cout << generateList (opt->extargs_string_set);
			}
			if(isRange(opt->extargs_type)) {
				cout << " 范围";
				if (type == "int")
					cout << generateRange(opt->extargs_int_min, opt->extargs_int_max);
				if (type == "double")
					cout << generateRange(opt->extargs_double_min, opt->extargs_double_max);
			}
			if(isDefualt(opt->extargs_type, opt->extargs_string_default)) {
				cout << " 缺省:";
				if(type == "int")
					cout << opt->extargs_int_default;
				if(type == "double")
					cout << opt->extargs_double_default;
				if(type == "string")
					cout << opt->extargs_string_default;
				if(type == "IP地址") 
					cout << getStringIPAddr(opt->extargs_ipaddr_default);
			}
			cout << ")\n";
			return -1;
		}
		
		if(isSet(opt->extargs_type)) {
			if(!checkNotInSet(argv[retcnt + 1], type, setlist)) {
				if(type == "int") 
					opt->extargs_int_value = atoi(argv[retcnt + 1]);
				if(type == "double")
					opt->extargs_double_value = stod(string(argv[retcnt + 1]));
				if(type == "string") 
					opt->extargs_string_value = string(argv[retcnt + 1]);
			}
			else
				opt->giveInitValue();
		}
		if(isRange(opt->extargs_type)) {
			if(!checkNotInRange(argv[retcnt + 1], type, rangelist)) {
				if(type == "int") 
					opt->extargs_int_value = atoi(argv[retcnt + 1]);
				if(type == "double")
					opt->extargs_double_value = stod(string(argv[retcnt + 1]));
			}
			else
				opt->giveInitValue();
		}
		if(!isSet(opt->extargs_type) && !isRange(opt->extargs_type)) {
			if(type == "int") 
				opt->extargs_int_value = atoi(argv[retcnt + 1]);
			if(type == "double")
				opt->extargs_double_value = stod(string(argv[retcnt + 1]));
			if(type == "string") 
				opt->extargs_string_value = string(argv[retcnt + 1]);
			if(type == "IP地址") {
				opt->extargs_string_value = string(argv[retcnt + 1]);
				opt->extargs_ipaddr_value = getIntIPAddr(opt->extargs_string_value);
			}
		}
		++retcnt;
	}
	if(retcnt == argc || follow_up_args) {
		args_analyse_tools* p = args;
		while(p->get_name() != invalidVal <string> ()) {
			if(!p->existed())
				p->giveInitValue();
			p++;
		}
		return retcnt;
	}
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
	int wname = 4;
	int wtype = 4;
	int wdefault = 7;
	int wexists = 6;
	int wvalue = 5;
	int wrange = 9;
	const args_analyse_tools* p = args;
	while(p->get_name() != invalidVal <string> ()) { // 获取每个区段的长度
		wname = max(wname, p->get_name().length());
		wtype = max(wtype, TYPE_NAME[(int)p->extargs_type].length());
		string type = getType(p->extargs_type);
		if(isDefualt(p->extargs_type, p->extargs_string_default)) {
			if(type == "int")
				wdefault = max(wdefault, to_string(p->extargs_int_default).length());
			if(type == "double")
				wdefault = max(wdefault, to_string(p->extargs_double_default).length());
			if(type == "string" || type == "IP地址")
				wdefault = max(wdefault, p->extargs_string_default.length());
		}
		if(p->args_existed) {
			if(type == "int")
				wvalue = max(wvalue, to_string(p->extargs_int_value).length());
			if(type == "double")
				wvalue = max(wvalue, to_string(p->extargs_double_value).length());
			if(type == "string" || type == "IP地址")
				wvalue = max(wvalue, p->extargs_string_value.length());
			if(type == "bool")
				wvalue = max(wvalue, 4);
		}
		if(isSet(p->extargs_type)) {
			if(type == "int")
				wrange = max(wrange, generateList(p->extargs_int_set).length());
			if(type == "double")
				wrange = max(wrange, generateList(p->extargs_double_set).length());
			if(type == "string")
				wrange = max(wrange, generateList(p->extargs_string_set).length());
		}
		if(isRange(p->extargs_type)) {
			if(type == "int")
				wrange = max(wrange, generateRange(p->extargs_int_min, p->extargs_int_max).length());
			if(type == "double")
				wrange = max(wrange, generateRange(p->extargs_double_min, p->extargs_double_max).length());
		}
		p++;
	}
	int tot = wrange + wvalue + wname + wtype + wdefault + wexists + 7;
	cout << setw(tot) << setfill('=') << "=" << setfill(' ') << endl;;
	cout << left << ' ' << setw(wname) << "name" << ' ' << setw(wtype) << "type" << ' ' << setw(wdefault) << "default" << ' ' << setw(wexists) << "exists" << ' ' << setw(wvalue) << "value" << ' ' << setw(wrange) << "range/set" << endl;
	cout << setw(tot) << setfill('=') << "=" << setfill(' ') << endl;

	p = args;
	while(p->get_name() != invalidVal <string> ()) { // 输出每个区段
		cout << " " << setw(wname) << p->get_name();
		cout << " " << setw(wtype) << TYPE_NAME[(int)p->extargs_type];
		string type = getType(p->extargs_type);
		if(isDefualt(p->extargs_type, p->extargs_string_default)) {
			if(type == "int")
				cout << " " << setw(wdefault) << p->extargs_int_default;
			if(type == "double")
				cout << " " << setw(wdefault) << fixed << setprecision(6) << p->extargs_double_default;
			if(type == "string" || type == "IP地址")
				cout << " " << setw(wdefault) << p->extargs_string_default;
			if(type == "bool")
				cout << " " << setw(wdefault) << (p->extargs_bool_default ? "true" : "false");
		}
		else
			cout << " " << setw(wdefault) << "/";
		cout << " " << setw(wexists) << p->existed();
		if(p->args_existed) {
			if(type == "int")
				cout << " " << setw(wvalue) << p->extargs_int_value;
			if (type == "double")
				cout << " " << setw(wvalue) << p->extargs_double_value;
			if(type == "string" || type == "IP地址")
				cout << " " << setw(wvalue) << p->extargs_string_value;
			if(type == "bool")
				cout << " " << setw(wvalue) << "true";
		}
		else
			cout << " " << setw(wvalue) << "/";
		if(isSet(p->extargs_type)) {
			if(type == "int")
				cout << " " << setw(wrange) << generateList(p->extargs_int_set);
			if(type == "double")
				cout << " " << setw(wrange) << generateList(p->extargs_double_set);
			if(type == "string")
				cout << " " << setw(wrange) << generateList(p->extargs_string_set);
		}
		if(isRange(p->extargs_type)) {
			if(type == "int")
				cout << " " << setw(wrange) << generateRange(p->extargs_int_min, p->extargs_int_max);
			if(type == "double")
				cout << " " << setw(wrange) << generateRange(p->extargs_double_min, p->extargs_double_max);
		}
		if(!isRange(p->extargs_type) && !isSet(p->extargs_type))
			cout << " " << setw(wrange) << "/";
		cout << endl;
		p++;
	}

	cout << setw(tot) << setfill('=') << "=" << setfill(' ') << endl << endl;
	cout.unsetf(ios::fixed);
	return 0; //此句根据需要修改
}

#endif // !ENABLE_LIB_COMMON_TOOLS
