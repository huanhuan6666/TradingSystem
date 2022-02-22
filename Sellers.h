//
// Created by Administrator on 2022/2/21.
//

#ifndef PROJECT1_SELLERS_H
#define PROJECT1_SELLERS_H
#include "Users.h"
#include "common.h"

class Sellers : public Users{
public:
    int m_status = STATUS_SELLER; //卖家
public:
    Sellers();
    Sellers(Users &u);
    void display_cmd() override; //展示命令行
    void release_good();  //发布商品
    void update_my_good();  //修改商品信息
    void off_my_shelf();   //下架商品


};


#endif //PROJECT1_SELLERS_H
