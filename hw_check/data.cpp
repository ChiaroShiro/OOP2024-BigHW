#include "hw_check.h"
#include "debug.h"
#include <iostream>
#include <string>
#include "../include/class_cft.h"

#include "../include_mariadb_x86/mysql/mysql.h"      // mysql特有

using namespace std;


/*
+-----------------+------------+------+-----+---------+-------+
| Field           | Type       | Null | Key | Default | Extra |
+-----------------+------------+------+-----+---------+-------+
| stu_term        | char(11)   | NO   |     |         |       |
| stu_grade       | char(4)    | NO   |     |         |       |
| stu_no          | char(8)    | NO   |     |         |       |
| stu_name        | char(32)   | NO   |     |         |       |
| stu_sex         | varchar(1) | NO   |     |         |       |
| stu_class_fname | char(32)   | NO   |     |         |       |
| stu_class_sname | char(16)   | NO   |     |         |       |
| stu_cno         | char(16)   | YES  |     | NULL    |       |
+-----------------+------------+------+-----+---------+-------+


+-------------+--------------+------+-----+---------+-------+
| Field       | Type         | Null | Key | Default | Extra |
+-------------+--------------+------+-----+---------+-------+
| hw_ftype    | int(2)       | NO   |     | 0       |       |
| hw_cno      | char(16)     | NO   |     | NULL    |       |
| hw_id       | smallint(6)  | NO   |     | NULL    |       |
| hw_chapter  | tinyint(4)   | NO   |     | NULL    |       |
| hw_week     | tinyint(4)   | NO   |     | NULL    |       |
| hw_filename | char(64)     | NO   |     | NULL    |       |
| hw_score    | decimal(5,1) | NO   |     | 0.0     |       |
+-------------+--------------+------+-----+---------+-------+
*/

const string STUDENT_TABLE = "view_hwcheck_stulist";
const string HW_TABLE = "view_hwcheck_hwlist";

// stu_or_hw: 0-学生，1-作业
string generateSQLQuery(int stu_or_hw, string resultList, _VS conditions, string connect) 
{
	string ret = "select " + resultList + " from " + (stu_or_hw == 0 ? STUDENT_TABLE : HW_TABLE);
	if(conditions.size() > 0) {
		ret += " where ";
		for (int i = 0; i < int(conditions.size()); i++) {
			ret += conditions[i] + ' ' + (i == conditions.size() - 1 ? "" : connect) + ' ';
		}
	}
	ret += ";";
	return ret;
}

string generateSQLQueryFromInfo(INFO info, string resultList)
{
	_VS conditions;
	if(info.cno.size() == 1) {
		conditions.push_back("hw_cno = " + info.cno[0]);
	}
	else if(info.cno.size() == 2) {
		conditions.push_back("(hw_cno = " + info.cno[0] + " or hw_cno = " + info.cno[1] + ")");
	}
	if(info.file != "all") {
		conditions.push_back("hw_file = " + info.file);
	}
	if(info.chapter != -1) {
		conditions.push_back("hw_chapter = " + to_string(info.chapter));
	}
	if(info.week != -1) {
		conditions.push_back("hw_week = " + to_string(info.week));
	}
	return generateSQLQuery(1, resultList, conditions);
}

vector <_VS> sqlQuery(MYSQL* mysql, string sql) 
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

static bool initMySQL(config_file_tools& cfg, MYSQL*& mysql, INFO info) 
{
	string dbserver, dbuser, dbpasswd, dbname;
	cfg.item_get_raw("[数据库]", "db_host", dbserver, 0, 0);
	cfg.item_get_raw("[数据库]", "db_username", dbuser, 0, 0);
	cfg.item_get_raw("[数据库]", "db_passwd", dbpasswd, 0, 0);
	cfg.item_get_raw("[数据库]", "db_name", dbname, 0, 0);

	__debugMySQL(dbserver, dbuser, dbpasswd, dbname, info);

	/* 初始化 mysql 变量，失败返回NULL */
	if ((mysql = mysql_init(NULL)) == NULL) {
		cout << "mysql_init failed" << endl;
		return false;
	}

	/* 连接数据库，失败返回NULL
		1、mysqld没运行
		2、没有指定名称的数据库存在 */
	if (mysql_real_connect(mysql, dbserver.c_str(), dbuser.c_str(), dbpasswd.c_str(), dbname.c_str(), 0, NULL, 0) == NULL) {
		cout << "mysql_real_connect failed(" << mysql_error(mysql) << ")" << endl;
		return false;
	}

	/* 设置字符集，否则读出的字符乱码 */
	mysql_set_character_set(mysql, "gbk");
	return 1;
}

bool dataMain(INFO info, string& path, MYSQL*& mysql) 
{
	config_file_tools cfg(info.cfgfile);
	if(cfg.is_read_succeeded() == 0) {
		cout << "配置文件读取失败" << endl;
		return 0;
	}
	cfg.item_get_raw("[文件目录]", "src_rootdir", path, 0, 0);

	if(!initMySQL(cfg, mysql, info)) {
		return false;
	}
	return 1;
}
