/* 2351871 郎若谷 计科 */ 

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
/* 添加自己需要的头文件，注意限制 */
#include "../include/class_cft.h"
#include <cctype>
#include <cstring>
#include <sstream>
using namespace std;

/*

太 配置文件为文本文件, 支持 Windows/Linux 格式, 限定每行最长 1024字节，超过则报错

如果某行出现; 或或//(均为半角), 则表示该符号出现至本行尾部均为注释 (左侧红色), 不需要符合语法要求, 不计入有效内容中
例 1：某行内容"\#xxxx yyyzzz"，则整行均为注释行
例 2：某行内容"name=aa \#xxxx yyyzzz"，则"name=aa "是有效内容

配置文件分为若干组, 每组用 [***]表示组名, 组名各不相同

每组有若干项，每项的基本格式是 "项目名=值"，同组的项目名不相同，不同组可能相同
- 每个项目一行, 不允许多项目一行
- =称为分隔符, 常用的配置文件分隔符有 $=$ 和空格 (含 $t a b$ ) 两种, 为了方便，后续出现 $=$ 的位置均可以理解为分隔符
项目名为字符串, 由英文/中文/数字/符号等组成, 均为图形字符 $=$ 后面的内容称为项目值, 形式为一行, 中间允许空格

项目值的可能取值有：整数、浮点数、单字符、字符串、IP 地址、空字符串可能为英文/中文/数字/符号等
字符串不建议包含 ";\#=[]" 等特殊含义的半角字符
项目名及值的前后允许有空格、 tab 等, 不包含在内, 也不算错误（左侧例子中 [FaceTrack] 仍为 Title=人脸追踪）

某些配置文件，可能只有项目名，没有组名，下文中称为简单配置文件；有组名/项目名的则称为正常配置文件；如果文件一开始有若干配置项，再跟组名，则称为混合配置文件

其他
- 定义一行的统一处理顺序: 取出一行后, 先截断;或\#或//开始的注释，再去除前后空格/tab，剩下为有效内容
- 有效内容第一个是［，最后一个是］，就认为是组名，否则不是
- 组名内部允许带空格，但忽略前后空格

例：某行" [ abc ]def ] \# 测试"，则组名= "abc ]def"
- 不含=的项名直接忽略即可（不读取, 也不必报错）

【文件处理逻辑:】
1、文件为文本文件，支持Windows/Linux 两种格式
2、每行含行结束符不超过 1024 字节, 否则报"非法格式的配置文件, 第 [xx] 行超过最大长度 1024. ",不再进行后续操作
3、最后一行允许不含行结束符
【行处理逻辑：】
1、取出一行后, 先截断;或\#或//开始的注释, 再去除前后空格/tab, 剩下为有效内容
2、有效内容为空则直接忽略该行
3、有效内容的第一个字符是 [, 最后一个字符是], 则认为是组名, 否则认为是项, 均称为有效行
4、如果该行是项，则在有效内容中查找分隔符（如果有多个，则认为左起第一个是分隔符，其它是值）
5、将项以分隔符做截断, 左侧是项名, 右侧是项值, 将项名和项值再做一次去除前后空格/ tab 的操作，分别称为项目和项值
6、项值允许含空格、转义符、单双引号等, 为了简化, 均不做特殊处理, 都当做普通字符
7、从项值中取整数、浮点数、单字符、字符串、IP 地址等各种数据类型时, 统一的处理方式为将值放入 istringstream 中, 再〉方式提取第一个有效值, fail () 为 0 时取到的数据可信, 否则不可信
8、如果某行的有效内容非空, 不是组, 也没有分隔符, 则也当做项, 但仅能被 get_item_all 按原始方式全部读取时被读取
9、约定一个配置文件只支持一种分隔符方式，在初始化时确定，如果有其它分隔符，可按原始内容读出后自行进行后续处理

例 1：某行" [abc def] \#test" $\Rightarrow$ 组名"[abc def]"
例 2: 某行" name = 张三 \#姓名"
项名 $=>$ "name"
项值 $=>$ "张三"，按 cstring/string 均取到"张三"，char 取到半个汉字，int/double/ip 非法
例 3：某行" name = \" $\backslash \mathrm{t}$ 张三 李四 $\$ "姓名"，默认分隔符为=
项名 $\Rightarrow>$ "name"
项值 $=>" \backslash$ " $\backslash$ 张三 李四 \" ", 按 cstring/string 均取到" \" $\backslash \mathrm{t}$ 张三", char 取到" \", 其余非法
例 4：某行" name = \" $\backslash \mathrm{t}$ 张三 李四\"\#姓名"，指定分隔符为空格
项名 $=>$ "name"
项值 $=>"=\backslash " \backslash \mathrm{t}$ 张三 李四 $\backslash "$ ", 按 char/cstring/string 均取到"=", 其余非法
例 5：某行" name \"\t 张三 李四\"\#姓名"，默认分隔符是 $=$ ，则该行无法匹配项名和项值，仅能被 get_item_all 整体读取
【注】：前后的红色"不是行的有效内容，仅为了方便分辨是否有空格

【工具函数集的定义】
class cfgfile_read_tools 的定义放在 class_crt. h 中, 各成员函数的说明如下:
cfgfile_read_tools (const char* const cfgname,
const enum BREAK_CTYPE bctype = BREAK_CTYPE: :Equal)
使用说明：构造函数，指定要读取的配置文件名及分隔符的形式
- cfgname 指定配置文件名，可带绝对路径/相对路径
- 分隔符有=和空格 (含 tab) 两种形式, 具体见 class cft.h中的定义, 默认为 $=$
- 还有一个重载函数, 适用 string 形式的文件名, 其余同
* ～cfgfile_read_tools（）

使用说明: 析构函数, 按需放入需要的内容

bool is_read_succeeded()
使用说明：判断配置文件是否打开成功
$\star$ int get_all_group(vector <string>\& ret)
使用说明：返回配置文件中的所有组，放在 vector 中
- 成功返回组的数量, 失败返回 0 (返回 0 时 vector 为空)
- 如果有多个 group 相同则全部返回（即只要取到所有 $[* *]$ 的行，不考虑是否重复）
- 对于简单配置文件, 返回一个组, 组名为"", 返回值为 1
- 对于混合配置文件, 第一个组是"", 后续是有组名的组
int get_all_item(const char* const group_name, vector 〈string〉\& ret,
const bool is_case_sensitive = true)
使用说明: 返回 group_name 组中的所有项, 放在 vector 中
- is_case_sensitive 为 true 表示 group_name 大小写敏感, false 表示大小写不敏感, 默认为 false
- 成功返回该组中项的数量, 无任何项则返回 0 （返回 0 时 vector 为空）
- 返回的是该组的有效行的全部内容的完整字符串形式（不考虑值类型，供后续自行处理；全部有效内容指去除项目名之前/值之后的空格、 tab 等，下同），不考虑是否含分隔符，也不考虑项是否重复
- 对于简单配置文件，group_name 指定为""（如果指定为 nullprt/NULL则直接返回 0）
- 还有一个重载函数，group_name 为 string 类型（即形参为 const string \&group_name）,其余同
int item_get_raw(const char* const group_name, const char* const item_name, string \&ret, const bool group_is_case_sensitive = true, const bool item_is_case_sensitive = true)
使用说明：返回 group_name 组中的 item_name 项的内容，放在 string 中
- group_ignore_lower_upper_case 为 true 表示 group_name 大小写敏感, false 表示大小写不敏感，默认为 true（后续均相同，不再重复说明）
- item_ignore_lower_upper_case 为 true 表示 item_name 大小写敏感, false 表示大小写不敏感，默认为 true（后续均相同，不再重复说明）
- group_name 的处理同 get_all_item（后续均相同，不再重复说明）
- 成功返回 1 , 失败返回 0 （返回 0 时 ret 的值不可信）
- 返回形式是 "项目名 $=$ 值" 的有效行的全部内容
- 如果 item 值相同的有多项, 取第一项
- 还有一个重载函数, group_name/item_name 为 string 类型, 其余同
int item_get_null (const char* const group_name, const char* const item_name, const bool group_is_case_sensitive = true, const bool item_is_case_sensitive = true)
使用说明：判断 group_name 组中 item_name 项是否存在
- 该项存在返回 1 , 不存在返回 0
- 仅判断该项是否存在，对是否有值、值的具体内容不关心

例: "name=" / "name=Y" / "name=张三" / "name=123" 均返回 1
- 还有一个重载函数, group_name/item_name 为 string 类型, 其余同

int item_get_char (const char* const group_name,
const char* const item_name,
char \&value,
const char $*$ const choice_set $=$ nullptr,
const char def_value = DEFAULT_CHAR_VALUE,
const bool group_is_case_sensitive = true,
const bool item_is_case_sensitive = true)
使用说明：按 char 类型读取 group_name 组中的 item_name 项的内容
- 返回值放在 value 中
- choice_set 指定合法的取值（例："YyNn"表示只有大小写 Y/N 是合法的, 默认 nullptr表示任意字符均合法）
- def_value 表示当取值不合法或不存在时, 返回默认值, 有两种情况
- def_value 是 DEFAULT_CHAR_VALUE, 则不合法/不存在时返回 0
- def_value 不是 DEFAULT_CHAR_VALUE, 则不合法/不存在时置 def_value 并返回 1
- 具体请参考 test_readcfg 中的测试用例
- 取到合法值/默认值返回 1 , 否则返回 0 （返回 0 时 value 的值不可信）
- 还有一个重载函数, group_name/item_name 为 string 类型, 其余同
int item_get_int (const char* const group_name,
const char* const item_name,
int \&value,
const int min_value = INT_MIN,
const int max_value = INT_MAX,
const int def_value = DEFAULT_INT_VALUE,
const bool group_is_case_sensitive = true,
const bool item_is_case_sensitive = true)
使用说明：按 int 类型读取 group_name 组中的 item_name 项的内容
- 返回值放在 value 中
- [min_value..max_value]指定了合法取值的范围（例：[0?100]表示成绩范围，默认为全部int 值）
- def_value 表示当取值不合法或不存在时, 返回默认值, 有两种情况
- def_value 是 DEFAULT_INT_VALUE, 则不合法/不存在时返回 0
- def_value 不是 DEFAULT_INT_VALUE, 则不合法/不存在时置 def_value 并返回 1
- 具体请参考 test_readcfg 中的测试用例
- 取到合法值/默认值返回 1 , 否则返回 0 （返回 0 时 value 的值不可信）
- 还有一个重载函数, group_name/item_name 为 string 类型, 其余同
int item_get_double(const char* const group_name,
const char* const item_name,
double \&value,
const int min_value = DBL_MIN,
const int max_value = DBL_MAX,
const int def_value = DEFAULT_DOUBLE_VALUE,
const bool group_is_case_sensitive = true,
const bool item_is_case_sensitive = true)
使用说明：按 double 类型读取 group_name 组中的 item_name 项的内容
- 返回值放在 value 中
- [min_value. . max_value]指定了合法取值的范围（例：[3.8 $\left.{ }^{\sim} 4.3\right]$ 表示电压范围, 默认为全部 double 值）

- def_value 表示当取值不合法或不存在时, 返回默认值, 有两种情况
- def_value 是 DEFAULT_DOUBLE_VALUE, 则不合法/不存在时返回 0
- def_value 不是 DEFAULT_DOUBLE_VALUE, 则不合法/不存在时置 def_value 并返回 1
- 具体请参考 test_readcfg 中的测试用例
- 取到合法值/默认值返回 1 ，否则返回 0 （返回 0 时 value 的值不可信）
- 还有一个重载函数，group_name/item_name 为 string 类型，其余同

丸 int item_get_cstring (const char* const group_name,
const char* const item_name,
char* const value,
const int str_maxlen,
const char* const def_value = DEFAULT_CSTRING_VALUE,
const bool group_is_case_sensitive = true,
const bool item_is_case_sensitive = true)
使用说明：按 char 数组/指针类型读取 group_name 组中的 item_name 项的内容
- 返回值放在 value 中
- 调用者需要保证 value 有足够的空间（char 数组或者 char*但已动态申请足够的空间）， str_maxlen 指定了传入的 char 数组/指针含尾零能存放的最大字符数（含尾零），最长不超过 MAX_STRLEN（即使给的空间再大，最多也就读 MAX_STRLEN-1 个字节）
- 注意：测试用例中有一个运行错误的测试项！！！
- def_value 表示当取值不合法或不存在时, 返回默认值, 有两种情况
- def_value 是 DEFAULT_CSTRING_VALUE, 则不合法/不存在时返回 0
- def_value 不是 DEFAULT_CSTRING_VALUE, 则不合法/不存在时置 def_value 并返回 1
- 具体请参考 test_readcfg 中的测试用例
- 取到合法值/默认值返回 1 , 否则返回 0 （返回 0 时 value 的值不可信）
- 还有一个重载函数, group_name/item_name 为 string 类型, 其余同
? int item_get_string (const char* const group_name,
const char* const item_name,
string\& value,
const string\& def_value = DEFAULT_STRING_VALUE,
const bool group_is_case_sensitive = true,
const bool item_is_case_sensitive = true)
使用说明：按 string 类型读取 group_name 组中的 item_name 项的内容
- 返回值放在 value 中
- def_value 表示当取值不合法或不存在时，返回默认值，有两种情况
- def_value 是 DEFAULT_STRING_VALUE, 则不合法/不存在时返回 0
- def_value 不是 DEFAULT_STRING_VALUE, 则不合法/不存在时置 def_value 并返回 1
- 具体请参考 test_readcfg 中的测试用例
- 取到合法值/默认值返回 1 , 否则返回 0 （返回 0 时 value 的值不可信）
- 还有一个重载函数，group_name/item_name 为 string 类型，其余同

? int item_get_ipaddr (const char* const group_name,
const char* const item_name,
unsigned int\& value,
const unsigned int\& def_value = DEFAULT_IPADDR_VALUE,
const bool group_is_case_sensitive = true,
const bool item_is_case_sensitive = true)
使用说明：按 IP 地址类型读取 group_name 组中的 item_name 项的内容

- 返回值放在 value 中 (是点分十进制的整型表示, 不是"1.2.3.4"形式的字符串)
- def_value 表示当取值不合法或不存在时, 返回默认值, 有两种情况
- def_value 是 DEFAULT_IPADDR_VALUE, 则不合法/不存在时返回 0
- def_value 不是 DEFAULT_IPADDR_VALUE, 则不合法/不存在时置 def_value 并返回 1
- 具体请参考 test_readcfg 中的测试用例
- 取到合法值/默认值返回 1 , 否则返回 0 （返回 0 时 value 的值不可信）
- 还有一个重载函数, group_name/item_name 为 string 类型, 其余同

*/

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

