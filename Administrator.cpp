//
// Created by Administrator on 2022/2/21.
//

#include "Administrator.h"
#include <iostream>

using namespace std;

Administrator::Administrator() {

}

void Administrator::cmd_display() {
    cout << "=========================================================================" << endl;
    cout << "1.查看所有商品 2.搜索商品 3.查看所有订单 4.查看所有用户 5.删除用户 6.下架商品 7.注销" << endl;
    cout << "=========================================================================" << endl;
}

void Administrator::search_goods() {

}

void Administrator::display_all_goods() {

}

void Administrator::display_all_users() {

}

void Administrator::display_all_orders() {

}

void Administrator::off_the_shelf() {

}

void Administrator::block_users() {

}

bool Administrator::check_pass() {
    return true;
}
