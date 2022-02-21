//
// Created by Administrator on 2022/2/21.
//

#include "Users.h"
#include <iostream>

using namespace std;

Users::Users(vector<string> &each) {
    m_id    =  each[0];
    m_name  =  each[1];
    m_pass  =  each[2];
    m_tel   =  each[3];
    m_addr  =  each[4];
    m_money =  stof(each[5]);
    m_state =  each[6];
}


bool Users::check_pass() {
    return true;
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
