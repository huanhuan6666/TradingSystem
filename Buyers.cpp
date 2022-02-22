//
// Created by Administrator on 2022/2/21.
//

#include "Buyers.h"

void Buyers::display_cmd() {
    cout << "===========================================================================" << endl;
    cout << "1.查看商品列表 2.购买商品 3.搜索商品 4.查看历史订单 5.查看商品详细信息 6.返回用户主界面" << endl;
    cout << "===========================================================================" << endl;
    cout << endl << "输入操作: " ;
}

Buyers::Buyers() {
    ;
}

//通过User实例化Buyer，是因为设计框架将User派生出Buyer，main中User确定用户身份为Buyer后将调用该函数成为一个买家对象
//在卖家类中也是如此
Buyers::Buyers(Users &u) {
    m_id    =  u.m_id   ;
    m_name  =  u.m_name ;
    m_pass  =  u.m_pass ;
    m_tel   =  u.m_tel  ;
    m_addr  =  u.m_addr ;
    m_money =  u.m_money;
    m_state =  u.m_state;
    m_sql_helper.user_id = m_id;
}
