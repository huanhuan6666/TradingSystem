//
// Created by Administrator on 2022/2/21.
//

#include "SqlHelper.h"
#include <iostream>
#include <cstdio>
#include <ctime>
#include <string>
#include <set>
#include "common.h"

using namespace std;

void SqlHelper::sql_analyse(const string &cmd) {
    write_order(cmd); //记录到order文件
    my_split(cmd, ' ', str_table); //用空格分隔cmd并填充str_table
    int size = str_table.size();
    string first(str_table[0]);
    if(first == "SELECT") //查询命令
    {
        if(str_table[1] != "*" || str_table[2] != "FROM")
        {
            cout << "Usage: 无法解析命令 " << cmd << " error: " << str_table[1] << " " << str_table[2] << endl;
            return ;
        }
        string where = str_table[3];
        if(where == "commodity") //查看商品
        {
            if(size > 4) //有额外条件
            {
                string option, value;
                if(str_table[4] != "WHERE")
                {
                    cout << "Usage: 无法解析命令 " << cmd << " 可能的格式error: " << str_table[4] << endl;
                    return ;
                }
                option = str_table[5];
                value = str_table[7];
                if(str_table[6] == "=") {//强搜索
                    show_commodity(1, option, value);
                }
                else if(str_table[6] == "CONTAINS") {//包含搜索
                    show_commodity(2, option, value);
                }
                else //除了CONTAINS 和 = 以外的
                {
                    cout << "Usage: 无法解析命令 " << cmd << " 可能的格式error: " << str_table[6] << endl;
                    return ;
                }
            }
            else {//没有额外条件则展示所有条目
                show_commodity(0, "", "");
            }
        }
        else if(where == "user") //查看用户
        {
                show_user(0, "", "");
        }
        else if(where == "order") //查看订单
        {
                show_order(0, "", "");
        }
        else //没有这个表
        {
            cout << "Usage: 不支持的表项 " << where;
        }
    }
    else if(first == "INSERT")
    {

    }
    else if(first == "UPDATE")
    {
        if(str_table[2] != "SET" || str_table[4] != "=" || str_table[6] != "WHERE" || str_table[8] != "=")
        {
            cout << "Usage: 无法解析命令 " << cmd << " 可能的格式error: "
                << str_table[2] << " " << str_table[4] << " " << str_table[6] << " " << str_table[8] << endl;
            return ;
        }
        string where = str_table[1];
        string tobe_option = str_table[3];
        string tobe_value = str_table[5];
        string option = str_table[7];
        string value = str_table[9];
        if(where == "user")
        {
            update_user(0, option, value, tobe_option, tobe_value);
        }
        else if(where == "commodity")
        {
            update_commodity(0, option, value, tobe_option, tobe_value);
        }
        else //
        {
            cout << "Usage: 不支持的表项 " << where << endl;
        }
    }
    else if(first == "DELETE")
    {

    }
    else
    {
        cout << "Usage: 无法解析命令 " << first << " ...相关操作未达成" << endl;
        return;
    }

}

UserSqlHelper::UserSqlHelper(int& status, string &id) {
    user_status = status;
    user_id = id;
}

