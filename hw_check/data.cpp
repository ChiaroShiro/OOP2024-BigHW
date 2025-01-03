#include "hw_check.h"
#include "debug.h"
#include <string>
#include <iostream>
#include "../include/class_cft.h"
#include "../include_mariadb_x86/mysql/mysql.h"
using namespace std;

/**
+------------------------+------------+------+-----+---------+-------+
| Field                  | Type       | Null | Key | Default | Extra |
+------------------------+------------+------+-----+---------+-------+
| @param stu_term        | char(11)   | NO   |     |         |       |
| @param stu_grade       | char(4)    | NO   |     |         |       |
| @param stu_no          | char(8)    | NO   |     |         |       |
| @param stu_name        | char(32)   | NO   |     |         |       |
| @param stu_sex         | varchar(1) | NO   |     |         |       |
| @param stu_class_fname | char(32)   | NO   |     |         |       |
| @param stu_class_sname | char(16)   | NO   |     |         |       |
| @param stu_cno         | char(16)   | YES  |     | NULL    |       |
+------------------------+------------+------+-----+---------+-------+


+------------------------+--------------+------+-----+---------+-------+
| Field                  | Type         | Null | Key | Default | Extra |
+------------------------+--------------+------+-----+---------+-------+
| @param hw_ftype        | int(2)       | NO   |     | 0       |       |
| @param hw_cno          | char(16)     | NO   |     | NULL    |       |
| @param hw_id           | smallint(6)  | NO   |     | NULL    |       |
| @param hw_chapter      | tinyint(4)   | NO   |     | NULL    |       |
| @param hw_week         | tinyint(4)   | NO   |     | NULL    |       |
| @param hw_filename     | char(64)     | NO   |     | NULL    |       |
| @param hw_score        | decimal(5,1) | NO   |     | 0.0     |       |
+------------------------+--------------+------+-----+---------+-------+
*/

const string STUDENT_TABLE = "view_hwcheck_stulist";
const string HW_TABLE = "view_hwcheck_hwlist";

// stu_or_hw: 0-学生，1-作业
string generateSQLQuery(int stu_or_hw, const string& resultList, const _VS& conditions, const string& connect) 
{
	string ret = "select " + resultList + " from " + (stu_or_hw == 0 ? STUDENT_TABLE : HW_TABLE);
	if(conditions.size() > 0) {
		ret += " where ";
		for (int i = 0; i < int(conditions.size()); i++)
			ret += conditions[i] + ' ' + (i == conditions.size() - 1 ? "" : connect) + ' ';
	}
	return ret + ';';
}

string generateSQLQueryFromInfo(const INFO& info, const string& resultList)
{
	_VS conditions;
	if(info.cno.size() == 1)
		conditions.push_back("hw_cno = " + info.cno[0]);
	else if(info.cno.size() == 2)
		conditions.push_back("(hw_cno = " + info.cno[0] + " or hw_cno = " + info.cno[1] + ")");
	if(info.file != "all")
		conditions.push_back("hw_filename = '" + info.file + "'");
	if(info.chapter != -1)
		conditions.push_back("hw_chapter = " + to_string(info.chapter));
	if(info.week != -1)
		conditions.push_back("hw_week = " + to_string(info.week));
	return generateSQLQuery(1, resultList, conditions);
}

vector <_VS> sqlQuery(MYSQL* mysql, const string& sql) 
{
	if (mysql_query(mysql, sql.c_str())) {
		cout << "mysql_query failed(" << mysql_error(mysql) << ")" << endl;
		return vector<_VS>();
	}
	MYSQL_RES* result;
	if ((result = mysql_store_result(mysql)) == NULL) {
		cout << "mysql_store_result failed" << endl;
		return vector<_VS>();
	}
	MYSQL_ROW  row;
	vector<_VS> res;
	while ((row = mysql_fetch_row(result)) != NULL) {
		_VS tmp;
		for (int i = 0; i < int(mysql_num_fields(result)); i++)
			tmp.push_back(string(row[i]));
		res.push_back(tmp);
	}
	mysql_free_result(result);
	return res;
}

static bool initMySQL(config_file_tools& cfg, MYSQL*& mysql, const INFO& info) 
{
	string dbserver, dbuser, dbpasswd, dbname;
	cfg.item_get_raw("[数据库]", "db_host", dbserver, 0, 0);
	cfg.item_get_raw("[数据库]", "db_username", dbuser, 0, 0);
	cfg.item_get_raw("[数据库]", "db_passwd", dbpasswd, 0, 0);
	cfg.item_get_raw("[数据库]", "db_name", dbname, 0, 0);
	__debugMySQL(dbserver, dbuser, dbpasswd, dbname, info);

	if ((mysql = mysql_init(NULL)) == NULL) {
		cout << "mysql_init failed" << endl;
		return 0;
	}
	if (mysql_real_connect(mysql, dbserver.c_str(), dbuser.c_str(), dbpasswd.c_str(), dbname.c_str(), 0, NULL, 0) == NULL) {
		cout << "mysql_real_connect failed(" << mysql_error(mysql) << ")" << endl;
		return 0;
	}
	mysql_set_character_set(mysql, "gbk");
	return 1;
}

bool dataMain(const INFO& info, string& path, MYSQL*& mysql) 
{
	config_file_tools cfg(info.cfgfile);
	if(cfg.is_read_succeeded() == 0) {
		cout << "配置文件读取失败" << endl;
		return 0;
	}
	cfg.item_get_raw("[文件目录]", "src_rootdir", path, 0, 0);
	if(!initMySQL(cfg, mysql, info))
		return 0;
	return 1;
}
