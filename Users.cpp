//
// Created by Administrator on 2022/2/21.
//

#include "Users.h"
#include <iostream>
#include "common.h"
using namespace std;

Users::Users(vector<string> &each) {
    m_id    =  each[0];
    m_name  =  each[1];
    m_pass  =  each[2];
    m_tel   =  each[3];
    m_addr  =  each[4];
    m_money =  stof(each[5]);
    m_state =  each[6];
    m_sql_helper.user_id = m_id; //向SQL解析器转入自己的ID
}

//调用这个成员函数的User将会进行登录检测，成功登录的话该User对象的属性将全部按照文件内容填写
//即真正代表某个确定的用户
bool Users::check_pass() {
    string name, pass, line;
    cout << "请输入用户名: ";
    cin >> name;
    ifstream fin(user_file);
    if(!fin)
    {
        cout << "Error: open file failed! " << user_file << endl;
        return false;
    }
    getline(fin, line); //第一行表头
    while(getline(fin, line))
    {
        vector<string> each;
        my_split(line, ',', each);
        Users tmp(each);
        if(tmp.m_name == name) //用户名存在
        {
            if(tmp.m_state == "封禁")
            {
                cout << "当前用户已被封禁，无法登录" << endl;
                fin.close();
                return false;
            }
            cout << "请输入密码: ";
            cin >> pass;
            if(pass == tmp.m_pass)
            {
                cout << "-----登录成功-----" << endl;
                *this = tmp; //!!!填写用户的全部信息，得到独一无二的用户实例
                fin.close();
                return true;
            }
            else
            {
                cout << "-----密码错误!-----" << endl;
                fin.close();
                return false;
            }
        }
        else
            continue;
    }
    cout << "用户表中不存在此用户名" << endl;
    fin.close();
    return false;

}

void Users::display_cmd() {
    cout << "========================================" << endl;
    cout << "1.注销登录 2.我是买家 3.我是卖家 4.个人信息管理 " << endl;
    cout << "========================================" << endl;
    cout << endl << "输入操作: " ;

}

ostream &operator<<(ostream& out, Users &u)
{
    out << u.m_id << ',' << u.m_name << ',' << u.m_pass << ',' << u.m_tel << ','
        << u.m_addr << ',' << u.m_money << ',' << u.m_state << endl;
    return out;
}

Users &Users::operator=(Users &tmp) {
    m_id    =  tmp.m_id   ;
    m_name  =  tmp.m_name ;
    m_pass  =  tmp.m_pass ;
    m_tel   =  tmp.m_tel  ;
    m_addr  =  tmp.m_addr ;
    m_money =  tmp.m_money;
    m_state =  tmp.m_state;
    m_sql_helper.user_id = m_id;
    return *this;
}