//用户解析SQL可以查看和修改的内容会比管理员SqlHelper的少，比如UPDATE和SELECT
//还有用户状态下的INSERT命令就需要大写特写了
void UserSqlHelper::sql_analyse(const string &cmd) {
    write_order(cmd); //记录到order文件
    my_split(cmd, ' ', str_table); //用空格分隔cmd并填充str_table
    int size = str_table.size();
    string first(str_table[0]);
    if(first == "SELECT") //查询命令
    {
        if(str_table[1] != "*" || str_table[2] != "FROM")
        {
            cout << "Usage: 无法解析命令 " << cmd << " error: " << str_table[1] << " " << str_table[2] << endl;
            return ;
        }
        string where = str_table[3];
        if(where == "commodity") //查看发布商品
        {
            if(size > 4) //有额外条件
            {
                string option, value;
                if(str_table[4] != "WHERE")
                {
                    cout << "Usage: 无法解析命令 " << cmd << " 可能的格式error: " << str_table[4] << endl;
                    return ;
                }
                option = str_table[5];
                value = str_table[7];
                //根据user_status传入不同的type即可，待补
                if(str_table[6] == "=") {//强搜索
                    show_commodity(user_status+1, option, value);
                }
                else if(str_table[6] == "CONTAINS") {//包含搜索
                    show_commodity(user_status+2, option, value);
                }
                else //除了CONTAINS 和 = 以外的
                {
                    cout << "Usage: 无法解析命令 " << cmd << " 可能的格式error: " << str_table[6] << endl;
                    return ;
                }
            }
            else {//没有额外条件则展示所有条目
                show_commodity(user_status, "", user_id); //用户需要传入自己状态和ID
            }
        }
        else if(where == "user") //查看用户
        {
            show_user(user_status, "", "");
        }
        else if(where == "order") //查看订单
        {
            if(size <= 4) {//没有额外条件 查看所有订单
                show_order(user_status, "", user_id);
            }
            else //WHERE条件
            {
                //待补
            }
        }
        else //没有这个表
        {
            cout << "Usage: 不支持的表项 " << where;
        }
    }
    else if(first == "INSERT")
    {
        if(str_table[1] != "INTO" || str_table[3] != "VALUES"){
            cout << "Usage: 无法解析命令 " << cmd << " error: " << str_table[1] << " " << str_table[3] << endl;
            return ;
        }
        string where = str_table[2];
        string temp_all = str_table[4]; //需要去除首尾()
        string all_values = temp_all.substr(1, temp_all.length()-2); //理论上就是value1,value2..的那个
        vector<string> values_res;
        my_split(all_values, ',', values_res); //拆开填写到vector中

        if(where == "commodity" && user_status == STATUS_SELLER) { //这个应该是卖家发布商品INSERT到商品表中
            //首先给商品获取一个新ID
            set<string> id_pool; //商品表中已经存在的商品ID池
            ifstream fin(commodity_file);
            if (!fin) {
                cout << "Error: open file failed! " << user_file;
                return;
            }
            string line;
            getline(fin, line); //第一行表头
            while (getline(fin, line)) {
                vector<string> each;
                my_split(line, ',', each);
                Commodity_t tmp(each);
                id_pool.insert(tmp.c_id);
            }
            fin.close(); //得到商品ID池

            //取id池中最大id + 1生成新的id, 用串IO实现3位编号补零
            ostringstream idout;
            idout << setw(3) <<setfill('0') << stoi((*id_pool.rbegin()).substr(1, 3)) + 1;
            string new_id = "M" + idout.str();
            //获取上架时间 年-月-日
            time_t t = time(nullptr);
            char tmp[32] = { 0 };
            strftime(tmp, sizeof(tmp), "%Y-%m-%d", localtime(&t));
            string cur_time(tmp);

            values_res.insert(values_res.begin(), new_id);
            values_res.push_back(user_id);
            values_res.push_back(cur_time); //上架时间
            values_res.emplace_back("销售中");
            //创建新商品实例
            Commodity_t new_com(values_res);
            cout << "请确认发布的商品信息无误! " << endl;
            cout << "*****************************" << endl;
            cout << "商品名称: " << new_com.c_name << endl;
            cout << "商品价格: " << new_com.c_price << endl;
            cout << "商品数量: " << new_com.c_count << endl;
            cout << "商品描述: " << new_com.c_des << endl;
            cout << "上架时间(系统确定): " << new_com.c_time << endl;
            cout << "商品ID(系统确定): " << new_com.c_id << endl;
            cout << "商品状态(系统确定): " << new_com.c_state << endl;
            cout << "*****************************" << endl;

            cout << "您确认要发布商品吗?" << endl;
            cout << "请选择(y/n)，输入非y代表放弃本次操作: ";
            string tmp_choose;
            cin >> tmp_choose;
            if(tmp_choose == "y") { //确认发布则附加写新商品
                ofstream fout(commodity_file, ios::app);
                if (!fout) {
                    cout << "Error: open file failed! " << user_file;
                    return;
                }
                fout << new_com;
                fout.close();
                cout << "发布商品成功！" << endl;
            }else{
                cout << "操作已放弃" << endl;
                return ;
            }
        }
        else if(where == "order" && user_status == STATUS_BUYER) { //这个应该是买家购买商品后INSERT到订单表中
            //需要修改文件：附加写订单文件    修改用户文件的余额  修改商品文件的商品数量
            //XXX:若商品数量为0下架指令在这里被优化了！
            //订单ID和交易时间在生成sql命令的时候就包含了
            cout << "您确认要购买吗?" << endl;
            cout << "请选择(y/n)，输入非y代表放弃本次操作: ";
            string tmp_choose;
            cin >> tmp_choose;
            if(tmp_choose == "y") {
                //修改用户的余额
                float balance;
                ifstream fin(user_file);
                ofstream fout("tmp_user_file.txt");
                if (!fin || !fout) {
                    cout << "Error: open file failed! " << user_file;
                    return;
                }
                string line;
                getline(fin, line); //第一行表头
                fout << line << endl; //表头别忘了写到临时文件里！
                while(getline(fin, line)) {
                    vector<string> each;
                    my_split(line, ',', each); //用,分隔user_file文件中的每一行填充each
                    Users tmp(each); //得到当前这行代表的用户tmp
                    if(tmp.m_id == user_id) { //需要修改这个用户的余额
                        tmp.m_money -= stof(values_res[2]) * (float)stoi(values_res[3]);
                        balance = tmp.m_money;
                        fout << tmp;
                    }else{ //不需要修改直接写
                        fout << line << endl;
                    }
                }
                fin.close();
                fout.close();
                const char* oldname = "tmp_user_file.txt";
                const char* newname = user_file.c_str();
                rm_rename(newname, oldname);
                //修改商品文件的商品数量
                ifstream com_fin(commodity_file);
                ofstream com_fout("tmp_commodity_file.txt");
                if (!com_fin || !com_fout) {
                    cout << "Error: open file failed! " << user_file;
                    return;
                }
                getline(com_fin, line); //第一行表头
                com_fout << line << endl; //表头别忘了写到临时文件里！
                while(getline(com_fin, line)) {
                    vector<string> each;
                    my_split(line, ',', each);
                    Commodity_t tmp(each); //得到当前这行代表的商品tmp
                    if(tmp.c_id == values_res[1]) { //需要修改这个商品的数量
                        tmp.c_count -= stoi(values_res[3]);
                        if(tmp.c_count == 0) {//优化sql指令
                            tmp.c_state = "下架";
                        }
                        com_fout << tmp;
                    }else{ //不需要修改直接写
                        com_fout << line << endl;
                    }
                }
                com_fin.close();
                com_fout.close();
                const char* com_oldname = "tmp_commodity_file.txt";
                const char* com_newname = commodity_file.c_str();
                rm_rename(com_newname, com_oldname);
                //附加写订单文件
                Order_t new_odr(values_res); //生成新订单实例
                ofstream odr_fout(order_file, ios::app);
                if (!odr_fout) {
                    cout << "Error: open file failed! " << user_file;
                    return;
                }
                odr_fout << new_odr;
                odr_fout.close();
                cout << setiosflags(ios::fixed);
                cout << "*************************" << endl;
                cout << "交易提醒!" << endl;
                cout << "交易时间: " << new_odr.o_time << endl;
                cout << "交易单价: " << setprecision(1) << new_odr.o_price << endl;
                cout << "交易数量: " << new_odr.o_count << endl;
                cout << "交易状态: " << "交易成功" << endl;
                cout << "您的余额: " << setprecision(1) << balance << endl;
                cout << "*************************" << endl;
            }
        }
        else{ //待补
            cout << "Usage: 这个表应该不能INSERT吧就是说" << endl;
            return ;
        }
    }
    else if(first == "UPDATE")
    {
        if(str_table[2] != "SET" || str_table[4] != "=" || str_table[6] != "WHERE" || str_table[8] != "=")
        {
            cout << "Usage: 无法解析命令 " << cmd << " 可能的格式error: "
                 << str_table[2] << " " << str_table[4] << " " << str_table[6] << " " << str_table[8] << endl;
            return ;
        }
        string where = str_table[1];
        string tobe_option = str_table[3];
        string tobe_value = str_table[5];
        string option = str_table[7];
        string value = str_table[9];
        if(where == "user")
        {
            update_user(user_status, option, value, tobe_option, tobe_value);
        }
        else if(where == "commodity")
        {
            update_commodity(user_status, option, value, tobe_option, tobe_value, user_id); //别忘了传入user_id
        }
        else //
        {
            cout << "Usage: 不支持的表项 " << where << endl;
        }
    }
    else if(first == "DELETE")
    {

    }
    else
    {
        cout << "Usage: 无法解析命令 " << first << " ...相关操作未达成" << endl;
        return;
    }

}
