//
// Created by Administrator on 2022/2/21.
//
/*
 * 这个头文件包含一些通用的内容，比如各种文件路径，一些结构体和实用函数的定义
*/
#ifndef PROJECT1_COMMON_H
#define PROJECT1_COMMON_H
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
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
    Commodity_t(vector<string>& each)
    {
        c_id    = each[0];
        c_name  = each[1];
        c_price = stof(each[2]);
        c_count = stoi(each[3]);
        c_des   = each[4];
        m_id    = each[5];
        c_time  = each[6];
        c_state = each[7];
    }
    string c_id; //商品ID
    string c_name; //商品名称
    float c_price; //商品价格
    int c_count; //数量
    string c_des; //描述
    string m_id;  //卖家ID
    string c_time; //上架时间
    string c_state; //商品状态
};

//将字符串cmd按照字符pattern分隔，将结果存放到res中
//通过串IO输入流实现的，getline可指定delim分隔字符
inline void my_split(const string &cmd, const char &pattern, vector<string>& res) {
    res.clear();
    istringstream is(cmd);
    string tmp;

    while(getline(is, tmp, pattern))
    {
        res.push_back(tmp);
    }
}

//展示商品表中 选项option 含有 值value的条目
//为了让接口更紧把展示全部也合并进来了,type == 0表示展示全部，非0表示有额外条件
inline void show_commodity(int type, const string& option, const string& value)
{
    vector<Commodity_t> res;
    ifstream fin(commodity_file);
    if(!fin)
    {
        cout << "Error: open file failed! " << commodity_file;
        return;
    }
    string line;
    getline(fin, line); //第一行表头
    //cout << line;
    if(type == 0)  //显示所有条目
    {
        while(getline(fin, line))
        {
            vector<string> each;
            my_split(line, ',', each); //用,分隔commodity文件中的每一行填充each
            Commodity_t tmp(each);
            res.push_back(tmp); //添加该商品到res中
        }
    }
    else //带有额外条件的
    {
        while(getline(fin, line))
        {
            vector<string> each;
            my_split(line, ',', each); //用,分隔commodity文件中的每一行填充each
            Commodity_t tmp(each);
            if(option == "名称"){
                if(tmp.c_name == value)
                    res.push_back(tmp); //添加该商品到res中
            }
            else if(option == "商品ID"){
                if(tmp.c_id == value)
                    res.push_back(tmp);
            }
            else if(option == "卖家ID"){
                if(tmp.m_id == value)
                    res.push_back(tmp);
            }
            else //其他选项
            {
                cout << "Usage: 暂不支持条件选项 " << option << " 的匹配" << endl;
                break;
            }
        }
    }

    cout << "*******************************************************************" << endl;
    cout << "商品ID   名称      价格      上架时间       卖家ID    数量       商品状态" << endl;
    for(auto com : res)
    {
        cout << com.c_id << '\t' << com.c_name << '\t' << com.c_price << '\t'
             << com.c_time << '\t' << com.m_id << '\t' << com.c_count << '\t' << com.c_state << endl;
    }
    cout << "********************************************************************" << endl;
    fin.close();

}

#endif //PROJECT1_COMMON_H
