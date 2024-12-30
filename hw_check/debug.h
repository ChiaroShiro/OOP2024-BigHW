#pragma once

#include "hw_check.h"

void __debugInfo(INFO info);
void __debugMySQL(string dbserver, string dbuser, string dbpasswd, string dbname, INFO info);
void __debugSQLQueryAll(MYSQL* mysql, INFO info);
void __debugPrint(const INFO& info, const string& str);
void __debugPrintContainer(const INFO& info, const _VS& container, const string& name);
