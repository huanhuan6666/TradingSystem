//
// Created by Administrator on 2022/2/21.
//

#ifndef PROJECT1_SQLHELPER_H
#define PROJECT1_SQLHELPER_H
#include <string>
#include <vector>
using namespace std;

//这个类理论上来说是其他类的组件，用来解析中间sql命令
class SqlHelper {
public:
    vector<string> str_table;
public:
    void sql_analyse(const string &cmd);
};


#endif //PROJECT1_SQLHELPER_H
