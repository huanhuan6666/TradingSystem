//
// Created by Administrator on 2022/2/21.
//

#include "Administrator.h"
#include <iostream>

using namespace std;

Administrator::Administrator() {
    admin_name = "admin"; //默认管理员姓名和密码
    admin_pass = "123456";
}

void Administrator::cmd_display() {
    cout << "=========================================================================" << endl;
    cout << "1.查看所有商品 2.搜索商品 3.查看所有订单 4.查看所有用户 5.封禁用户 6.下架商品 7.注销" << endl;
    cout << "=========================================================================" << endl;
    cout << endl << "请输入操作: ";
}

void Administrator::search_goods() {
    string com_name;
    cout << "请输入商品名称: ";
    cin.sync();
    cin >> com_name;
    sql_cmd = "SELECT * FROM commodity WHERE 名称 CONTAINS " + com_name;
    cout << "对应SQL命令为: " << sql_cmd << endl;
    m_sql_helper.sql_analyse(sql_cmd);
}

void Administrator::display_all_goods() {
    sql_cmd = "SELECT * FROM commodity";
    cout << "对应SQL命令为: " << sql_cmd << endl;
    m_sql_helper.sql_analyse(sql_cmd);
}

void Administrator::display_all_users() {
    sql_cmd = "SELECT * FROM user";
    cout << "对应SQL命令为: " << sql_cmd << endl;
    m_sql_helper.sql_analyse(sql_cmd);
}

void Administrator::display_all_orders() {
    sql_cmd = "SELECT * FROM order";
    cout << "对应SQL命令为: " << sql_cmd << endl;
    m_sql_helper.sql_analyse(sql_cmd);
}

void Administrator::off_the_shelf() {
    string com_id;
    cout << "请输入要下架的商品ID: ";
    cin.sync();
    cin >> com_id;
    sql_cmd = "UPDATE commodity SET 商品状态 = 已下架 WHERE 商品ID = " + com_id;
    cout << "对应SQL命令为: " << sql_cmd << endl;
    m_sql_helper.sql_analyse(sql_cmd);
}

void Administrator::block_users() {
    string user_id;
    cout << "请输入要封禁的用户ID: ";
    cin.sync();
    cin >> user_id;
    sql_cmd = "UPDATE user SET 用户状态 = 封禁 WHERE 用户ID = " + user_id;
    cout << "对应SQL命令为: " << sql_cmd << endl;
    m_sql_helper.sql_analyse(sql_cmd);
    if(m_sql_helper.next == 6) { //需要继续下架
        m_sql_helper.next = 0; //刷新
        sql_cmd = "UPDATE commodity SET 商品状态 = 已下架 WHERE 卖家ID = " + user_id;
        cout << "对应SQL命令为: " << sql_cmd << endl;
        m_sql_helper.sql_analyse(sql_cmd);
    }
}

bool Administrator::check_pass() const {
    string name, pass;
    cout << "请输入管理员姓名(默认admin): ";
    cin.sync();
    cin >> name;
    while(name != admin_name)
    {
        cout << "管理员姓名默认admin，请重新输入: ";
        cin.sync();
        cin >> name;
    }
    cin.sync();
    cout << "请输入密码: ";
    cin >> pass;
    if(pass == admin_pass) {
        cout << endl << "-----登录成功-----" << endl << endl;
        return true;
    }
    cout << endl << "----密码错误，登录失败，返回主菜单----" << endl << endl;
    return false;
}
