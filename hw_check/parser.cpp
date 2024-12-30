// ���������в���

#include "../include/class_aat.h"
#include "hw_check.h"
#include <iostream>
#include <iomanip>
#include <cstring>
using namespace std;

void usage(const char* const full_procname);

#define HELP 		(string("--help"))
#define DEBUG 		(string("--debug"))
#define ACTION 		(string("--action"))
#define CNO 		(string("--cno"))
#define STU 		(string("--stu"))
#define FILE 		(string("--file"))
#define CHAPTER 	(string("--chapter"))
#define WEEK 		(string("--week"))
#define DISPLAY 	(string("--display"))
#define CFGFILE 	(string("--cfgfile"))

// ��ȡ��Ӧ���ֵĲ���
static args_analyse_tools& getArgs(string argname, args_analyse_tools* args) 
{
	int i = 0;
	while(args[i].get_name() != argname) {
		if(args[i].get_name() == "")
			break;
		i++;
	}
	return args[i];
}

// ���display�����Ƿ���ȷ
static bool checkDisplay(args_analyse_tools* args, char* argv[]) 
{
	if(getArgs(DISPLAY, args).get_string().length() != 5) {
		usage(argv[0]);
		return 0;
	}
	for(char c: getArgs(DISPLAY, args).get_string()) {
		if(c != '0' && c != '1') {
			usage(argv[0]);
			return 0;
		}
	}
	return 1;
}

// ����Ҫ�����Ƿ����
static bool checkNecessary(args_analyse_tools* args, char* argv[]) 
{
	if(getArgs(ACTION, args).existed() == 0 || getArgs(CNO, args).existed() == 0 || getArgs(STU, args).existed() == 0 || getArgs(FILE, args).existed() == 0) {
		usage(argv[0]);
		return 0;
	}
	return 1;
}

// ���cno�����Ƿ���ȷ
static bool checkCno(args_analyse_tools* args, char* argv[], INFO& info) 
{
	static const string cno_set[] = {"10108001", "10108002", "5000244001602"};
	string cno = getArgs(CNO, args).get_string();
	if(cno == cno_set[0] || cno == cno_set[1] || cno == cno_set[2]) {
		info.cno.push_back(cno);
		return 1;
	}
	// ������ܴ��ո�Ķ��ŷָ���ʽ
	size_t pos = cno.find(",");
	if(pos != string::npos) {
		string cno1 = trim(cno.substr(0, pos));
		string cno2 = trim(cno.substr(pos + 1));
		if((cno1 == cno_set[0] && cno2 == cno_set[1]) || (cno1 == cno_set[1] && cno2 == cno_set[0])) {
			info.cno.push_back(cno1);
			info.cno.push_back(cno2);
			return 1;
		}
	}
	cout << "�κŴ���" << endl;
	return 0;
}

