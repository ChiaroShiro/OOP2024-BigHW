// 解析命令行参数

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

// 获取对应名字的参数
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

// 检查display参数是否正确
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

// 检查必要参数是否存在
static bool checkNecessary(args_analyse_tools* args, char* argv[]) 
{
	if(getArgs(ACTION, args).existed() == 0 || getArgs(CNO, args).existed() == 0 || getArgs(STU, args).existed() == 0 || getArgs(FILE, args).existed() == 0) {
		usage(argv[0]);
		return 0;
	}
	return 1;
}

// 检查cno参数是否正确
static bool checkCno(args_analyse_tools* args, char* argv[], INFO& info) 
{
	static const string cno_set[] = {"10108001", "10108002", "5000244001602"};
	string cno = getArgs(CNO, args).get_string();
	if(cno == cno_set[0] || cno == cno_set[1] || cno == cno_set[2]) {
		info.cno.push_back(cno);
		return 1;
	}
	// 处理可能带空格的逗号分隔形式
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
	cout << "课号错误" << endl;
	return 0;
}

// 检查命令行参数是否正确
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
		cout << "仅secondline模式下支持两个课号" << endl;
		return 0;
	}
	if(getArgs(ACTION, args).get_string() == "secondline" && getArgs(FILE, args).get_string() == "all") {
		cout << "secondline模式下仅支持单个文件" << endl;
		return 0;
	}
	if(getArgs(ACTION, args).get_string() == "secondline" && getArgs(STU, args).get_string() != "all") {
		cout << "secondline模式下仅支持全体学生" << endl;
		return 0;
	}
	if(getArgs(FILE, args).get_string() != "all" && (getArgs(CHAPTER, args).get_int() != -1 || getArgs(WEEK, args).get_int() != -1)) {
		cout << "参数[--chapter/--week]不能出现在[--file 单个文件名]时" << endl;
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
			cout << "secondline模式下文件名错误" << endl;
			return 0;
		}
	}
	if(getArgs(ACTION, args).get_string() == "firstline") {
		string filestr = getFileSuffix(getArgs(FILE, args).get_string());
		if(filestr != "h" && filestr != "cpp" && filestr != "c" && filestr != "hpp" && filestr != "all") {
			cout << "首行检查的文件[" << getArgs(FILE, args).get_string() << "]必须是源程序文件" << endl;
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

// 打印使用方法
void usage(const char* const full_procname)
{
	const char *procname = strrchr(full_procname, '\\');
	if (procname == NULL)
		procname = full_procname;

	const int wkey = 7 + strlen(procname) + 1;
	const int wopt = 7 + strlen(procname) + 4;
	cout << endl;

	cout << "Usage: " << procname << " 必选项/可选项" << endl;
	cout << endl;
	cout << setw(wkey) << ' ' << "必选项：指定操作" << endl;
	cout << setw(wopt) << ' ' << "--action opname : 支持的opname如下" << endl;
	cout << setw(wopt) << ' ' << "\t base              : 基础检查(文件是否提交、是否被改动、编码是否正确)" << endl;
	cout << setw(wopt) << ' ' << "\t firstline         : 首行检查（仅源程序文件需要）" << endl;
	cout << setw(wopt) << ' ' << "\t secondline        : 次行检查（仅部分源程序文件需要）" << endl;

	cout << setw(wkey) << ' ' << "必选项：指定课号" << endl;
	cout << setw(wopt) << ' ' << "--cno course_no     : 课号" << endl;

	cout << setw(wkey) << ' ' << "必选项：指定学生" << endl;
	cout << setw(wopt) << ' ' << "--stu no/all        : 指定单个/全部(7位数字为学号,all为全部)" << endl;

	cout << setw(wkey) << ' ' << "必选项：指定文件" << endl;
	cout << setw(wopt) << ' ' << "--file filename/all : 具体文件名/全部(all为全部,其余为具体文件名)" << endl;

	cout << setw(wkey) << ' ' << "可选项：" << endl;
	cout << setw(wopt) << ' ' << "--chapter n         : 在--file的基础上再进行章节的筛选(无/-1则全部章节,可与--week共存)" << endl;
	cout << setw(wopt) << ' ' << "--week n            : 在--file的基础上再进行周次的筛选(无/-1则全部周次,可与--chapter共存)" << endl;
	cout << setw(wopt) << ' ' << "--display xxxxx     : 每位0/1分别表示正常信息/未提交信息/错误信息/汇总信息/严重错误信息" << endl;
	cout << setw(wopt) << ' ' << "--cfgfile filename  : 指定配置文件名(缺省:hw_check.conf)" << endl;
	cout << endl;

	const char* DEMO_CNO = "10108001";
	const char* DEMO_SRC_FNAME = "12-b1.cpp";
	const char* DEMO_STUNO = "2359999";

	cout << "e.g.   " << procname << " --action base --cno " << DEMO_CNO << " --stu all --file all             : 检查" << DEMO_CNO << "所有学生的所有作业的基本信息" << endl;
	cout << "       " << procname << " --action base --cno " << DEMO_CNO << " --stu all --file all --chapter 4 : 检查" << DEMO_CNO << "所有学生的第4章作业的基本信息" << endl;
	cout << "       " << procname << " --action base --cno " << DEMO_CNO << " --stu all --file all --week 6    : 检查" << DEMO_CNO << "所有学生的第6周作业的基本信息" << endl;
	cout << "       " << procname << " --action base --cno " << DEMO_CNO << " --stu " << DEMO_STUNO << " --file all         : 检查" << DEMO_CNO << "的" << DEMO_STUNO << "学生的所有作业的基本信息" << endl;
	cout << "       " << procname << " --action base --cno " << DEMO_CNO << " --stu " << DEMO_STUNO << " --file " << DEMO_SRC_FNAME << "   : 检查" << DEMO_CNO << "的" << DEMO_STUNO << "学生的" << DEMO_SRC_FNAME << "作业的基本信息" << endl;
	cout << endl;

	cout << "       " << procname << " --action firstline --cno " << DEMO_CNO << " --stu all --file all             : 检查" << DEMO_CNO << "所有学生的所有作业的首行信息" << endl;
	cout << "       " << procname << " --action firstline --cno " << DEMO_CNO << " --stu all --file all --chapter 4 : 检查" << DEMO_CNO << "所有学生的第4章作业的首行信息" << endl;
	cout << "       " << procname << " --action firstline --cno " << DEMO_CNO << " --stu all --file all --week 6    : 检查" << DEMO_CNO << "所有学生的第6周作业的首行信息" << endl;
	cout << "       " << procname << " --action firstline --cno " << DEMO_CNO << " --stu " << DEMO_STUNO << " --file all         : 检查" << DEMO_CNO << "的" << DEMO_STUNO << "学生的所有作业的首行信息" << endl;
	cout << "       " << procname << " --action firstline --cno " << DEMO_CNO << " --stu " << DEMO_STUNO << " --file " << DEMO_SRC_FNAME << "   : 检查" << DEMO_CNO << "的" << DEMO_STUNO << "学生的" << DEMO_SRC_FNAME << "作业的首行信息" << endl;
	cout << endl;

	cout << "       " << procname << " --action secondline --cno " << DEMO_CNO << " --stu all --file " << DEMO_SRC_FNAME << " : 检查" << DEMO_CNO << "的所有学生的" << DEMO_SRC_FNAME << "作业的次行信息" << endl;
	cout << endl;

	cout << "       " << procname << " --cfgfile E:\\test\\my.conf --action base --cno " << DEMO_CNO << " --stu all --file all : 检查" << DEMO_CNO << "所有学生的所有作业的基本信息(指定配置文件)" << endl;
	cout << endl;


	cout << endl;
}