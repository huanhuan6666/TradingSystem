//
// Created by Administrator on 2022/2/21.
//

#ifndef PROJECT1_USERS_H
#define PROJECT1_USERS_H
#include <string>
#include <vector>
using namespace std;

//User为基类，买家和卖家需要重写一些接口
class Users {
public:
    string m_id; //用户ID
    string m_name;  //用户姓名
    string m_pass; //密码
    string m_tel; //联系方式
    string m_addr;  //地址
    float m_money; //余额
    string m_state;  //用户状态

public:
    Users() { ; } //默认构造函数
    Users(vector<string> &each);
    bool check_pass(); //密码验证
    virtual void display_cmd(); //界面展示


};


#endif //PROJECT1_USERS_H