// ��������в����Ƿ���ȷ
int argvChecker(args_analyse_tools* args, char* argv[], INFO& info) 
{
	if(getArgs(HELP, args).existed() == 1) {
		usage(argv[0]);
		return 0;
	}
	if(checkNecessary(args, argv) == 0)
		return 0;
	if(checkDisplay(args, argv) == 0)
		return 0;
	if(checkCno(args, argv, info) == 0)
		return 0;
	if(info.cno.size() == 2 && getArgs(ACTION, args).get_string() != "secondline") {
		cout << "��secondlineģʽ��֧�������κ�" << endl;
		return 0;
	}
	if(getArgs(ACTION, args).get_string() == "secondline" && getArgs(FILE, args).get_string() == "all") {
		cout << "secondlineģʽ�½�֧�ֵ����ļ�" << endl;
		return 0;
	}
	if(getArgs(ACTION, args).get_string() == "secondline" && getArgs(STU, args).get_string() != "all") {
		cout << "secondlineģʽ�½�֧��ȫ��ѧ��" << endl;
		return 0;
	}
	if(getArgs(FILE, args).get_string() != "all" && (getArgs(CHAPTER, args).get_int() != -1 || getArgs(WEEK, args).get_int() != -1)) {
		cout << "����[--chapter/--week]���ܳ�����[--file �����ļ���]ʱ" << endl;
		return 0;
	}
	if(getArgs(ACTION, args).get_string() == "secondline" && getArgs(FILE, args).get_string() != "all") {
		string file = getArgs(FILE, args).get_string();
		string cno = info.cno[0];
		bool valid = 1;
		if(cno == "50002440016" && file == "5-b14.c")
			valid = 1;
		else if(cno == "101080" && (file == "15-b2.cpp" || file == "15-b5.c"))
			valid = 1;
		if(!valid) {
			cout << "secondlineģʽ���ļ�������" << endl;
			return 0;
		}
	}
	if(getArgs(ACTION, args).get_string() == "firstline") {
		string filestr = getFileSuffix(getArgs(FILE, args).get_string());
		if(filestr != "h" && filestr != "cpp" && filestr != "c" && filestr != "hpp" && filestr != "all") {
			cout << "���м����ļ�[" << getArgs(FILE, args).get_string() << "]������Դ�����ļ�" << endl;
			return 0;
		}
	}
	info.stu 		= getArgs(STU, args).get_string();
	info.file 		= getArgs(FILE, args).get_string();
	info.chapter 	= getArgs(CHAPTER, args).get_int();
	info.week 		= getArgs(WEEK, args).get_int();
	info.cfgfile 	= getArgs(CFGFILE, args).get_string();
	info.debug 		= getArgs(DEBUG, args).existed();
	info.origin_cno	= getArgs(CNO, args).get_string();
	bool *ptr = &info.printNormal;
	for(char c: getArgs(DISPLAY, args).get_string())
		*ptr++ = c - '0';
	if(getArgs(ACTION, args).get_string() == "base") 
		return 1;
	if(getArgs(ACTION, args).get_string() == "firstline")
		return 2;
	if(getArgs(ACTION, args).get_string() == "secondline")
		return 3;
	usage(argv[0]);
	return 0;
}

