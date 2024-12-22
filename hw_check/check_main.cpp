#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>
#include <sstream>
#include "../include/class_aat.h"

using namespace std;



int main(int argc, char* argv[])
{
	const string action_set[] = {"ERROR", "base", "firstline", "secondline", ""};

	args_analyse_tools args[] = {
		args_analyse_tools("--action", 	ST_EXTARGS_TYPE::str_with_set_error,1, 	0, 				action_set),
		args_analyse_tools("--cno",    	ST_EXTARGS_TYPE::str, 				1, 	string("error")),
		args_analyse_tools("--stu",    	ST_EXTARGS_TYPE::str, 				1, 	string("error")),
		args_analyse_tools("--file",    ST_EXTARGS_TYPE::str, 				1, 	string("error")), 
		args_analyse_tools("--chapter", ST_EXTARGS_TYPE::int_with_error, 	1, 	-1, 			0, 2499999), 
		args_analyse_tools("--week",    ST_EXTARGS_TYPE::int_with_error, 	1, 	-1, 			1, 17), 
		args_analyse_tools("--chapter", ST_EXTARGS_TYPE::int_with_error, 	1, 	-1, 			0, 2499999), 
		args_analyse_tools("--display", ST_EXTARGS_TYPE::str, 				1, 	string("11111")), 
		args_analyse_tools("--cfgfile", ST_EXTARGS_TYPE::str, 				1, 	string("hw_check.conf")), 
		args_analyse_tools()
	};
	int cur_argc = 0;
	if ((cur_argc = args_analyse_process(argc, argv, args, 0)) < 0) {
		//错误信息在函数中已打印
		std::cerr << "error" << std::endl;
		return -1;
	}
	if(argvChecker(argc, argv) == 0) {
		return 0;
	}
	std::cerr << "cur_argc: " << cur_argc << std::endl;
	return 0;
}

