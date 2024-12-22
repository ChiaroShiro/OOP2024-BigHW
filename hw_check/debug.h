#pragma once

#include "hw_check.h"

void __debugInfo(INFO info);
void __debugMySQL(string dbserver, string dbuser, string dbpasswd, string dbname, INFO info);
void __debugSQLQueryAll(MYSQL* mysql, INFO info);