// ��ӡʹ�÷���
void usage(const char* const full_procname)
{
	const char *procname = strrchr(full_procname, '\\');
	if (procname == NULL)
		procname = full_procname;

	const int wkey = 7 + strlen(procname) + 1;
	const int wopt = 7 + strlen(procname) + 4;
	cout << endl;

	cout << "Usage: " << procname << " ��ѡ��/��ѡ��" << endl;
	cout << endl;
	cout << setw(wkey) << ' ' << "��ѡ�ָ������" << endl;
	cout << setw(wopt) << ' ' << "--action opname : ֧�ֵ�opname����" << endl;
	cout << setw(wopt) << ' ' << "\t base              : �������(�ļ��Ƿ��ύ���Ƿ񱻸Ķ��������Ƿ���ȷ)" << endl;
	cout << setw(wopt) << ' ' << "\t firstline         : ���м�飨��Դ�����ļ���Ҫ��" << endl;
	cout << setw(wopt) << ' ' << "\t secondline        : ���м�飨������Դ�����ļ���Ҫ��" << endl;

	cout << setw(wkey) << ' ' << "��ѡ�ָ���κ�" << endl;
	cout << setw(wopt) << ' ' << "--cno course_no     : �κ�" << endl;

	cout << setw(wkey) << ' ' << "��ѡ�ָ��ѧ��" << endl;
	cout << setw(wopt) << ' ' << "--stu no/all        : ָ������/ȫ��(7λ����Ϊѧ��,allΪȫ��)" << endl;

	cout << setw(wkey) << ' ' << "��ѡ�ָ���ļ�" << endl;
	cout << setw(wopt) << ' ' << "--file filename/all : �����ļ���/ȫ��(allΪȫ��,����Ϊ�����ļ���)" << endl;

	cout << setw(wkey) << ' ' << "��ѡ�" << endl;
	cout << setw(wopt) << ' ' << "--chapter n         : ��--file�Ļ������ٽ����½ڵ�ɸѡ(��/-1��ȫ���½�,����--week����)" << endl;
	cout << setw(wopt) << ' ' << "--week n            : ��--file�Ļ������ٽ����ܴε�ɸѡ(��/-1��ȫ���ܴ�,����--chapter����)" << endl;
	cout << setw(wopt) << ' ' << "--display xxxxx     : ÿλ0/1�ֱ��ʾ������Ϣ/δ�ύ��Ϣ/������Ϣ/������Ϣ/���ش�����Ϣ" << endl;
	cout << setw(wopt) << ' ' << "--cfgfile filename  : ָ�������ļ���(ȱʡ:hw_check.conf)" << endl;
	cout << endl;

	const char* DEMO_CNO = "10108001";
	const char* DEMO_SRC_FNAME = "12-b1.cpp";
	const char* DEMO_STUNO = "2359999";

	cout << "e.g.   " << procname << " --action base --cno " << DEMO_CNO << " --stu all --file all             : ���" << DEMO_CNO << "����ѧ����������ҵ�Ļ�����Ϣ" << endl;
	cout << "       " << procname << " --action base --cno " << DEMO_CNO << " --stu all --file all --chapter 4 : ���" << DEMO_CNO << "����ѧ���ĵ�4����ҵ�Ļ�����Ϣ" << endl;
	cout << "       " << procname << " --action base --cno " << DEMO_CNO << " --stu all --file all --week 6    : ���" << DEMO_CNO << "����ѧ���ĵ�6����ҵ�Ļ�����Ϣ" << endl;
	cout << "       " << procname << " --action base --cno " << DEMO_CNO << " --stu " << DEMO_STUNO << " --file all         : ���" << DEMO_CNO << "��" << DEMO_STUNO << "ѧ����������ҵ�Ļ�����Ϣ" << endl;
	cout << "       " << procname << " --action base --cno " << DEMO_CNO << " --stu " << DEMO_STUNO << " --file " << DEMO_SRC_FNAME << "   : ���" << DEMO_CNO << "��" << DEMO_STUNO << "ѧ����" << DEMO_SRC_FNAME << "��ҵ�Ļ�����Ϣ" << endl;
	cout << endl;

	cout << "       " << procname << " --action firstline --cno " << DEMO_CNO << " --stu all --file all             : ���" << DEMO_CNO << "����ѧ����������ҵ��������Ϣ" << endl;
	cout << "       " << procname << " --action firstline --cno " << DEMO_CNO << " --stu all --file all --chapter 4 : ���" << DEMO_CNO << "����ѧ���ĵ�4����ҵ��������Ϣ" << endl;
	cout << "       " << procname << " --action firstline --cno " << DEMO_CNO << " --stu all --file all --week 6    : ���" << DEMO_CNO << "����ѧ���ĵ�6����ҵ��������Ϣ" << endl;
	cout << "       " << procname << " --action firstline --cno " << DEMO_CNO << " --stu " << DEMO_STUNO << " --file all         : ���" << DEMO_CNO << "��" << DEMO_STUNO << "ѧ����������ҵ��������Ϣ" << endl;
	cout << "       " << procname << " --action firstline --cno " << DEMO_CNO << " --stu " << DEMO_STUNO << " --file " << DEMO_SRC_FNAME << "   : ���" << DEMO_CNO << "��" << DEMO_STUNO << "ѧ����" << DEMO_SRC_FNAME << "��ҵ��������Ϣ" << endl;
	cout << endl;

	cout << "       " << procname << " --action secondline --cno " << DEMO_CNO << " --stu all --file " << DEMO_SRC_FNAME << " : ���" << DEMO_CNO << "������ѧ����" << DEMO_SRC_FNAME << "��ҵ�Ĵ�����Ϣ" << endl;
	cout << endl;

	cout << "       " << procname << " --cfgfile E:\\test\\my.conf --action base --cno " << DEMO_CNO << " --stu all --file all : ���" << DEMO_CNO << "����ѧ����������ҵ�Ļ�����Ϣ(ָ�������ļ�)" << endl;
	cout << endl;


	cout << endl;
}