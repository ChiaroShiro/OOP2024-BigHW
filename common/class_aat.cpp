/* 2351871 ������ �ƿ�*/
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <cstring>
#include "../include/class_aat.h"
//���б�Ҫ�����Լ�������ͷ�ļ�
using namespace std;

#if !ENABLE_LIB_COMMON_TOOLS //��ʹ��lib����Ч

/* ---------------------------------------------------------------
	 �������������Ҫstatic�������ڲ������ã�
   ---------------------------------------------------------------- */

template <class _A, class _B>
static _A max(const _A &a, const _B &b)
{
	return a > (_A)b ? a : b;
}

template <class _T>
static _T invalidVal() // ���ض�Ӧ���͵�ĩβ��ֵ
{
	if (typeid(_T) == typeid(int))
		return (_T)(INVALID_INT_VALUE_OF_SET);
	else
		return _T();
}

template <>
static double invalidVal() // ����ģ��Ԫ����Ҫ�ػ�double
{
	return INVALID_DOUBLE_VALUE_OF_SET;
}

template <>
static string invalidVal() // ����ģ��Ԫ����Ҫ�ػ�string
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
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����null
 ***************************************************************************/
args_analyse_tools::args_analyse_tools()
{
	memset(this, 0, sizeof(*this));
	args_name = string("");
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����boolean
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
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����int_with_default��int_with_error
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
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����int_with_set_default��int_with_set_error
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
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����str��ipaddr_with_default��ipaddr_with_error
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
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����str_with_set_default��str_with_set_error
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
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����double_with_default��double_with_error
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
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����double_with_set_default��double_with_set_error
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
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
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
	 ����AAT���Զ����Ա������ʵ�֣�private��
   ---------------------------------------------------------------- */

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������ʵ�֣���Ҫ��
 ***************************************************************************/
const string args_analyse_tools::get_name() const
{
	return this->args_name;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������!!��ֻ����0/1
			��ʵ�֣���Ҫ��
 ***************************************************************************/
const int args_analyse_tools::existed() const
{
	return !!this->args_existed;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������ʵ�֣���Ҫ��
 ***************************************************************************/
const int args_analyse_tools::get_int() const
{
	return this->extargs_int_value;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������ʵ�֣���Ҫ��
 ***************************************************************************/
const double args_analyse_tools::get_double() const
{
	return this->extargs_double_value;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������ʵ�֣���Ҫ��
 ***************************************************************************/
const string args_analyse_tools::get_string() const
{
	return this->extargs_string_value;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������ʵ�֣���Ҫ��
 ***************************************************************************/
const unsigned int args_analyse_tools::get_ipaddr() const
{
	return this->extargs_ipaddr_value;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������ extargs_ipaddr_value ��ֵ�� 0x7f000001 תΪ "127.0.0.1"
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
		return "����";
	if(t == ST_EXTARGS_TYPE::int_with_default || t == ST_EXTARGS_TYPE::int_with_error || t == ST_EXTARGS_TYPE::int_with_set_default || t == ST_EXTARGS_TYPE::int_with_set_error)
		return "����";
	if(t == ST_EXTARGS_TYPE::double_with_default || t == ST_EXTARGS_TYPE::double_with_error || t == ST_EXTARGS_TYPE::double_with_set_default || t == ST_EXTARGS_TYPE::double_with_set_error)
		return "������";
	if(t == ST_EXTARGS_TYPE::str || t == ST_EXTARGS_TYPE::str_with_set_default || t == ST_EXTARGS_TYPE::str_with_set_error)
		return "�ַ���";
	if(t == ST_EXTARGS_TYPE::ipaddr_with_default || t == ST_EXTARGS_TYPE::ipaddr_with_error)
		return "IP��ַ";
	return "���ʹ���!";
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
		return "IP��ַ";
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
static string generateList(_T* ptr) // ��ӡ a/b/c/d ���ֶ���
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
	return flag1 || flag2; // ��һ��ʧ�ܻ�ڶ��γɹ����Ǵ���
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
	if(type == "IP��ַ") {
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
	if(retcnt + 1 >= argc) // ����û������
		return 1;
	if(checkIsCommand(arg.c_str())) // �����Ǹ�ָ��
		return 1;
	if(checkInputTypeError(arg, type)) // �������ʹ���
		return 1;
	if(isSet(opttype) && !isDefualt(opttype, strde) && checkNotInSet(arg, type, setlist)) // ���� set ��
		return 1;
	if(isRange(opttype) && !isDefualt(opttype, strde) && checkNotInRange(arg, type, rangelist)) // ���ڷ�Χ��
		return 1;
	return 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������follow_up_args���Ƿ��к�������
			0  ���޺�������
			1  ���к�������
  �� �� ֵ��
  ˵    ������Ԫ����
***************************************************************************/
int args_analyse_process(const int argc, const char* const *const argv, args_analyse_tools* const args, const int follow_up_args)
{
	int retcnt = 0;
	for(retcnt = 1; retcnt < argc; retcnt++) {
		if(checkIsCommand(argv[retcnt]) == 0) {
			cout << "����[" << argv[retcnt] << "]��ʽ�Ƿ�(����--��ͷ����Ч����)\n";
			return -1;
		}
		args_analyse_tools *opt = findOption(args, argv[retcnt]);
		if(opt == NULL) // �Ҳ����������
			break;
		opt->args_existed = 1;
		if(opt->extargs_num == 0)
			continue;
		void* setlist[] = {NULL, opt->extargs_int_set, opt->extargs_double_set, opt->extargs_string_set};
		void* rangelist[] = {NULL, &opt->extargs_int_min, &opt->extargs_double_min};
		
		string type = getType(opt->extargs_type);
		if(needPrintTypeInfo(retcnt, argc, argv[retcnt + 1], opt->extargs_type, setlist, rangelist, opt->extargs_string_default)) { // ��������ʱ������ֻ�������������Ԫ��֪����ʲô��������ʵ�ַ�ʽ��
			// ���������Ϣ
			if(retcnt + 1 >= argc) 
				cout << "����[" << argv[retcnt] << "]�ĸ��Ӳ�������. (";
			else if(checkIsCommand(argv[retcnt + 1]))
				cout << "����[" << argv[retcnt] << "]ȱ�ٸ��Ӳ���. (";
			else if(checkInputTypeError(argv[retcnt + 1], type)) {
				if(type == "IP��ַ" && !checkIsIPAddr(argv[retcnt + 1])) { // IP ��ַ����ֱ�ӷ��ؼ���
					cout << "����[--iperr]�ĸ��Ӳ���ֵ(1)�Ƿ�. (����:IP��ַ)";
					return -1;
				}
				cout << "����[" << argv[retcnt] << "]�ĸ��Ӳ�������" << getTypeInChinese(opt->extargs_type) << ". (";
			}
			else {
				if(type == "int") {
					int x = atoi(argv[retcnt + 1]);
					cout << "����[" << argv[retcnt] << "]�ĸ��Ӳ���ֵ(" << x << ")�Ƿ�. (";
				}
				else if(type == "double") {
					double x = stod(argv[retcnt + 1]);
					cout << "����[" << argv[retcnt] << "]�ĸ��Ӳ���ֵ(" << x << ")�Ƿ�. (";
				}
				else if(type == "string")
					cout << "����[" << argv[retcnt] << "]�ĸ��Ӳ���ֵ(" << argv[retcnt + 1] << ")�Ƿ�. (";
				else 
					cout << "<Error Output ID: FHIOQJS>\n";
			}
			
			// ���������Ϣ
			cout << "����:" << type;
			if(isSet(opt->extargs_type) || isRange(opt->extargs_type) || isDefualt(opt->extargs_type, opt->extargs_string_default))
				cout << ",";
			if(isSet(opt->extargs_type)) {
				cout << " ��ȡֵ";
				if(type == "int") 
					cout << generateList (opt->extargs_int_set);
				if(type == "double")
					cout << generateList (opt->extargs_double_set);
				if(type == "string")
					cout << generateList (opt->extargs_string_set);
			}
			if(isRange(opt->extargs_type)) {
				cout << " ��Χ";
				if (type == "int")
					cout << generateRange(opt->extargs_int_min, opt->extargs_int_max);
				if (type == "double")
					cout << generateRange(opt->extargs_double_min, opt->extargs_double_max);
			}
			if(isDefualt(opt->extargs_type, opt->extargs_string_default)) {
				cout << " ȱʡ:";
				if(type == "int")
					cout << opt->extargs_int_default;
				if(type == "double")
					cout << opt->extargs_double_default;
				if(type == "string")
					cout << opt->extargs_string_default;
				if(type == "IP��ַ") 
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
			if(type == "IP��ַ") {
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
	cout << "����[" << argv[retcnt] << "]�Ƿ�\n";
	return -1; //�˾������Ҫ�޸�
}


/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������Ԫ����
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
	while(p->get_name() != invalidVal <string> ()) { // ��ȡÿ�����εĳ���
		wname = max(wname, p->get_name().length());
		wtype = max(wtype, TYPE_NAME[(int)p->extargs_type].length());
		string type = getType(p->extargs_type);
		if(isDefualt(p->extargs_type, p->extargs_string_default)) {
			if(type == "int")
				wdefault = max(wdefault, to_string(p->extargs_int_default).length());
			if(type == "double")
				wdefault = max(wdefault, to_string(p->extargs_double_default).length());
			if(type == "string" || type == "IP��ַ")
				wdefault = max(wdefault, p->extargs_string_default.length());
		}
		if(p->args_existed) {
			if(type == "int")
				wvalue = max(wvalue, to_string(p->extargs_int_value).length());
			if(type == "double")
				wvalue = max(wvalue, to_string(p->extargs_double_value).length());
			if(type == "string" || type == "IP��ַ")
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
	while(p->get_name() != invalidVal <string> ()) { // ���ÿ������
		cout << " " << setw(wname) << p->get_name();
		cout << " " << setw(wtype) << TYPE_NAME[(int)p->extargs_type];
		string type = getType(p->extargs_type);
		if(isDefualt(p->extargs_type, p->extargs_string_default)) {
			if(type == "int")
				cout << " " << setw(wdefault) << p->extargs_int_default;
			if(type == "double")
				cout << " " << setw(wdefault) << fixed << setprecision(6) << p->extargs_double_default;
			if(type == "string" || type == "IP��ַ")
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
			if(type == "string" || type == "IP��ַ")
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
	return 0; //�˾������Ҫ�޸�
}

#endif // !ENABLE_LIB_COMMON_TOOLS
