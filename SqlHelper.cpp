//
// Created by Administrator on 2022/2/21.
//

#include "SqlHelper.h"
#include <iostream>
#include <sstream>
#include <string>
#include "common.h"

using namespace std;

void SqlHelper::sql_analyse(const string &cmd) {
    my_split(cmd, ' ', str_table); //用空格分隔cmd并填充str_table
    int i = 0, size = str_table.size();
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
                    cout << "Usage: 无法解析命令 " << cmd << " error: " << str_table[4] << endl;
                    return ;
                }
                option = str_table[5];
                value = str_table[7];
                if(str_table[6] == "=") //强搜索
                    show_commodity(1, option, value);
                else if(str_table[6] == "CONTAINS") //包含搜索
                    show_commodity(2, option, value);
                else //除了CONTAINS 和 = 以外的
                {
                    cout << "Usage: 无法解析命令 " << cmd << " error: " << str_table[6] << endl;
                    return ;
                }
            }
            else //没有额外条件则展示所有条目
                show_commodity(0, "", "");
        }
        else if(where == "user") //查看用户
        {
                show_user(0, "", "");
        }
        else if(where == "order") //查看订单
        {
                show_order(0, "", "");
        }
    }
    else if(first == "INSERT")
    {

    }
    else if(first == "UPDATE")
    {

    }
    else if(first == "DELETE")
    {

    }
    else
    {
        cout << "Usage: 无法解析命令 " << first << "...相关操作未达成" << endl;
        return;
    }

}