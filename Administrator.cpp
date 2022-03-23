//
// Created by Administrator on 2022/2/21.
//

#include "Administrator.h"
#include <iostream>
#include "common.h"
#include <iomanip>

using namespace std;

Administrator::Administrator() {
    admin_name = "admin"; //默认管理员姓名和密码
    admin_pass = "123456";
}

void Administrator::cmd_display() {
    cout << "=========================================================================" << endl;
    cout << "1.查看所有商品 2.搜索商品 3.查看所有订单 4.查看所有用户 5.封禁用户 6.下架商品 7.注销" << endl;
    cout << "=========================================================================" << endl;
    cout << endl << "请输入操作: ";
}

void Administrator::search_goods() {
    string choose;
    while (true) {
        //管理员可以看到所有商品
        cout << "请输入想要搜索的属性: (1.商品ID 2.名称 3.价格 4.数量 5.卖家ID 6.状态): ";
        getline(cin, choose);
        if (choose.empty()) {
            cout << "输入不能为空！" << endl;
            continue;
        } else
            break;
    }
    string option, value;
    if (choose == "1") { //按照商品ID搜索
        string ID;
        while (true) {
            cout << "请输入要搜索的商品ID: ";
            getline(cin, ID);
            if (ID.empty()) {
                cout << "输入不能为空！" << endl;
                continue;
            } else
                break;
        }
        option = "商品ID";
        value = ID;

    } else if (choose == "2") { //基础功能 利用sql指令按名称搜索
        string com_name;
        cout << "请输入商品名称: ";
        cin.sync();
        cin >> com_name;
        sql_cmd = "SELECT * FROM commodity WHERE 名称 CONTAINS " + com_name;
        cout << "对应SQL命令为: " << sql_cmd << endl;
        m_sql_helper.sql_analyse(sql_cmd);
        return;
    } else if (choose == "3") { //按照价格搜索
        string price;
        float p;
        while (true) {
            cout << "请输入商品价格(保留一位小数): ";
            cin.sync();
            getline(cin, price);
            if (price.empty()) {
                cout << "输入不能为空！" << endl;
                continue;
            }
            int tmp_count = 0;
            bool flag = false;
            for (auto &c: price) {
                if (!isdigit(c) && c != ' ' && c != '.' || tmp_count > 1) {
                    cout << "请输入正确的浮点数！" << endl;
                    flag = true;
                    break;
                }
                if (c == '.') tmp_count++;
            }
            if (flag) {
                continue;
            }
            try {
                p = stof(price); //用串IO保留一位小数
                break;
            }
            catch (invalid_argument &) {
                cout << "请输入正确的浮点数！" << endl;
            }
            catch (...) {
                cout << "其他异常！" << endl;
            }
        }
        ostringstream sout;
        sout << setiosflags(ios::fixed);
        sout << setprecision(1) << p;
        price = sout.str();  //保留一位小数

        option = "价格";
        value = price;
    } else if (choose == "4") { //数量
        string count;
        int q = 0;
        while (true) {
            cout << "请输入商品数量(正整数): ";
            cin.sync();
            getline(cin, count);
            if (count.empty()) {
                cout << "输入不能为空！" << endl;
                continue;
            }
            bool flag = false;
            for (auto &c: count) {
                if (!isdigit(c) && c != ' ' || c == '.') { //有小数点就报错
                    cout << "请输入正确的正整数！" << endl;
                    flag = true;
                    break;
                }
            }
            if (flag) {
                continue;
            }
            try {
                q = stoi(count); //用串IO得到整数
                break;
            }
            catch (invalid_argument &) {
                cout << "请输入正确的正整数！" << endl;
            }
            catch (...) {
                cout << "其他异常！" << endl;
            }
        }
        count = to_string(q); //获取正整数的字符串
        option = "数量";
        value = count;
    } else if (choose == "5") { //按照卖家ID搜索
        string d;
        while (true) {
            cout << "请输入要搜索的商品ID: ";
            getline(cin, d);
            if (d.empty()) {
                cout << "输入不能为空！" << endl;
                continue;
            } else
                break;
        }
        option = "卖家ID";
        value = d;
    } else if (choose == "6") {
        string s;
        while (true) {
            cout << "请输入要搜索的商品状态(1.已下架 2.销售中): ";
            getline(cin, s);
            if (s.empty()) {
                cout << "输入不能为空！" << endl;
                continue;
            } else if (s != "1" && s != "2") {
                cout << "只能输入1或2！" << endl;
                continue;
            } else
                break;
        }
        option = "状态";
        if (s == "1")
            value = "已下架";
        else if (s == "2")
            value = "销售中";
        else
            cout << "不可能！" << endl;
    } else {
        cout << "请输入上述展示功能对应的数字！" << endl;
        return;
    }

    vector<Commodity_t> res;
    ifstream fin(commodity_file);
    if (!fin) {
        cout << "Error: open file failed! " << commodity_file;
        return;
    }
    string line;
    getline(fin, line); //第一行表头
    while (getline(fin, line)) {
        vector<string> each;
        my_split(line, ',', each); //用,分隔commodity文件中的每一行填充each
        Commodity_t tmp(each);
//        if(tmp.c_state != "销售中") //买家只能看到销售中的商品！
//            continue;
        if (option == "商品ID") {
            if(tmp.c_id == value)
                res.push_back(tmp);
        } else if (option == "价格") {
            if(tmp.c_price == stof(value))
                res.push_back(tmp);
        } else if (option == "数量") {
            if(tmp.c_count == stoi(value))
                res.push_back(tmp);
        } else if (option == "卖家ID") {
            if(tmp.m_id == value)
                res.push_back(tmp);
        } else if (option == "状态"){
            if(tmp.c_state == value)
                res.push_back(tmp);
        }
        else {
            cout << "正常情况不可能落到这里" << endl;
        }
    }
    if(res.empty())
    {
        cout << "*****************************" << endl;
        cout << "没有找到您想要的商品！返回初始界面" << endl;
        cout << "*****************************" << endl;
        fin.close();
        return;
    }
    cout << endl << "*******************************************************************************************" << endl;
    cout << internal;
    cout << "商品ID     名称                 价格              上架时间           卖家ID    数量       商品状态" << endl;
    for (const auto& com: res) {
        cout << setiosflags(ios::fixed);
        cout << left << setw(10) << com.c_id << left << setw(20) << com.c_name ;
        cout << left << setw(15) << setprecision(1) << com.c_price << setw(20) << com.c_time << left << setw(10) << com.m_id << setw(10) << com.c_count << setw(10) << com.c_state << endl;
    }
    cout << "*******************************************************************************************" << endl;

    fin.close();
}

