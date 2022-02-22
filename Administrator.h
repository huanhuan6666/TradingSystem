//
// Created by Administrator on 2022/2/21.
//

#ifndef PROJECT1_ADMINISTRATOR_H
#define PROJECT1_ADMINISTRATOR_H

#include <iostream>
#include "SqlHelper.h"

using namespace std;

// 管理员类
class Administrator {
public:
    string admin_name;
    string admin_pass;
    string sql_cmd; //选择功能中间生成的SQL命令
    SqlHelper m_sql_helper; //用来解析sql语句的对象
public:
    Administrator();

    bool check_pass() const;   //检测密码
    void cmd_display();  //展示管理员功能界面
    void search_goods();  //搜索商品
    void display_all_goods(); //查看所有商品
    void display_all_users(); //查看所有用户

    void display_all_orders(); //查看所有订单
    void off_the_shelf(); //下架商品
    void block_users();  //封禁用户

};


#endif //PROJECT1_ADMINISTRATOR_H


