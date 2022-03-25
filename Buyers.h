//
// Created by Administrator on 2022/2/21.
//

#ifndef PROJECT1_BUYERS_H
#define PROJECT1_BUYERS_H
#include "Users.h"
#include "common.h"
#include <iostream>

using namespace std;

class Buyers : public Users{
public:
    int m_status = STATUS_BUYER;
public:
    Buyers();
    explicit Buyers(Users &u);
    void display_cmd() override; //展示命令行
    void buy_goods();   //购买商品
    void search_goods();    //搜索商品
    void display_good_info();   //查看商品详细信息
};


#endif //PROJECT1_BUYERS_H
