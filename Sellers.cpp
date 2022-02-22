//
// Created by Administrator on 2022/2/21.
//

#include "Sellers.h"
#include <iostream>

using namespace std;

void Sellers::display_cmd(){
    cout << "=======================================================================" << endl;
    cout << "1.发布商品 2.查看发布商品 3.修改商品信息 4.下架商品 5.查看历史订单 6.返回用户主界面" << endl;
    cout << "=======================================================================" << endl;
    cout << endl << "输入操作: " ;

}

Sellers::Sellers() {
    ;
}

Sellers::Sellers(Users &u) {
    m_id    =  u.m_id   ;
    m_name  =  u.m_name ;
    m_pass  =  u.m_pass ;
    m_tel   =  u.m_tel  ;
    m_addr  =  u.m_addr ;
    m_money =  u.m_money;
    m_state =  u.m_state;
    m_sql_helper.user_id = m_id;
}

void Sellers::release_good() {

}

void Sellers::display_my_good() {
    string sql_cmd = "SELECT * FROM commodity";
    cout << "对应SQL命令为: " << sql_cmd << endl;
    m_sql_helper.sql_analyse(sql_cmd);
}

void Sellers::update_my_good() {

}

void Sellers::off_my_shelf() {

}

void Sellers::display_my_order() {

}