void Administrator::display_all_goods() {
    sql_cmd = "SELECT * FROM commodity";
    cout << "对应SQL命令为: " << sql_cmd << endl;
    m_sql_helper.sql_analyse(sql_cmd);
}

void Administrator::display_all_users() {
    sql_cmd = "SELECT * FROM user";
    cout << "对应SQL命令为: " << sql_cmd << endl;
    m_sql_helper.sql_analyse(sql_cmd);
}

void Administrator::display_all_orders() {
    sql_cmd = "SELECT * FROM order";
    cout << "对应SQL命令为: " << sql_cmd << endl;
    m_sql_helper.sql_analyse(sql_cmd);
}

void Administrator::off_the_shelf() {
    string com_id;
    cout << "请输入要下架的商品ID: ";
    cin.sync();
    cin >> com_id;
    sql_cmd = "UPDATE commodity SET 商品状态 = 已下架 WHERE 商品ID = " + com_id;
    cout << "对应SQL命令为: " << sql_cmd << endl;
    m_sql_helper.sql_analyse(sql_cmd);
}

void Administrator::block_users() {
    string user_id;
    cout << "请输入要封禁的用户ID: ";
    cin.sync();
    cin >> user_id;
    sql_cmd = "UPDATE user SET 用户状态 = 封禁 WHERE 用户ID = " + user_id;
    cout << "对应SQL命令为: " << sql_cmd << endl;
    m_sql_helper.sql_analyse(sql_cmd);
    if (m_sql_helper.next == 6) { //需要继续下架
        m_sql_helper.next = 0; //刷新
        sql_cmd = "UPDATE commodity SET 商品状态 = 已下架 WHERE 卖家ID = " + user_id;
        cout << "对应SQL命令为: " << sql_cmd << endl;
        m_sql_helper.sql_analyse(sql_cmd);
    }
}

bool Administrator::check_pass() const {
    string name, pass;
    cout << "请输入管理员姓名(默认admin): ";
    cin.sync();
    cin >> name;
    while (name != admin_name) {
        cout << "管理员姓名默认admin，请重新输入: ";
        cin.sync();
        cin >> name;
    }
    cin.sync();
    cout << "请输入密码: ";
    cin >> pass;
    if (pass == admin_pass) {
        cout << endl << "-----登录成功-----" << endl << endl;
        return true;
    }
    cout << endl << "----密码错误，登录失败，返回主菜单----" << endl << endl;
    return false;
}
