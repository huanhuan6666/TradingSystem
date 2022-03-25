//
// Created by Administrator on 2022/2/21.
//

#ifndef PROJECT1_SQLHELPER_H
#define PROJECT1_SQLHELPER_H
#include <string>
#include <vector>
using namespace std;

//这个类理论上来说是其他类的组件，用来解析中间sql命令
//呃好吧把他做成了一个父类，只作为Admin的组件，派生出UserSqlHelper作为User的组件
class SqlHelper {
public:
    int next; //用于管理员封禁用户时决定是否继续下架商品，如果放弃封禁则不执行下架指令
    vector<string> str_table;
public:
    virtual void sql_analyse(const string &cmd);
};

//因为对于同一个命令管理员获得的结果和用户获得的结果不同，所以还是把SqlHelper分开比较好
//传入的user_id就表示对应的用户ID，同样这个类是作为User的组件
class UserSqlHelper : public SqlHelper{
public:
    int user_status;  //表示当前用户是买家还是卖家 0为卖家 1为买家
    string user_id; //用户ID

public:
    UserSqlHelper() { ; }
    UserSqlHelper(int& status, string &id);
    void sql_analyse(const string &cmd) override;
};
#endif //PROJECT1_SQLHELPER_H