/* 给出各种自定义函数的实现（已给出的内容不全） */

void config_file_tools::read_cfg_file(const string& _cfgname, const enum BREAK_CTYPE _ctype)
{
	cfgname = _cfgname;
	item_separate_character_type = _ctype;
	is_read_succeeded_value = false;
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
	while (fs) {
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

	string current_group;
	bool noGroupInit = true;

	for (const string& line : lines) {
		if (line.front() == '[' && line.back() == ']') {
			noGroupInit = false;
			current_group = line;
			data.push_back(make_pair(current_group, vector<pair<string, string>>()));
			lineraw.push_back(make_pair(current_group, vector<string>()));
		} else {
			if (noGroupInit) {
				current_group = SIMPLE_GNAME;
				noGroupInit = false;
				data.push_back(make_pair(current_group, vector<pair<string, string>>()));
				lineraw.push_back(make_pair(current_group, vector<string>()));
			}
			lineraw.back().second.push_back(line);
			size_t separator_pos = line.find(getSeparator(item_separate_character_type));
			if (separator_pos != string::npos) {
				string item_name = line.substr(0, separator_pos);
				string item_value = line.substr(separator_pos + 1);
				clearStringProSuf(item_name);
				clearStringProSuf(item_value);
				data.back().second.push_back(make_pair(item_name, item_value));
			}
		}
	}
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
config_file_tools::config_file_tools(const char* const _cfgname, const enum BREAK_CTYPE _ctype)
{
	read_cfg_file(_cfgname, _ctype);
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
config_file_tools::config_file_tools(const string& _cfgname, const enum BREAK_CTYPE _ctype)
{
	read_cfg_file(_cfgname, _ctype);
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
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
	for (const DATA_TYPE& group : data) {
		ret.push_back(group.first);
	}
	return ret.size();
}

/***************************************************************************
  函数名称：
  功    能：查找指定组的所有项并返回项的原始内容
  输入参数：const char* const group_name：组名
		   vector <string>& ret：vector 中每项为一个项的原始内容
		   const bool is_case_sensitive = false : 组名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
  返 回 值：项的数量，0表示空
  说    明：
***************************************************************************/
int config_file_tools::get_all_item(const char* const group_name, vector <string>& ret, const bool is_case_sensitive)
{
	/* 按需完成，根据需要改变return的值 */
	return get_all_item(string(group_name), ret, is_case_sensitive);
}

static string transferNoChange(const string& str)
{
	return str;
}

static string transferToLower(const string& str)
{
	string ret = str;
	for (char& ch : ret)
		ch = tolower(ch);
	return ret;
}

/***************************************************************************
  函数名称：
  功    能：组名/项目为string方式，其余同上
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int config_file_tools::get_all_item(const string& group_name, vector <string>& ret, const bool is_case_sensitive)
{
	/* 按需完成，根据需要改变return的值 */
	string (*transfer)(const string&) = is_case_sensitive ? transferNoChange : transferToLower;
	for (const LINERAW_TYPE& group : lineraw) {
		if (transfer(group.first) == transfer(group_name)) {
			for (const string& item : group.second) {
				ret.push_back(item);
			}
		}
	}
	return ret.size();
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
	string (*group_transfer)(const string&) = group_is_case_sensitive ? transferNoChange : transferToLower;
	string (*item_transfer)(const string&) = item_is_case_sensitive ? transferNoChange : transferToLower;
	string group_name_str = string(group_name);
	string item_name_str = string(item_name);


	// 遍历所有组
	/*for (const LINERAW_TYPE& group : lineraw) {
		cout << "  finding: group: " << group.first << endl;
		if (group_transfer(group.first) == group_transfer(group_name_str)) {
			cout << "  finding: group found" << endl;
			for (const string& item : group.second) {
				cout << "  finding: item: " << item << endl;*/
	// 			if (item_transfer(item) == item_transfer(item_name_str)) {
	// 				ret = item;
	// 				return 1;
	// 			}
	// 		}
	// 	}
	// }
	return 0;
}

/***************************************************************************
  函数名称：
  功    能：组名/项目为string方式，其余同上
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int config_file_tools::item_get_raw(const string& group_name, const string& item_name, string& ret, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* 本函数已实现 */
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
	string (*group_transfer)(const string&) = group_is_case_sensitive ? transferNoChange : transferToLower;
	string (*item_transfer)(const string&) = item_is_case_sensitive ? transferNoChange : transferToLower;
	string group_name_str = string(group_name);
	string item_name_str = string(item_name);
	for (const LINERAW_TYPE& group : lineraw) {
		if (group_transfer(group.first) == group_transfer(group_name_str)) {
			for (const string& item : group.second) {
				if (item_transfer(item) == item_transfer(item_name_str)) {
					return 1;
				}
			}
		}
	}
	return 0;
}

/***************************************************************************
  函数名称：
  功    能：组名/项目为string方式，其余同上
  输入参数：
  返 回 值：
  说    明：因为工具函数一般在程序初始化阶段被调用，不会在程序执行中被高频次调用，
		   因此这里直接套壳，会略微影响效率，但不影响整体性能（对高频次调用，此方法不适合）
***************************************************************************/
int config_file_tools::item_get_null(const string& group_name, const string& item_name, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* 本函数已实现 */
	return this->item_get_null(group_name.c_str(), item_name.c_str(), group_is_case_sensitive, item_is_case_sensitive);
}

// 获取对应组的对应项目名的项目值，如果能找到则返回1，否则返回0
int config_file_tools::getItemValue(const string& group_name, const string& item_name, string& val, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	string (*group_transfer)(const string&) = group_is_case_sensitive ? transferNoChange : transferToLower;
	string (*item_transfer)(const string&) = item_is_case_sensitive ? transferNoChange : transferToLower;
	string group_name_str = group_transfer(group_name);
	string item_name_str = item_transfer(item_name);
	for (const DATA_TYPE& group : data) {
		if (group_transfer(group.first) == group_name_str) {
			for (const ITEM_TYPE& item : group.second) {
				if (item_transfer(item.first) == item_name_str) {
					val = item.second;
					return 1;
				}
			}
		}
	}
	return 0;
}

template <typename T>
static T readFromString(const string& str, T& value)
{
    istringstream iss(str);
    if (!(iss >> value))
        return 0;
    return 1;
}

template <typename T>
static int getDefaultValue(T& value, const T& def_value)
{
	if (value == def_value) {
		return 0;
	} else {
		value = def_value;
		return 1;
	}
}

static int getDefaultValue(char* const value, const char* def_value) // 针对char*的特化
{
    if (strcmp(value, def_value) == 0) {
        return 0;
    } else {
        strcpy(value, def_value);
        return 1;
    }
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
	if (getItemValue(group_name, item_name, item_value, group_is_case_sensitive, item_is_case_sensitive) == 0) { // 没找到
		return getDefaultValue <char> (value, def_value);
	}

	if (readFromString(item_value, value) == 0) { // 读取失败
		return getDefaultValue <char> (value, def_value);
	}

	if (choice_set != nullptr && strchr(choice_set, value) == nullptr) { // 不在字符集之内
		return getDefaultValue <char> (value, def_value);
	}

	return 1;
}

/***************************************************************************
  函数名称：
  功    能：组名/项目为string方式，其余同上
  输入参数：
  返 回 值：
  说    明：因为工具函数一般在程序初始化阶段被调用，不会在程序执行中被高频次调用，
		   因此这里直接套壳，会略微影响效率，但不影响整体性能（对高频次调用，此方法不适合）
***************************************************************************/
int config_file_tools::item_get_char(const string& group_name, const string& item_name, char& value,
						const char* const choice_set, const char def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* 本函数已实现 */
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
	if (getItemValue(group_name, item_name, item_value, group_is_case_sensitive, item_is_case_sensitive) == 0) { // 没找到
		return getDefaultValue <int> (value, def_value);
	}
	if (readFromString(item_value, value) == 0) { // 读取失败
		return getDefaultValue <int> (value, def_value);
	}
	if (value < min_value || value > max_value) { // 超出范围
		return getDefaultValue <int> (value, def_value);
	}
	return 1;
}

/***************************************************************************
  函数名称：
  功    能：组名/项目为string方式，其余同上
  输入参数：
  返 回 值：
  说    明：因为工具函数一般在程序初始化阶段被调用，不会在程序执行中被高频次调用，
		   因此这里直接套壳，会略微影响效率，但不影响整体性能（对高频次调用，此方法不适合）
***************************************************************************/
int config_file_tools::item_get_int(const string& group_name, const string& item_name, int& value,
						const int min_value, const int max_value, const int def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* 本函数已实现 */
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
	if (getItemValue(group_name, item_name, item_value, group_is_case_sensitive, item_is_case_sensitive) == 0) { // 没找到
		return getDefaultValue (value, def_value);
	}
	if (readFromString(item_value, value) == 0) { // 读取失败
		return getDefaultValue (value, def_value);
	}
	if (value < min_value - DOUBLE_EPS || value > max_value + DOUBLE_EPS) { // 超出范围
		return getDefaultValue (value, def_value);
	}
	return 1;
}

/***************************************************************************
  函数名称：
  功    能：组名/项目为string方式，其余同上
  输入参数：
  返 回 值：
  说    明：因为工具函数一般在程序初始化阶段被调用，不会在程序执行中被高频次调用，
		   因此这里直接套壳，会略微影响效率，但不影响整体性能（对高频次调用，此方法不适合）
***************************************************************************/
int config_file_tools::item_get_double(const string& group_name, const string& item_name, double& value,
						const double min_value, const double max_value, const double def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* 本函数已实现 */
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
	if (getItemValue(group_name, item_name, item_value, group_is_case_sensitive, item_is_case_sensitive) == 0) { // 没找到
		return getDefaultValue (value, def_value);
	}
	if (str_maxlen < 1) { // 长度非法
		value[0] = '\0';
		return 0;
	}

	int max_len = (str_maxlen > MAX_STRLEN) ? MAX_STRLEN : str_maxlen;
	if (int(item_value.length()) > max_len - 1) {
		item_value = item_value.substr(0, max_len - 1);
	}

	istringstream iss(item_value);
	if (!(iss >> value)) { // 读取失败
		return getDefaultValue(value, def_value);
	}

	return 1;
}

/***************************************************************************
  函数名称：
  功    能：组名/项目为string方式，其余同上
  输入参数：
  返 回 值：
  说    明：因为工具函数一般在程序初始化阶段被调用，不会在程序执行中被高频次调用，
		   因此这里直接套壳，会略微影响效率，但不影响整体性能（对高频次调用，此方法不适合）
***************************************************************************/
int config_file_tools::item_get_cstring(const string& group_name, const string& item_name, char* const value,
						const int str_maxlen, const char* const def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)

{
	/* 本函数已实现 */
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
	if (getItemValue(group_name, item_name, item_value, group_is_case_sensitive, item_is_case_sensitive) == 0) { // 没找到
		return getDefaultValue (value, def_value);
	}
	value = item_value;
	return 1;
}

/***************************************************************************
  函数名称：
  功    能：组名/项目为string方式，其余同上
  输入参数：
  返 回 值：
  说    明：因为工具函数一般在程序初始化阶段被调用，不会在程序执行中被高频次调用，
		   因此这里直接套壳，会略微影响效率，但不影响整体性能（对高频次调用，此方法不适合）
***************************************************************************/
int config_file_tools::item_get_string(const string& group_name, const string& item_name, string& value,
						const string& def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* 本函数已实现 */
	return this->item_get_string(group_name.c_str(), item_name.c_str(), value, def_value, group_is_case_sensitive, item_is_case_sensitive);
}

typedef unsigned int u_int;

static bool checkIsIPAddr(string s)
{
	int cnt = 0;
	int pos[5] = {0};
	for(u_int i = 0; i < s.size(); i++) {
		if(s[i] == '.') {
			pos[++cnt] = i;
			if(cnt > 3)
				return 0;
		}
	}
	if(cnt != 3)
		return 0;
	pos[0] = -1;
	pos[4] = s.size() + 1;
	string subs[4];
	for(int i = 0; i < 4; i++) {
		subs[i] = s.substr(pos[i] + 1, pos[i + 1] - pos[i] - 1);
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
	if (getItemValue(group_name, item_name, item_value, group_is_case_sensitive, item_is_case_sensitive) == 0) { // 没找到
		return getDefaultValue(value, def_value);
	}

	if (!checkIsIPAddr(item_value)) { // 不是合法的IP地址
		return getDefaultValue(value, def_value);
	}

	value = getIntIPAddr(item_value);
	return 1;
}

/***************************************************************************
  函数名称：
  功    能：组名/项目为string方式，其余同上
  输入参数：
  返 回 值：
  说    明：因为工具函数一般在程序初始化阶段被调用，不会在程序执行中被高频次调用，
		   因此这里直接套壳，会略微影响效率，但不影响整体性能（对高频次调用，此方法不适合）
***************************************************************************/
int config_file_tools::item_get_ipaddr(const string& group_name, const string& item_name, unsigned int& value,
						const unsigned int& def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* 本函数已实现 */
	return this->item_get_ipaddr(group_name.c_str(), item_name.c_str(), value, def_value, group_is_case_sensitive, item_is_case_sensitive);
}
