#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>
#include "../include/class_aat.h"
#include "../include_mariadb_x86/mysql/mysql.h"      // mysql特有
#include "hw_check.h"
#include "debug.h"

using namespace std;

static void updatePairVector(vector<Pair>& pairs, const string& result) {
    bool found = false;
    for(Pair& p : pairs) {
        if(p.str == result) {
            p.num++;
            found = true;
            break;
        }
    }
    if(!found) {
        pairs.push_back({result, 1});
    }
}

static void printSummary(vector<Pair>& pairs, string title, char separator = '=') {
    int maxLen = 0;
    // 删除数量为0的项
    vector<Pair> newPairs;
    for(Pair& p : pairs) {
        if(p.num > 0) {
            newPairs.push_back(p);
        }
    }
    pairs.clear();
    pairs.insert(pairs.begin(), newPairs.begin(), newPairs.end());

    for(Pair& p : pairs) {
        maxLen = max(maxLen, int(p.str.length()));
    }
    maxLen += 2;
    string sepLine(maxLen + 10, separator);
    
    cout << sepLine << endl;
    cout << title << endl;
    cout << sepLine << endl;
    
    for(Pair& p : pairs) {
        cout << setw(maxLen) << right << p.str << " : " << p.num << endl;
    }
    
    cout << sepLine << endl;
}

static string replaceChineseDot(string str) {
    string result;
    for(size_t i = 0; i < str.length(); i++) {
        // 检查是否为中文点号 (0xA1 0xA4 in GB2312)
        if(i + 1 < str.length() && 
           static_cast<unsigned char>(str[i]) == 0xA1 && 
           static_cast<unsigned char>(str[i+1]) == 0xA4) {
            result += '.';
            i++; // 跳过下一个字节
        } else {
            result += str[i];
        }
    }
    return result;
}


void multifilePrinter(string path, _VS filenames, _VS names, _VS stu_no, string cno, CHECKER_FUNC checker, INIT_FUNC initPairVector)
{
    int lineWidth = 0;
    for (int i = 0; i < int(filenames.size()); i++) {
        lineWidth = max(lineWidth, int(filenames[i].size()));
    }
    ++lineWidth;
    vector <Pair> res;
    initPairVector(res);
    int totalFiles = 0, totalPassed = 0;
    for (int i = 0; i < int(names.size()); i++) {
        cout << setw(3) << left << i + 1 << ": " << "学号-" << stu_no[i] << " 姓名-" << replaceChineseDot(names[i]) << " 课号-" << cno << " 文件数量-" << filenames.size() << endl;
        vector <Pair> singleRes;
        initPairVector(singleRes);
        string path_stu = path + "/" + cno + '-' + stu_no[i];
        for(string filename : filenames) {
            cout << "  " << setw(lineWidth) << left << filename << ": ";
            string result;
            cout << checker(path_stu + "/" + filename, result) << endl;
            
            updatePairVector(res, result);
            updatePairVector(singleRes, result);
        }
        int total = 0, passed = 0;
        for(const Pair& p : singleRes) {
            total += p.num;
            if(p.str == "正确") {
                passed = p.num;
            }
        }
        totalFiles += total;
        totalPassed += passed;
        if(passed == total)
            cout << "全部通过" << passed << "/" << total << "个文件，本次通过" << passed << "个" << endl;
        else 
            cout << "检查通过" << passed << "/" << total << "个文件，本次通过" << passed << "个" << endl;
        printSummary(singleRes, "学生详细信息", '-');
        cout << endl;
    }
    cout << endl;
    cout << "共完成" << names.size() << "个学生的检查，文件总数:" << totalFiles << "，通过总数:" << totalPassed << "，本次通过" << totalPassed << "个" << endl;
    printSummary(res, "整体详细信息", '=');
    cout << endl;
}

void singlefilePrinter(string filename, string name)
{

}