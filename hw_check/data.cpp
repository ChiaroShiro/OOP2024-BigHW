#include "hw_check.h"
#include "debug.h"
#include <iostream>
#include <string>
#include "../include/class_cft.h"

#include "../include_mariadb_x86/mysql/mysql.h"      // mysql特有

using namespace std;

vector <vector <string>> sqlQuery(MYSQL* mysql, string sql) 
{
	if (mysql_query(mysql, sql.c_str())) {
		cout << "mysql_query failed(" << mysql_error(mysql) << ")" << endl;
		return vector<vector<string>>();
	}
	MYSQL_RES* result;
	if ((result = mysql_store_result(mysql)) == NULL) {
		cout << "mysql_store_result failed" << endl;
		return vector<vector<string>>();
	}
	MYSQL_ROW  row;
	vector<vector<string>> res;
	while ((row = mysql_fetch_row(result)) != NULL) {
		vector<string> tmp;
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
