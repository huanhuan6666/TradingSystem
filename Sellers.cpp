//
// Created by Administrator on 2022/2/21.
//

#include "Sellers.h"
#include <iostream>
#include <iomanip>
using namespace std;

void Sellers::display_cmd(){
    cout << "=======================================================================" << endl;
    cout << "1.发布商品 2.查看发布商品 3.修改商品信息 4.下架商品 5.查看历史订单 6.返回用户主界面" << endl;
    cout << "=======================================================================" << endl;
    cout << endl << "输入操作: " ;

}

Sellers::Sellers() {
    m_sql_helper.user_status = STATUS_SELLER;
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
    m_sql_helper.user_status = STATUS_SELLER;
}

void Sellers::release_good() {

}


void Sellers::update_my_good() {
    string sql_cmd, com_id;
    cout << "请输入被修改商品的ID: ";
    cin >> com_id;
    string tmp;
    cout << "请输入被修改商品的属性(1.价格 2.描述): ";
    cin >> tmp;
    if(tmp != "1" && tmp != "2")
    {
        cout << "没有这个功能，请输入上述展示功能对应的数字!" << endl;
        return;
    }
    if(tmp == "1")
    {
        string price;
        cout << "请输入被修改商品的价格(保留一位小数): ";
        cin >> price;
        float p = stof(price); //用串IO保留一位小数
        ostringstream sout;
        sout << setiosflags(ios::fixed);
        sout << setprecision(1) << p;
        sql_cmd = "UPDATE commodity SET 价格 = " + sout.str() + " WHERE 商品ID = " + com_id;
    }
    else if(tmp == "2")//tmp == 2
    {
        string des;
        cout << "请输入被修改商品的描述(超过100个字符部分将被截断): ";
        cin >> des;
        sql_cmd = "UPDATE commodity SET 描述 = " + des.substr(0, 99) + " WHERE 商品ID = " + com_id;
    }
    else //异常输入
    {
        cout << "没有这个功能，请输入上述展示功能对应的数字!" << endl;
        return ;
    }
    cout << "对应SQL命令为: " << sql_cmd << endl;
    m_sql_helper.sql_analyse(sql_cmd);
}

void Sellers::off_my_shelf() {
    string com_id;
    cout << "请输入要下架的商品ID: ";
    cin >> com_id;
    string sql_cmd = "UPDATE commodity SET 商品状态 = 已下架 WHERE 商品ID = " + com_id;
    cout << "对应SQL命令为: " << sql_cmd << endl;
    m_sql_helper.sql_analyse(sql_cmd);
}

