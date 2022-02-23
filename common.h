//
// Created by Administrator on 2022/2/21.
//
/*
 * 这个头文件包含一些通用的内容，比如各种文件路径，一些结构体和实用函数的定义
 * 比如用来辅助SqlHelper解析和执行命令的各种函数
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

//一些状态，用在UserSqlHelper中区分买家卖家
const int STATUS_SELLER = 3;
const int STATUS_BUYER = 6;

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

//重载Order_t的 << 方便在文件中的输出
inline ostream& operator<<(ostream& out, Order_t& odr)
{
    out << setiosflags(ios::fixed);
    out << odr.o_id << "," << odr.c_id << "," << setprecision(1) << odr.o_price << ","
        << odr.o_count << "," << odr.o_time << "," << odr.m_id << "," << odr.m_buy_id << endl;
    return out;
}
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

//重载Commodity_t的<<方便在文件中的输出
inline ostream &operator<<(ostream &out, Commodity_t& com)
{
    out << setiosflags(ios::fixed);
    out << com.c_id << "," << com.c_name << "," << setprecision(1) << com.c_price << "," << com.c_count << ","
        << com.c_des << "," << com.m_id << "," << com.c_time << "," << com.c_state << endl;
    return out;
}
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

//展示商品表中的条目
//为了让接口更紧把管理员和用户的展示都合并进来了
//管理员模式: type == 0~2表示展示全部,=1表示值相等,=2表示有CONTAINS包含条件
//卖家模式: type==3~5 表示展示某个 卖家ID 的条目, value需要传入卖家ID
//买家模式: type==6~8 展示当前销售中的商品
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
    if (type == 0)  //管理员 显示所有条目
    {
        while (getline(fin, line)) {
            vector<string> each;
            my_split(line, ',', each); //用,分隔commodity文件中的每一行填充each
            Commodity_t tmp(each);
            res.push_back(tmp); //添加该商品到res中
        }
    }
    else if(type == 1 || type == 2)//管理员 带有额外条件的
    {
        while (getline(fin, line)) {
            vector<string> each;
            my_split(line, ',', each); //用,分隔commodity文件中的每一行填充each
            Commodity_t tmp(each);
            if (option == "名称") {
                if (type == 1) {//值相等条件，即强匹配搜索
                    if (tmp.c_name == value)
                        res.push_back(tmp); //添加该商品到res中
                } else { //CONTAINS包含字串
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
    else if(type == 3) //卖家模式 查看本ID下所有状态的商品
    {
        while (getline(fin, line)) {
            vector<string> each;
            my_split(line, ',', each); //用,分隔commodity文件中的每一行填充each
            Commodity_t tmp(each);
            if (tmp.m_id == value) //调用时value需要传入该用户ID
                res.push_back(tmp);
        }
    }
    else if(type == 6) //买家模式 查看当前在售所有商品
    {
        while (getline(fin, line)) {
            vector<string> each;
            my_split(line, ',', each); //用,分隔commodity文件中的每一行填充each
            Commodity_t tmp(each);
            if(tmp.c_state == "销售中")
                res.push_back(tmp); //买家只看销售中的商品
        }
    }
    else if(type == 7 || type == 8) //买家模式的 CONTAINS搜索 和 查看==详细信息
    {
        while (getline(fin, line)) {
            vector<string> each;
            my_split(line, ',', each); //用,分隔commodity文件中的每一行填充each
            Commodity_t tmp(each);
            if (option == "名称") {
                if (type == 7) {//值相等条件，即强匹配搜索
                    if (tmp.c_state == "销售中" && tmp.c_name == value)
                        res.push_back(tmp);
                } else { //CONTAINS包含字串
                    if (tmp.c_state == "销售中" && tmp.c_name.find(value) != -1)
                        res.push_back(tmp);
                }
            }
            else if (option == "商品ID") {
                if (type == 7) {//值相等条件，即强匹配搜索
                    if (tmp.c_state == "销售中" && tmp.c_id == value)
                        res.push_back(tmp);
                } else {
                    // 事实上用户查看商品要求生成的Sql语句就是 ID CONTAINS ..而没用=，我也不好说什么..
                    if (tmp.c_state == "销售中" && tmp.c_id.find(value) != -1)
                        res.push_back(tmp);
                }
            }
            else if (tmp.c_state == "销售中" && option == "卖家ID") {
                if (tmp.m_id == value)
                    res.push_back(tmp);
            }
            else //其他选项
            {
                cout << "Usage: 暂不支持条件选项 " << option << " 的匹配" << endl;
                break;
            }
        }
        //买家的这个展示比较特殊，单独cout吧还是
        if(res.empty())
        {
            goto EMPTY;
        }
        if(option == "商品ID") {
            for (const auto &com: res) {
                cout << "********************************" << endl;
                cout << "商品ID: " << com.c_id << endl;
                cout << "商品名称: " << com.c_name << endl;
                cout << "商品价格: " << com.c_price << endl;
                cout << "上架时间: " << com.c_time << endl;
                cout << "商品描述: " << com.c_des << endl;
                cout << "商品卖家: " << com.m_id << endl;
                cout << "********************************" << endl;
            }
            cout << endl;
            goto END;
        }
    }
EMPTY:
    if(res.empty())
    {
        //可以根据type的不同展示不同的输出
        //if(type < 3) { //管理员模式
        cout << "*****************************" << endl;
        cout << "没有找到您想要的商品！返回初始界面" << endl;
        cout << "*****************************" << endl;
        //}
        goto END;
    }
    cout << endl << "********************************************************************" << endl;
    cout << "商品ID   名称      价格      上架时间       卖家ID    数量       商品状态" << endl;
    for (const auto& com: res) {
        cout << setiosflags(ios::fixed);
        cout << com.c_id << '\t' << com.c_name << '\t' << setprecision(1) << com.c_price << '\t'
             << com.c_time << '\t' << com.m_id << '\t' << com.c_count << '\t' << com.c_state << endl;
    }
    cout << "********************************************************************" << endl;

END:
    fin.close();
    return;
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
    }
    else {
        //待补
    }
    if(res.empty())
    {
        cout << "*****************************" << endl;
        cout << "没有找到您想要的用户！返回初始界面"  << endl;
        cout << "*****************************" << endl;
        goto END;
    }
    cout << endl << "*************************************************************" << endl;
    cout << "用户ID   用户名      联系方式      地址       钱包余额     用户状态" << endl;
    for (const auto& user: res) {
        cout << setiosflags(ios::fixed);
        cout << user.m_id << '\t' << user.m_name << '\t' << user.m_tel << '\t'
             << user.m_addr << '\t' << setprecision(1) << user.m_money << '\t' << user.m_state << endl;
    }
    cout << "**************************************************************" << endl;
END:
    fin.close();
    return;
}

//展示订单的函数，同样也只实现了type==0展示全部条目, 待补
//管理员模式: type == 0展示全部条目
//卖家模式: type == 3展示自己作为卖家的订单, value需要传入卖家ID
//买家模式: type == 6展示自己作为买家的订单, value需要传入买家ID
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
    }
    else if(type == 3) //卖家模式展示自己作为卖家的订单
    {
        while (getline(fin, line)) {
            vector<string> each;
            my_split(line, ',', each);
            Order_t tmp(each);
            if(tmp.m_id == value) //value是调用时传入的卖家ID
                res.push_back(tmp);
        }
    }
    else if(type == 6) //买家模式展示自己作为买家的订单
    {
        while (getline(fin, line)) {
            vector<string> each;
            my_split(line, ',', each);
            Order_t tmp(each);
            if(tmp.m_buy_id == value) //value是调用时传入的买家ID
                res.push_back(tmp);
        }
    }
    else {
        //待补
    }
    if(res.empty())
    {
        cout << "*****************************" << endl;
        cout << "没有找到您想要的订单！返回初始界面"  << endl;
        cout << "*****************************" << endl;
        goto END ;
    }
    cout << endl << "************************************************************" << endl;
    cout << "订单ID     商品ID   交易单价   数量   交易时间    买家ID   卖家ID" << endl;
    for (const auto& odr: res) {
        cout << setiosflags(ios::fixed);
        cout << odr.o_id << '\t' << odr.c_id << '\t' << setprecision(1) << odr.o_price << '\t'
             << odr.o_count << '\t' << odr.o_time << '\t' << odr.m_buy_id << '\t' << odr.m_id << endl;
    }
    cout << "************************************************************" << endl;
END:
    fin.close();
    return ;
}

//删除旧文件并改名新文件，辅助下面的update函数
inline void rm_rename(const char *newname, const char *oldname)
{
    if (remove(newname) != 0)
    {
        cout << "Error: remove old file error!" << endl;
        return;
    }
    if (rename(oldname, newname) != 0)
    {
        cout << "Error: rename temp file error!" << endl;
        return ;
    }
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
        bool exist = false; //用户表中是否存在该用户
        while (getline(fin, line)) {
            vector<string> each;
            my_split(line, ',', each); //用,分隔user_file文件中的每一行填充each
            Users tmp(each); //得到当前这行代表的用户tmp
            if(option == "用户ID")
            {
                if(tmp.m_id == value) //确实要改
                {
                    exist = true; //ID存在于表中
                    if(tobe_option == "用户状态")
                    {
                        cout << "查询到相关用户ID条目如下，确认要封禁该用户吗?" << endl;
                        cout << "*********************************************" << endl;
                        cout << "用户ID   用户名     联系方式     地址     钱包余额" << endl;
                        cout << setiosflags(ios::fixed);
                        cout << tmp.m_id << '\t' << tmp.m_name << '\t' << tmp.m_tel
                            << '\t' << tmp.m_addr << '\t' << setprecision(1) << tmp.m_money << endl;
                        cout << "*********************************************" << endl;
                        cout << "请选择(y/n)，输入非y代表放弃本次操作: ";
                        string tmp_choose;
                        cin >> tmp_choose;
                        if(tmp_choose == "y") {
                            tmp.m_state = "封禁";
                            fout << tmp;   //User类中重载了<<
                            cout << "封禁成功" << endl;
                        }
                        else {  //放弃则不改变原文件内容继续写tmpfile
                            cout << "操作已放弃" << endl;
                            fout << tmp;
                        }
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
                fin.close();
                fout.close();
                return;
            }
        }
        if(!exist)  //整张表都找不到这个ID的条目
        {
            cout << "********************" << endl;
            cout << "用户表中不存在此ID的条目" << endl;
            cout << "********************" << endl;
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
    rm_rename(newname, oldname);
}

//更新commodity.txt文件中option==value条目的tobe_option为tobe_value
//type == 0为管理员模式,可以修改所有商品
//type == 3为卖家模式, 只能修改自己发布的商品，卖家ID需要传入到id参数中
inline void update_commodity(int type, const string &option, const string &value,
                             const string &tobe_option, const string &tobe_value, const string& id=""){
    vector<Commodity_t> res; //这里的Commodity_t就是结构体了
    ifstream fin(commodity_file);
    ofstream fout("tmp_commodity_file.txt");
    if (!fin || !fout) {
        cout << "Error: open file failed! " << user_file;
        return;
    }
    string line;
    getline(fin, line); //第一行表头
    fout << line << endl; //表头别忘了写到临时文件里！
    //cout << line;
    if (type == 0) //管理员模式
    {
        bool exist = false;
        while (getline(fin, line)) {
            vector<string> each;
            my_split(line, ',', each); //用,分隔文件中的每一行填充each
            Commodity_t tmp(each); //得到当前这行代表的用户tmp
            if(option == "卖家ID") //从封禁用户那里来的，不提示全下架
            {
                if(tmp.m_id == value) //确实要改
                {
                    if(tobe_option == "商品状态")
                    {
                        tmp.c_state = "已下架";
                        fout << tmp;   //Commodity_t重载了<<
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
            else if(option == "商品ID") //从下架商品那里来的，需要提示
            {
                if(tmp.c_id == value) //确实要改
                {
                    exist = true;
                    if(tobe_option == "商品状态")
                    {
                        cout << "查询到相关商品条目如下，确认要下架该商品吗?" << endl;
                        cout << "***********************************************************" << endl;
                        cout << "商品ID   名称     价格     上架时间     数量   卖家ID    商品状态" << endl;
                        cout << setiosflags(ios::fixed);
                        cout << tmp.c_id << '\t' << tmp.c_name << '\t'<< setprecision(1) << tmp.c_price << '\t'
                            << tmp.c_time << '\t' << tmp.c_count << '\t' << tmp.m_id << '\t' << tmp.c_state << endl;
                        cout << "************************************************************" << endl;
                        cout << "请选择(y/n)，输入非y代表放弃本次操作: ";
                        string tmp_choose;
                        cin >> tmp_choose;
                        if(tmp_choose == "y") {
                            tmp.c_state = "已下架";
                            fout << tmp;   //Commodity_t中重载了<<
                            cout << "下架成功" << endl;
                        }
                        else {  //放弃则不改变原文件内容继续写tmpfile
                            cout << "操作已放弃" << endl;
                            fout << tmp;
                        }
                    }
                    else //管理员修改商品的其他信息
                    {
                        //待补
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
                fin.close();
                fout.close();
                return;
            }
        }
        if(!exist)
        {
            cout << "******************" << endl;
            cout << "商品表中不存在相关条目" << endl;
            cout << "******************" << endl;
        }
    }
    else if(type == 3) //卖家模式: 只能修改自己已经发布的商品，这里只实现了卖家下架自己的商品
    {
        bool exist = false;
        while (getline(fin, line)) {
            vector<string> each;
            my_split(line, ',', each); //用,分隔文件中的每一行填充each
            Commodity_t tmp(each); //得到当前这行代表的商品tmp
            if(option == "商品ID")
            {
                if(tmp.m_id == id && tmp.c_id == value) //需要更新，id即卖家ID需要在调用时传入
                {
                    exist = true;
                    if(tobe_option == "商品状态")
                    {
                        cout << "查询到相关商品条目如下，确认要下架该商品吗?" << endl;
                        cout << "***********************************************************" << endl;
                        cout << "商品ID   名称     价格     上架时间     数量   卖家ID    商品状态" << endl;
                        cout << setiosflags(ios::fixed);
                        cout << tmp.c_id << '\t' << tmp.c_name << '\t'<< setprecision(1) << tmp.c_price << '\t'
                             << tmp.c_time << '\t' << tmp.c_count << '\t' << tmp.m_id << '\t' << tmp.c_state << endl;
                        cout << "************************************************************" << endl;
                        cout << "请选择(y/n)，输入非y代表放弃本次操作: ";
                        string tmp_choose;
                        cin >> tmp_choose;
                        if(tmp_choose == "y") {
                            tmp.c_state = tobe_value;
                            fout << tmp;   //Commodity_t中重载了<<
                            cout << "下架成功" << endl;
                        }
                        else {  //放弃则不改变原文件内容继续写tmpfile
                            cout << "操作已放弃" << endl;
                            fout << tmp;
                        }
                    }
                    else if(tobe_option == "价格")
                    {
                        cout << "请确认修改的商品信息无误！" << endl;
                        cout << "**********************************" << endl;
                        cout << "商品ID: " << tmp.c_id << endl;
                        cout << "商品名称: " << tmp.c_name << endl;
                        cout << "商品价格: " << tobe_value << endl;
                        cout << "商品描述: " << tmp.c_des << endl;
                        cout << "**********************************" << endl;
                        cout << "确认修改?(y/n)，输入非y代表放弃本次操作: ";
                        string tmp_choose;
                        cin >> tmp_choose;
                        if(tmp_choose == "y") {
                            tmp.c_price = stof(tobe_value);
                            fout << tmp;   //Commodity_t中重载了<<
                            cout << "修改成功!" << endl;
                        }
                        else {  //放弃则不改变原文件内容继续写tmpfile
                            cout << "操作已放弃" << endl;
                            fout << tmp;
                        }
                    }
                    else if(tobe_option == "描述")
                    {
                        cout << setiosflags(ios::fixed);
                        cout << "请确认修改的商品信息无误！" << endl;
                        cout << "**********************************" << endl;
                        cout << "商品ID: " << tmp.c_id << endl;
                        cout << "商品名称: " << tmp.c_name << endl;
                        cout << "商品价格: " << setprecision(1) << tmp.c_price << endl;
                        cout << "商品描述: " << tobe_value << endl;
                        cout << "**********************************" << endl;
                        cout << "确认修改?(y/n)，输入非y代表放弃本次操作: ";
                        string tmp_choose;
                        cin >> tmp_choose;
                        if(tmp_choose == "y") {
                            tmp.c_des = tobe_value;
                            fout << tmp;   //Commodity_t中重载了<<
                            cout << "修改成功!" << endl;
                        }
                        else {  //放弃则不改变原文件内容继续写tmpfile
                            cout << "操作已放弃" << endl;
                            fout << tmp;
                        }
                    }
                    else //卖家修改商品的其他信息
                    {
                        //待补
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
                fin.close();
                fout.close();
                return;
            }
        }
        if(!exist)
        {
            cout << "********************" << endl;
            cout << "商品表没有您发布的该商品" << endl;
            cout << "********************" << endl;
        }
    }
    else { //其他模式
        //待补
    }

    //重命名覆盖
    fin.close();
    fout.close();

    const char* oldname = "tmp_commodity_file.txt";
    const char* newname = commodity_file.c_str();
    rm_rename(newname, oldname);

}

//将sql命令按照 [时间 : 命令]的格式写到文件中
inline void write_order(const string& cmd)
{
    time_t t = time(0);
    char tmp[32] = { NULL };
    strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&t));
    string cur_time(tmp);
    ofstream fout(commands_file, ios::app);
    fout << cur_time << ": " << cmd << endl;
    fout.close();
}
#endif //PROJECT1_COMMON_H
