//
// Created by Administrator on 2022/2/21.
//

#ifndef PROJECT1_USERS_H
#define PROJECT1_USERS_H
#include <string>
#include <vector>
#include "SqlHelper.h"

using namespace std;

//User为基类，买家和卖家需要重写一些接口
class Users {
    friend ostream &operator<<(ostream& out, Users &u); //重载<<便于文件输出
public:
    string  m_id; //用户ID
    string  m_name;  //用户姓名
    string  m_pass; //密码
    string  m_tel; //联系方式
    string  m_addr;  //地址
    float   m_money; //余额
    string  m_state;  //用户状态: 正常封禁
    UserSqlHelper m_sql_helper; //用户的SQL解析器

public:
    Users() { ; } //默认构造函数
    explicit Users(vector<string> &each);
    Users &operator=(const Users &tmp);
    bool check_pass(); //密码验证
    void sign_up(); //用户注册，写到了User类中
    void display_my_order(); //查看历史订单: 这个函数在买家卖家中一致，直接写到父类里
    void display_my_good(); //查看商品: 同样买家卖家行为一致，统一写到父类
    virtual void display_cmd(); //界面展示


};


#endif //PROJECT1_USERS_H
