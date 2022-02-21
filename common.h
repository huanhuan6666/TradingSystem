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
#include <iomanip>
#include<cstdio>
#include <fstream>
#include <sstream>
#include "Users.h"

using namespace std;
//通用的文件名
const string commodity_file = "commodity.txt";
const string commands_file = "commands.txt";
const string order_file = "order.txt";
const string user_file = "user.txt";

//商品和订单我们只关注其数据，因此表示成结构体就可以
struct Order_t {
    Order_t(vector<string> &each) {
        o_id = each[0];
        c_id = each[1];
        o_price = stof(each[2]);
        o_count = stoi(each[3]);
        o_time = each[4];
        m_id = each[5];
        m_buy_id = each[6];
    }

    string o_id; //订单ID
    string c_id; //商品ID
    float o_price; //交易单价
    int o_count; //数量
    string o_time; //交易时间
    string m_id; //卖家ID
    string m_buy_id; //买家ID
};

struct Commodity_t {
    Commodity_t(vector<string> &each) {
        c_id = each[0];
        c_name = each[1];
        c_price = stof(each[2]);
        c_count = stoi(each[3]);
        c_des = each[4];
        m_id = each[5];
        c_time = each[6];
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
inline void my_split(const string &cmd, const char &pattern, vector<string> &res) {
    res.clear();
    istringstream is(cmd);
    string tmp;

    while (getline(is, tmp, pattern)) {
        res.push_back(tmp);
    }
}

//TODO: 下面这几个show函数大同小异，有时间可以考虑写成函数模板

//展示商品表中 选项option 含有 值value的条目
//为了让接口更紧把展示全部也合并进来了,type == 0表示展示全部,=1表示值相等,=2表示有CONTAINS包含条件
inline void show_commodity(int type, const string &option, const string &value) {
    vector<Commodity_t> res;
    ifstream fin(commodity_file);
    if (!fin) {
        cout << "Error: open file failed! " << commodity_file;
        return;
    }
    string line;
    getline(fin, line); //第一行表头
    //cout << line;
    if (type == 0)  //显示所有条目
    {
        while (getline(fin, line)) {
            vector<string> each;
            my_split(line, ',', each); //用,分隔commodity文件中的每一行填充each
            Commodity_t tmp(each);
            res.push_back(tmp); //添加该商品到res中
        }
    } else //带有额外条件的
    {
        while (getline(fin, line)) {
            vector<string> each;
            my_split(line, ',', each); //用,分隔commodity文件中的每一行填充each
            Commodity_t tmp(each);
            if (option == "名称") {
                if (type == 1) {//值相等条件，即强匹配搜索
                    if (tmp.c_name == value)
                        res.push_back(tmp); //添加该商品到res中
                } else if (type == 2) { //CONTAINS包含字串
                    if (tmp.c_name.find(value) != -1)
                        res.push_back(tmp);
                }
            } else if (option == "商品ID") {
                if (tmp.c_id == value)
                    res.push_back(tmp);
            } else if (option == "卖家ID") {
                if (tmp.m_id == value)
                    res.push_back(tmp);
            } else //其他选项
            {
                cout << "Usage: 暂不支持条件选项 " << option << " 的匹配" << endl;
                break;
            }
        }
    }

    cout << endl << "*******************************************************************" << endl;
    cout << "商品ID   名称      价格      上架时间       卖家ID    数量       商品状态" << endl;
    for (auto com: res) {
        cout << com.c_id << '\t' << com.c_name << '\t' << com.c_price << '\t'
             << com.c_time << '\t' << com.m_id << '\t' << com.c_count << '\t' << com.c_state << endl;
    }
    cout << "********************************************************************" << endl;
    fin.close();

}

//和show_commodity类似，不过目前只实现了展示所有用户的功能，即type==0，后续可能补条件搜索
inline void show_user(int type, const string &option, const string &value) {
    vector<Users> res; //这里的Users用的是class类
    ifstream fin(user_file);
    if (!fin) {
        cout << "Error: open file failed! " << user_file;
        return;
    }
    string line;
    getline(fin, line); //第一行表头
    //cout << line;
    if (type == 0)  //显示所有条目
    {
        while (getline(fin, line)) {
            vector<string> each;
            my_split(line, ',', each); //用,分隔user_file文件中的每一行填充each
            Users tmp(each);
            res.push_back(tmp); //添加该商品到res中
        }
    } else {
        //待补
    }
    cout << endl << "*************************************************************" << endl;
    cout << "用户ID   用户名      联系方式      地址       钱包余额     用户状态" << endl;
    for (auto user: res) {
        cout << setiosflags(ios::fixed);
        cout << user.m_id << '\t' << user.m_name << '\t' << user.m_tel << '\t'
             << user.m_addr << '\t' << setprecision(1) << user.m_money << '\t' << user.m_state << endl;
    }
    cout << "**************************************************************" << endl;
    fin.close();
}

//展示订单的函数，同样也只实现了type==0展示全部条目
inline void show_order(int type, const string &option, const string &value) {
    vector<Order_t> res; //这里的Users用的是class类
    ifstream fin(order_file);
    if (!fin) {
        cout << "Error: open file failed! " << user_file;
        return;
    }
    string line;
    getline(fin, line); //第一行表头
    //cout << line;
    if (type == 0)  //显示所有条目
    {
        while (getline(fin, line)) {
            vector<string> each;
            my_split(line, ',', each); //用,分隔user_file文件中的每一行填充each
            Order_t tmp(each);
            res.push_back(tmp); //添加该商品到res中
        }
    } else {
        //待补
    }

    cout << endl << "************************************************************" << endl;
    cout << "订单ID     商品ID   交易单价   数量   交易时间    买家ID   卖家ID" << endl;
    for (auto odr: res) {
        cout << setiosflags(ios::fixed);
        cout << odr.o_id << '\t' << odr.c_id << '\t' << setprecision(1) << odr.o_price << '\t'
             << odr.o_count << '\t' << odr.o_time << '\t' << odr.m_buy_id << '\t' << odr.m_id << endl;
    }
    cout << "************************************************************" << endl;
    fin.close();
}

//update命令同样用type支持各种模式,
//将option==value的条目的tobe_option更新成tobe_value
//更新文件的方式是典中典的重写临时文件然后删除原文件改名
inline void update_user(int type, const string &option, const string &value,
                        const string &tobe_option, const string &tobe_value) {
    vector<Users> res; //这里的Users用的是class类
    ifstream fin(user_file);
    ofstream fout("tmp_user_file.txt");
    if (!fin || !fout) {
        cout << "Error: open file failed! " << user_file;
        return;
    }
    string line;
    getline(fin, line); //第一行表头
    fout << line << endl; //表头别忘了写到临时文件里！
    //cout << line;
    if (type == 0)
    {
        while (getline(fin, line)) {
            vector<string> each;
            my_split(line, ',', each); //用,分隔user_file文件中的每一行填充each
            Users tmp(each); //得到当前这行代表的用户tmp
            if(option == "用户ID")
            {
                if(tmp.m_id == value) //确实要改
                {
                    if(tobe_option == "用户状态")
                    {
                        tmp.m_state = "封禁";
                        fout << tmp;   //User类中重载了<<
                    }
                    else
                    {
                        cout << "Usage: 选项 " << tobe_option << "暂不支持, 待补" << endl;
                    }

                }
                else //不用改则直接写到临时文件
                {
                    fout << line << endl;
                }
            }
            else
            {
                cout << "Usage: 选项 " << tobe_option << "暂不支持, 待补" << endl;
                return;
            }
        }

    }
    else {
        //待补
    }

    //重命名覆盖
    fin.close();
    fout.close();

    const char* oldname = "tmp_user_file.txt";
    const char* newname = user_file.c_str();
    if (remove(newname) != 0)
    {
        cout << "Error: remove old file error!";
        return;
    }
    if (rename(oldname, newname) != 0)
    {
        cout << "Error: rename temp file error!";
        return ;
    }
}

#endif //PROJECT1_COMMON_H
