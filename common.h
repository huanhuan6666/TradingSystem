//
// Created by Administrator on 2022/2/21.
//
/*
 * 这个头文件包含一些通用的内容，比如各种文件路径，一些结构体和实用函数的定义
*/
#ifndef PROJECT1_COMMON_H
#define PROJECT1_COMMON_H
#include <string>

using namespace std;
//通用的文件名
const string commodity_file = "commodity.txt";
const string commands_file = "commands.txt";
const string order_file = "order.txt";
const string user_file = "user.txt";

//商品和订单我们只关注其数据，因此表示成结构体就可以
struct Order_t
{
    string o_id; //订单ID
    string c_id; //商品ID
    float o_price; //交易单价
    int o_count; //数量
    string o_time; //交易时间
    string m_id; //卖家ID
};

struct Commodity_t
{
    string c_id; //商品ID
    string c_name; //商品名称
    float c_price; //商品价格
    string c_time; //上架时间
    string m_id;  //卖家ID
};

#endif //PROJECT1_COMMON_H
