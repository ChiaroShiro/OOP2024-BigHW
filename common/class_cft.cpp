/* 2351871 ������ �ƿ� */ 

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
/* ����Լ���Ҫ��ͷ�ļ���ע������ */
#include "../include/class_cft.h"
#include <cctype>
#include <cstring>
#include <sstream>
using namespace std;

/*

̫ �����ļ�Ϊ�ı��ļ�, ֧�� Windows/Linux ��ʽ, �޶�ÿ��� 1024�ֽڣ������򱨴�

���ĳ�г���; ���//(��Ϊ���), ���ʾ�÷��ų���������β����Ϊע�� (����ɫ), ����Ҫ�����﷨Ҫ��, ��������Ч������
�� 1��ĳ������"\#xxxx yyyzzz"�������о�Ϊע����
�� 2��ĳ������"name=aa \#xxxx yyyzzz"����"name=aa "����Ч����

�����ļ���Ϊ������, ÿ���� [***]��ʾ����, ����������ͬ

ÿ���������ÿ��Ļ�����ʽ�� "��Ŀ��=ֵ"��ͬ�����Ŀ������ͬ����ͬ�������ͬ
- ÿ����Ŀһ��, ���������Ŀһ��
- =��Ϊ�ָ���, ���õ������ļ��ָ����� $=$ �Ϳո� (�� $t a b$ ) ����, Ϊ�˷��㣬�������� $=$ ��λ�þ��������Ϊ�ָ���
��Ŀ��Ϊ�ַ���, ��Ӣ��/����/����/���ŵ����, ��Ϊͼ���ַ� $=$ ��������ݳ�Ϊ��Ŀֵ, ��ʽΪһ��, �м�����ո�

��Ŀֵ�Ŀ���ȡֵ�У������������������ַ����ַ�����IP ��ַ�����ַ�������ΪӢ��/����/����/���ŵ�
�ַ������������ ";\#=[]" �����⺬��İ���ַ�
��Ŀ����ֵ��ǰ�������пո� tab ��, ����������, Ҳ���������������� [FaceTrack] ��Ϊ Title=����׷�٣�

ĳЩ�����ļ�������ֻ����Ŀ����û�������������г�Ϊ�������ļ���������/��Ŀ�������Ϊ���������ļ�������ļ�һ��ʼ������������ٸ����������Ϊ��������ļ�

����
- ����һ�е�ͳһ����˳��: ȡ��һ�к�, �Ƚض�;��\#��//��ʼ��ע�ͣ���ȥ��ǰ��ո�/tab��ʣ��Ϊ��Ч����
- ��Ч���ݵ�һ���ǣۣ����һ���ǣݣ�����Ϊ��������������
- �����ڲ�������ո񣬵�����ǰ��ո�

����ĳ��" [ abc ]def ] \# ����"��������= "abc ]def"
- ����=������ֱ�Ӻ��Լ��ɣ�����ȡ, Ҳ���ر���

���ļ������߼�:��
1���ļ�Ϊ�ı��ļ���֧��Windows/Linux ���ָ�ʽ
2��ÿ�к��н����������� 1024 �ֽ�, ����"�Ƿ���ʽ�������ļ�, �� [xx] �г�����󳤶� 1024. ",���ٽ��к�������
3�����һ���������н�����
���д����߼�����
1��ȡ��һ�к�, �Ƚض�;��\#��//��ʼ��ע��, ��ȥ��ǰ��ո�/tab, ʣ��Ϊ��Ч����
2����Ч����Ϊ����ֱ�Ӻ��Ը���
3����Ч���ݵĵ�һ���ַ��� [, ���һ���ַ���], ����Ϊ������, ������Ϊ����, ����Ϊ��Ч��
4������������������Ч�����в��ҷָ���������ж��������Ϊ�����һ���Ƿָ�����������ֵ��
5�������Էָ������ض�, ���������, �Ҳ�����ֵ, ����������ֵ����һ��ȥ��ǰ��ո�/ tab �Ĳ������ֱ��Ϊ��Ŀ����ֵ
6����ֵ�����ո�ת�������˫���ŵ�, Ϊ�˼�, ���������⴦��, ��������ͨ�ַ�
7������ֵ��ȡ�����������������ַ����ַ�����IP ��ַ�ȸ�����������ʱ, ͳһ�Ĵ���ʽΪ��ֵ���� istringstream ��, �١���ʽ��ȡ��һ����Чֵ, fail () Ϊ 0 ʱȡ�������ݿ���, ���򲻿���
8�����ĳ�е���Ч���ݷǿ�, ������, Ҳû�зָ���, ��Ҳ������, �����ܱ� get_item_all ��ԭʼ��ʽȫ����ȡʱ����ȡ
9��Լ��һ�������ļ�ֻ֧��һ�ַָ�����ʽ���ڳ�ʼ��ʱȷ��������������ָ������ɰ�ԭʼ���ݶ��������н��к�������

�� 1��ĳ��" [abc def] \#test" $\Rightarrow$ ����"[abc def]"
�� 2: ĳ��" name = ���� \#����"
���� $=>$ "name"
��ֵ $=>$ "����"���� cstring/string ��ȡ��"����"��char ȡ��������֣�int/double/ip �Ƿ�
�� 3��ĳ��" name = \" $\backslash \mathrm{t}$ ���� ���� $\$ "����"��Ĭ�Ϸָ���Ϊ=
���� $\Rightarrow>$ "name"
��ֵ $=>" \backslash$ " $\backslash$ ���� ���� \" ", �� cstring/string ��ȡ��" \" $\backslash \mathrm{t}$ ����", char ȡ��" \", ����Ƿ�
�� 4��ĳ��" name = \" $\backslash \mathrm{t}$ ���� ����\"\#����"��ָ���ָ���Ϊ�ո�
���� $=>$ "name"
��ֵ $=>"=\backslash " \backslash \mathrm{t}$ ���� ���� $\backslash "$ ", �� char/cstring/string ��ȡ��"=", ����Ƿ�
�� 5��ĳ��" name \"\t ���� ����\"\#����"��Ĭ�Ϸָ����� $=$ ��������޷�ƥ����������ֵ�����ܱ� get_item_all �����ȡ
��ע����ǰ��ĺ�ɫ"�����е���Ч���ݣ���Ϊ�˷���ֱ��Ƿ��пո�

�����ߺ������Ķ��塿
class cfgfile_read_tools �Ķ������ class_crt. h ��, ����Ա������˵������:
cfgfile_read_tools (const char* const cfgname,
const enum BREAK_CTYPE bctype = BREAK_CTYPE: :Equal)
ʹ��˵�������캯����ָ��Ҫ��ȡ�������ļ������ָ�������ʽ
- cfgname ָ�������ļ������ɴ�����·��/���·��
- �ָ�����=�Ϳո� (�� tab) ������ʽ, ����� class cft.h�еĶ���, Ĭ��Ϊ $=$
- ����һ�����غ���, ���� string ��ʽ���ļ���, ����ͬ
* ��cfgfile_read_tools����

ʹ��˵��: ��������, ���������Ҫ������

bool is_read_succeeded()
ʹ��˵�����ж������ļ��Ƿ�򿪳ɹ�
$\star$ int get_all_group(vector <string>\& ret)
ʹ��˵�������������ļ��е������飬���� vector ��
- �ɹ������������, ʧ�ܷ��� 0 (���� 0 ʱ vector Ϊ��)
- ����ж�� group ��ͬ��ȫ�����أ���ֻҪȡ������ $[* *]$ ���У��������Ƿ��ظ���
- ���ڼ������ļ�, ����һ����, ����Ϊ"", ����ֵΪ 1
- ���ڻ�������ļ�, ��һ������"", ����������������
int get_all_item(const char* const group_name, vector ��string��\& ret,
const bool is_case_sensitive = true)
ʹ��˵��: ���� group_name ���е�������, ���� vector ��
- is_case_sensitive Ϊ true ��ʾ group_name ��Сд����, false ��ʾ��Сд������, Ĭ��Ϊ false
- �ɹ����ظ������������, ���κ����򷵻� 0 ������ 0 ʱ vector Ϊ�գ�
- ���ص��Ǹ������Ч�е�ȫ�����ݵ������ַ�����ʽ��������ֵ���ͣ����������д���ȫ����Ч����ָȥ����Ŀ��֮ǰ/ֵ֮��Ŀո� tab �ȣ���ͬ�����������Ƿ񺬷ָ�����Ҳ���������Ƿ��ظ�
- ���ڼ������ļ���group_name ָ��Ϊ""�����ָ��Ϊ nullprt/NULL��ֱ�ӷ��� 0��
- ����һ�����غ�����group_name Ϊ string ���ͣ����β�Ϊ const string \&group_name��,����ͬ
int item_get_raw(const char* const group_name, const char* const item_name, string \&ret, const bool group_is_case_sensitive = true, const bool item_is_case_sensitive = true)
ʹ��˵�������� group_name ���е� item_name ������ݣ����� string ��
- group_ignore_lower_upper_case Ϊ true ��ʾ group_name ��Сд����, false ��ʾ��Сд�����У�Ĭ��Ϊ true����������ͬ�������ظ�˵����
- item_ignore_lower_upper_case Ϊ true ��ʾ item_name ��Сд����, false ��ʾ��Сд�����У�Ĭ��Ϊ true����������ͬ�������ظ�˵����
- group_name �Ĵ���ͬ get_all_item����������ͬ�������ظ�˵����
- �ɹ����� 1 , ʧ�ܷ��� 0 ������ 0 ʱ ret ��ֵ�����ţ�
- ������ʽ�� "��Ŀ�� $=$ ֵ" ����Ч�е�ȫ������
- ��� item ֵ��ͬ���ж���, ȡ��һ��
- ����һ�����غ���, group_name/item_name Ϊ string ����, ����ͬ
int item_get_null (const char* const group_name, const char* const item_name, const bool group_is_case_sensitive = true, const bool item_is_case_sensitive = true)
ʹ��˵�����ж� group_name ���� item_name ���Ƿ����
- ������ڷ��� 1 , �����ڷ��� 0
- ���жϸ����Ƿ���ڣ����Ƿ���ֵ��ֵ�ľ������ݲ�����

��: "name=" / "name=Y" / "name=����" / "name=123" ������ 1
- ����һ�����غ���, group_name/item_name Ϊ string ����, ����ͬ

int item_get_char (const char* const group_name,
const char* const item_name,
char \&value,
const char $*$ const choice_set $=$ nullptr,
const char def_value = DEFAULT_CHAR_VALUE,
const bool group_is_case_sensitive = true,
const bool item_is_case_sensitive = true)
ʹ��˵������ char ���Ͷ�ȡ group_name ���е� item_name �������
- ����ֵ���� value ��
- choice_set ָ���Ϸ���ȡֵ������"YyNn"��ʾֻ�д�Сд Y/N �ǺϷ���, Ĭ�� nullptr��ʾ�����ַ����Ϸ���
- def_value ��ʾ��ȡֵ���Ϸ��򲻴���ʱ, ����Ĭ��ֵ, ���������
- def_value �� DEFAULT_CHAR_VALUE, �򲻺Ϸ�/������ʱ���� 0
- def_value ���� DEFAULT_CHAR_VALUE, �򲻺Ϸ�/������ʱ�� def_value ������ 1
- ������ο� test_readcfg �еĲ�������
- ȡ���Ϸ�ֵ/Ĭ��ֵ���� 1 , ���򷵻� 0 ������ 0 ʱ value ��ֵ�����ţ�
- ����һ�����غ���, group_name/item_name Ϊ string ����, ����ͬ
int item_get_int (const char* const group_name,
const char* const item_name,
int \&value,
const int min_value = INT_MIN,
const int max_value = INT_MAX,
const int def_value = DEFAULT_INT_VALUE,
const bool group_is_case_sensitive = true,
const bool item_is_case_sensitive = true)
ʹ��˵������ int ���Ͷ�ȡ group_name ���е� item_name �������
- ����ֵ���� value ��
- [min_value..max_value]ָ���˺Ϸ�ȡֵ�ķ�Χ������[0?100]��ʾ�ɼ���Χ��Ĭ��Ϊȫ��int ֵ��
- def_value ��ʾ��ȡֵ���Ϸ��򲻴���ʱ, ����Ĭ��ֵ, ���������
- def_value �� DEFAULT_INT_VALUE, �򲻺Ϸ�/������ʱ���� 0
- def_value ���� DEFAULT_INT_VALUE, �򲻺Ϸ�/������ʱ�� def_value ������ 1
- ������ο� test_readcfg �еĲ�������
- ȡ���Ϸ�ֵ/Ĭ��ֵ���� 1 , ���򷵻� 0 ������ 0 ʱ value ��ֵ�����ţ�
- ����һ�����غ���, group_name/item_name Ϊ string ����, ����ͬ
int item_get_double(const char* const group_name,
const char* const item_name,
double \&value,
const int min_value = DBL_MIN,
const int max_value = DBL_MAX,
const int def_value = DEFAULT_DOUBLE_VALUE,
const bool group_is_case_sensitive = true,
const bool item_is_case_sensitive = true)
ʹ��˵������ double ���Ͷ�ȡ group_name ���е� item_name �������
- ����ֵ���� value ��
- [min_value. . max_value]ָ���˺Ϸ�ȡֵ�ķ�Χ������[3.8 $\left.{ }^{\sim} 4.3\right]$ ��ʾ��ѹ��Χ, Ĭ��Ϊȫ�� double ֵ��

- def_value ��ʾ��ȡֵ���Ϸ��򲻴���ʱ, ����Ĭ��ֵ, ���������
- def_value �� DEFAULT_DOUBLE_VALUE, �򲻺Ϸ�/������ʱ���� 0
- def_value ���� DEFAULT_DOUBLE_VALUE, �򲻺Ϸ�/������ʱ�� def_value ������ 1
- ������ο� test_readcfg �еĲ�������
- ȡ���Ϸ�ֵ/Ĭ��ֵ���� 1 �����򷵻� 0 ������ 0 ʱ value ��ֵ�����ţ�
- ����һ�����غ�����group_name/item_name Ϊ string ���ͣ�����ͬ

�� int item_get_cstring (const char* const group_name,
const char* const item_name,
char* const value,
const int str_maxlen,
const char* const def_value = DEFAULT_CSTRING_VALUE,
const bool group_is_case_sensitive = true,
const bool item_is_case_sensitive = true)
ʹ��˵������ char ����/ָ�����Ͷ�ȡ group_name ���е� item_name �������
- ����ֵ���� value ��
- ��������Ҫ��֤ value ���㹻�Ŀռ䣨char ������� char*���Ѷ�̬�����㹻�Ŀռ䣩�� str_maxlen ָ���˴���� char ����/ָ�뺬β���ܴ�ŵ�����ַ�������β�㣩��������� MAX_STRLEN����ʹ���Ŀռ��ٴ����Ҳ�Ͷ� MAX_STRLEN-1 ���ֽڣ�
- ע�⣺������������һ�����д���Ĳ��������
- def_value ��ʾ��ȡֵ���Ϸ��򲻴���ʱ, ����Ĭ��ֵ, ���������
- def_value �� DEFAULT_CSTRING_VALUE, �򲻺Ϸ�/������ʱ���� 0
- def_value ���� DEFAULT_CSTRING_VALUE, �򲻺Ϸ�/������ʱ�� def_value ������ 1
- ������ο� test_readcfg �еĲ�������
- ȡ���Ϸ�ֵ/Ĭ��ֵ���� 1 , ���򷵻� 0 ������ 0 ʱ value ��ֵ�����ţ�
- ����һ�����غ���, group_name/item_name Ϊ string ����, ����ͬ
? int item_get_string (const char* const group_name,
const char* const item_name,
string\& value,
const string\& def_value = DEFAULT_STRING_VALUE,
const bool group_is_case_sensitive = true,
const bool item_is_case_sensitive = true)
ʹ��˵������ string ���Ͷ�ȡ group_name ���е� item_name �������
- ����ֵ���� value ��
- def_value ��ʾ��ȡֵ���Ϸ��򲻴���ʱ������Ĭ��ֵ�����������
- def_value �� DEFAULT_STRING_VALUE, �򲻺Ϸ�/������ʱ���� 0
- def_value ���� DEFAULT_STRING_VALUE, �򲻺Ϸ�/������ʱ�� def_value ������ 1
- ������ο� test_readcfg �еĲ�������
- ȡ���Ϸ�ֵ/Ĭ��ֵ���� 1 , ���򷵻� 0 ������ 0 ʱ value ��ֵ�����ţ�
- ����һ�����غ�����group_name/item_name Ϊ string ���ͣ�����ͬ

? int item_get_ipaddr (const char* const group_name,
const char* const item_name,
unsigned int\& value,
const unsigned int\& def_value = DEFAULT_IPADDR_VALUE,
const bool group_is_case_sensitive = true,
const bool item_is_case_sensitive = true)
ʹ��˵������ IP ��ַ���Ͷ�ȡ group_name ���е� item_name �������

- ����ֵ���� value �� (�ǵ��ʮ���Ƶ����ͱ�ʾ, ����"1.2.3.4"��ʽ���ַ���)
- def_value ��ʾ��ȡֵ���Ϸ��򲻴���ʱ, ����Ĭ��ֵ, ���������
- def_value �� DEFAULT_IPADDR_VALUE, �򲻺Ϸ�/������ʱ���� 0
- def_value ���� DEFAULT_IPADDR_VALUE, �򲻺Ϸ�/������ʱ�� def_value ������ 1
- ������ο� test_readcfg �еĲ�������
- ȡ���Ϸ�ֵ/Ĭ��ֵ���� 1 , ���򷵻� 0 ������ 0 ʱ value ��ֵ�����ţ�
- ����һ�����غ���, group_name/item_name Ϊ string ����, ����ͬ

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

// ��string��ǰ��׺���ɼ��ַ�ȥ��
static void clearStringProSuf(string& str)
{
	str.erase(0, str.find_first_not_of(" \t\n\r\f\v"));
	str.erase(str.find_last_not_of(" \t\n\r\f\v") + 1);
}

/* ���������Զ��庯����ʵ�֣��Ѹ��������ݲ�ȫ�� */

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
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
config_file_tools::config_file_tools(const char* const _cfgname, const enum BREAK_CTYPE _ctype)
{
	read_cfg_file(_cfgname, _ctype);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
config_file_tools::config_file_tools(const string& _cfgname, const enum BREAK_CTYPE _ctype)
{
	read_cfg_file(_cfgname, _ctype);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
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
	for (const DATA_TYPE& group : data) {
		ret.push_back(group.first);
	}
	return ret.size();
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�����ָ�����������������ԭʼ����
  ���������const char* const group_name������
		   vector <string>& ret��vector ��ÿ��Ϊһ�����ԭʼ����
		   const bool is_case_sensitive = false : �����Ƿ��Сд���У�true-��Сд���� / Ĭ��false-��Сд������
  �� �� ֵ�����������0��ʾ��
  ˵    ����
***************************************************************************/
int config_file_tools::get_all_item(const char* const group_name, vector <string>& ret, const bool is_case_sensitive)
{
	/* ������ɣ�������Ҫ�ı�return��ֵ */
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
  �������ƣ�
  ��    �ܣ�����/��ĿΪstring��ʽ������ͬ��
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int config_file_tools::get_all_item(const string& group_name, vector <string>& ret, const bool is_case_sensitive)
{
	/* ������ɣ�������Ҫ�ı�return��ֵ */
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
	string (*group_transfer)(const string&) = group_is_case_sensitive ? transferNoChange : transferToLower;
	string (*item_transfer)(const string&) = item_is_case_sensitive ? transferNoChange : transferToLower;
	string group_name_str = string(group_name);
	string item_name_str = string(item_name);


	// ����������
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
  �������ƣ�
  ��    �ܣ�����/��ĿΪstring��ʽ������ͬ��
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int config_file_tools::item_get_raw(const string& group_name, const string& item_name, string& ret, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* ��������ʵ�� */
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
  �������ƣ�
  ��    �ܣ�����/��ĿΪstring��ʽ������ͬ��
  ���������
  �� �� ֵ��
  ˵    ������Ϊ���ߺ���һ���ڳ����ʼ���׶α����ã������ڳ���ִ���б���Ƶ�ε��ã�
		   �������ֱ���׿ǣ�����΢Ӱ��Ч�ʣ�����Ӱ���������ܣ��Ը�Ƶ�ε��ã��˷������ʺϣ�
***************************************************************************/
int config_file_tools::item_get_null(const string& group_name, const string& item_name, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* ��������ʵ�� */
	return this->item_get_null(group_name.c_str(), item_name.c_str(), group_is_case_sensitive, item_is_case_sensitive);
}

// ��ȡ��Ӧ��Ķ�Ӧ��Ŀ������Ŀֵ��������ҵ��򷵻�1�����򷵻�0
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

static int getDefaultValue(char* const value, const char* def_value) // ���char*���ػ�
{
    if (strcmp(value, def_value) == 0) {
        return 0;
    } else {
        strcpy(value, def_value);
        return 1;
    }
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
	if (getItemValue(group_name, item_name, item_value, group_is_case_sensitive, item_is_case_sensitive) == 0) { // û�ҵ�
		return getDefaultValue <char> (value, def_value);
	}

	if (readFromString(item_value, value) == 0) { // ��ȡʧ��
		return getDefaultValue <char> (value, def_value);
	}

	if (choice_set != nullptr && strchr(choice_set, value) == nullptr) { // �����ַ���֮��
		return getDefaultValue <char> (value, def_value);
	}

	return 1;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�����/��ĿΪstring��ʽ������ͬ��
  ���������
  �� �� ֵ��
  ˵    ������Ϊ���ߺ���һ���ڳ����ʼ���׶α����ã������ڳ���ִ���б���Ƶ�ε��ã�
		   �������ֱ���׿ǣ�����΢Ӱ��Ч�ʣ�����Ӱ���������ܣ��Ը�Ƶ�ε��ã��˷������ʺϣ�
***************************************************************************/
int config_file_tools::item_get_char(const string& group_name, const string& item_name, char& value,
						const char* const choice_set, const char def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* ��������ʵ�� */
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
	if (getItemValue(group_name, item_name, item_value, group_is_case_sensitive, item_is_case_sensitive) == 0) { // û�ҵ�
		return getDefaultValue <int> (value, def_value);
	}
	if (readFromString(item_value, value) == 0) { // ��ȡʧ��
		return getDefaultValue <int> (value, def_value);
	}
	if (value < min_value || value > max_value) { // ������Χ
		return getDefaultValue <int> (value, def_value);
	}
	return 1;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�����/��ĿΪstring��ʽ������ͬ��
  ���������
  �� �� ֵ��
  ˵    ������Ϊ���ߺ���һ���ڳ����ʼ���׶α����ã������ڳ���ִ���б���Ƶ�ε��ã�
		   �������ֱ���׿ǣ�����΢Ӱ��Ч�ʣ�����Ӱ���������ܣ��Ը�Ƶ�ε��ã��˷������ʺϣ�
***************************************************************************/
int config_file_tools::item_get_int(const string& group_name, const string& item_name, int& value,
						const int min_value, const int max_value, const int def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* ��������ʵ�� */
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
	if (getItemValue(group_name, item_name, item_value, group_is_case_sensitive, item_is_case_sensitive) == 0) { // û�ҵ�
		return getDefaultValue (value, def_value);
	}
	if (readFromString(item_value, value) == 0) { // ��ȡʧ��
		return getDefaultValue (value, def_value);
	}
	if (value < min_value - DOUBLE_EPS || value > max_value + DOUBLE_EPS) { // ������Χ
		return getDefaultValue (value, def_value);
	}
	return 1;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�����/��ĿΪstring��ʽ������ͬ��
  ���������
  �� �� ֵ��
  ˵    ������Ϊ���ߺ���һ���ڳ����ʼ���׶α����ã������ڳ���ִ���б���Ƶ�ε��ã�
		   �������ֱ���׿ǣ�����΢Ӱ��Ч�ʣ�����Ӱ���������ܣ��Ը�Ƶ�ε��ã��˷������ʺϣ�
***************************************************************************/
int config_file_tools::item_get_double(const string& group_name, const string& item_name, double& value,
						const double min_value, const double max_value, const double def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* ��������ʵ�� */
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
	if (getItemValue(group_name, item_name, item_value, group_is_case_sensitive, item_is_case_sensitive) == 0) { // û�ҵ�
		return getDefaultValue (value, def_value);
	}
	if (str_maxlen < 1) { // ���ȷǷ�
		value[0] = '\0';
		return 0;
	}

	int max_len = (str_maxlen > MAX_STRLEN) ? MAX_STRLEN : str_maxlen;
	if (int(item_value.length()) > max_len - 1) {
		item_value = item_value.substr(0, max_len - 1);
	}

	istringstream iss(item_value);
	if (!(iss >> value)) { // ��ȡʧ��
		return getDefaultValue(value, def_value);
	}

	return 1;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�����/��ĿΪstring��ʽ������ͬ��
  ���������
  �� �� ֵ��
  ˵    ������Ϊ���ߺ���һ���ڳ����ʼ���׶α����ã������ڳ���ִ���б���Ƶ�ε��ã�
		   �������ֱ���׿ǣ�����΢Ӱ��Ч�ʣ�����Ӱ���������ܣ��Ը�Ƶ�ε��ã��˷������ʺϣ�
***************************************************************************/
int config_file_tools::item_get_cstring(const string& group_name, const string& item_name, char* const value,
						const int str_maxlen, const char* const def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)

{
	/* ��������ʵ�� */
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
	if (getItemValue(group_name, item_name, item_value, group_is_case_sensitive, item_is_case_sensitive) == 0) { // û�ҵ�
		return getDefaultValue (value, def_value);
	}
	value = item_value;
	return 1;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�����/��ĿΪstring��ʽ������ͬ��
  ���������
  �� �� ֵ��
  ˵    ������Ϊ���ߺ���һ���ڳ����ʼ���׶α����ã������ڳ���ִ���б���Ƶ�ε��ã�
		   �������ֱ���׿ǣ�����΢Ӱ��Ч�ʣ�����Ӱ���������ܣ��Ը�Ƶ�ε��ã��˷������ʺϣ�
***************************************************************************/
int config_file_tools::item_get_string(const string& group_name, const string& item_name, string& value,
						const string& def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* ��������ʵ�� */
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
	if (getItemValue(group_name, item_name, item_value, group_is_case_sensitive, item_is_case_sensitive) == 0) { // û�ҵ�
		return getDefaultValue(value, def_value);
	}

	if (!checkIsIPAddr(item_value)) { // ���ǺϷ���IP��ַ
		return getDefaultValue(value, def_value);
	}

	value = getIntIPAddr(item_value);
	return 1;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�����/��ĿΪstring��ʽ������ͬ��
  ���������
  �� �� ֵ��
  ˵    ������Ϊ���ߺ���һ���ڳ����ʼ���׶α����ã������ڳ���ִ���б���Ƶ�ε��ã�
		   �������ֱ���׿ǣ�����΢Ӱ��Ч�ʣ�����Ӱ���������ܣ��Ը�Ƶ�ε��ã��˷������ʺϣ�
***************************************************************************/
int config_file_tools::item_get_ipaddr(const string& group_name, const string& item_name, unsigned int& value,
						const unsigned int& def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* ��������ʵ�� */
	return this->item_get_ipaddr(group_name.c_str(), item_name.c_str(), value, def_value, group_is_case_sensitive, item_is_case_sensitive);
}
