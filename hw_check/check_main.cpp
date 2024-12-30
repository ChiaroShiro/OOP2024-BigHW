/**
 * @file check_main.cpp
 * @brief �����ں���
 * 
 * @file data.cpp
 * @brief �������ݿ⡢����SQL��ѯ��䡢�����ݿ⽻��
 * 
 * @file printer.cpp
 * @brief �������ģʽ�����ģ�壬ͨ������ָ���������Ч��
 * 
 * @file parser.cpp
 * @brief ���������в������г����д���������
 * 
 * @file tools.cpp
 * @brief �ڶ๤�ߺ���
 * 
 * @file base.cpp
 * @brief �������
 * 
 * @file first.cpp
 * @brief ��һ�м��
 * 
 * @file second.cpp
 * @brief �ڶ��м��
 * 
 * @file debug.cpp
 * @brief ���е��Ժ�����ͨ��--debug��������DEBUG_MODE���������
 */

#include <iostream>
#include <vector>
#include <string>
#include "../include/class_aat.h"
#include "../include_mariadb_x86/mysql/mysql.h"
#include "hw_check.h"
#include "debug.h"
using namespace std;

const string action_set[] = {"ERROR", "base", "firstline", "secondline", "help"};

int main(int argc, char* argv[])
{
	args_analyse_tools args[] = {
		args_analyse_tools("--help",     ST_EXTARGS_TYPE::boolean,            0, false),
		args_analyse_tools("--debug",    ST_EXTARGS_TYPE::boolean,            0, false),
		args_analyse_tools("--action",   ST_EXTARGS_TYPE::str_with_set_error, 1, 0, action_set),
		args_analyse_tools("--cno",      ST_EXTARGS_TYPE::str,                1, string("error")),
		args_analyse_tools("--stu",      ST_EXTARGS_TYPE::str,                1, string("error")),
		args_analyse_tools("--file",     ST_EXTARGS_TYPE::str,                1, string("error")),
		args_analyse_tools("--chapter",  ST_EXTARGS_TYPE::int_with_error,     1, -1, -1, 99),
		args_analyse_tools("--week",     ST_EXTARGS_TYPE::int_with_error,     1, -1, -1, 20),
		args_analyse_tools("--display",  ST_EXTARGS_TYPE::str,                1, string("11111")),
		args_analyse_tools("--cfgfile",  ST_EXTARGS_TYPE::str,                1, string("hw_check.conf")),
		args_analyse_tools()
	};
	int cur_argc = 0;
	if ((cur_argc = args_analyse_process(argc, argv, args, 0)) < 0)
		return -1;
	
	// ���������в���
	INFO info;
	int mode = argvChecker(args, argv, info);
	if(mode == 0)
		return 0;
	__debugInfo(info);

	// �������ݿ⡢���ݴ���
	string path;
	MYSQL* mysql;
	if(!dataMain(info, path, mysql)) {
		cout << "���ݳ�ʼ��ʧ��" << endl;
		return -1;
	}

	// ��ʼ����
	if(mode == 1)
		baseMain(info, mysql, path);
	else if(mode == 2)
		firstlineMain(info, mysql, path);
	else if(mode == 3)
		secondlineMain(info, mysql, path);
	return 0;
}
