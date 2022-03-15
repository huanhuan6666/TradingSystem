//
// Created by Administrator on 2022/2/21.
//

#include "Sellers.h"
#include <iostream>
#include <iomanip>
#include <set>
using namespace std;

void Sellers::display_cmd(){
    cout << "=======================================================================" << endl;
    cout << "1.发布商品 2.查看发布商品 3.修改商品信息 4.下架商品 5.查看历史订单 6.返回用户主界面" << endl;
    cout << "=======================================================================" << endl;
    cout << endl << "输入操作: " ;

}

Sellers::Sellers() {
    m_sql_helper.user_status = STATUS_SELLER;
}

Sellers::Sellers(Users &u) {
    m_id    =  u.m_id   ;
    m_name  =  u.m_name ;
    m_pass  =  u.m_pass ;
    m_tel   =  u.m_tel  ;
    m_addr  =  u.m_addr ;
    m_money =  u.m_money;
    m_state =  u.m_state;
    m_sql_helper.user_id = m_id;
    m_sql_helper.user_status = STATUS_SELLER;
}

void Sellers::release_good() {
    //INSERT INTO name VALUES (value1,value2,···,valueT)
    string name, price, count, des;
    //商品名称是可以重复的
    while (true) {
        cout << "请输入商品名称(英文字母不超过20个): ";
        cin.sync();
        cin >> name;
        if(name.length() > 20) {
            cout << "商品名称不能超过20个字符!";
            cout << "这个长度：" << name.length() << endl;
            continue;
        }
        bool flag = false;
        for(auto c : name){
            if(!isalpha(c)){
                cout << "商品名称只能出现英文字母！";
                cout << "出错字符: " << c << endl;
                flag = true;
                break;
            }
        }
        if(flag)
            continue;
        break;
    }

    float p = 0;
    while(true){
        cout << "请输入商品价格(保留一位小数): ";
        cin.sync();
        getline(cin, price);
        int tmp_count = 0;
        bool flag = false;
        for (auto& c : price) {
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
        catch (invalid_argument&) {
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

    int q = 0;
    while(true){
        cout << "请输入商品数量(正整数): ";
        cin.sync();
        getline(cin, count);
        if(count.empty()){
            cout << "输入不能为空！" << endl;
            continue;
        }
        bool flag = false;
        for(auto &c:count){
            if(!isdigit(c) && c!=' ' || c=='.'){ //有小数点就报错
                cout << "请输入正确的正整数！" << endl;
                flag = true;
                break;
            }
        }
        if(flag){
            continue;
        }
        try {
            q = stoi(count); //用串IO保留一位小数
            break;
        }
        catch (invalid_argument&) {
            cout << "请输入正确的正整数！" << endl;
        }
        catch (...) {
            cout << "其他异常！" << endl;
        }
    }
    count = to_string(q); //获取正整数的字符串

    while(true) {
        cout << "请输入商品描述(英文字母不超过200个): ";
//        cin >> des;
        cin.sync(); //清空缓冲区
        getline(cin, des);
        if(des.length() == 0){
            cout << "商品描述不能为空！" << endl;
            continue;
        }
        if(des.length() > 200) {
            cout << "商品描述不能超过200个字符!";
            cout << "这个长度：" << des.length() << endl;
            continue;
        }
        bool flag = false;
        for(auto& c: des){
            if(!isalpha(c) && c != ' '){
                cout << "商品描述只能出现英文字符！";
                cout << "出错字符: " << c << endl;
                flag = true;
                break;
            }
            if(c == ' ') //这里决定把空格替换成下划线 变相支持空格嘻嘻
            {
                c = '_';
            }
        }
        if(flag)
            continue;
        break;
    }
    string sql_cmd = "INSERT INTO commodity VALUES (" + name + "," + price + "," + count + "," + des + ")";
    cout << "对应SQL命令为: " << sql_cmd << endl;
    m_sql_helper.sql_analyse(sql_cmd);

}

//修改商品信息
void Sellers::update_my_good() {
    string sql_cmd, com_id;
    cout << "请输入被修改商品的ID: ";
    cin.sync();
    cin >> com_id;
    string tmp;
    cout << "请输入被修改商品的属性(1.价格 2.描述): ";
    cin.sync();
    cin >> tmp;
    if(tmp != "1" && tmp != "2")
    {
        cout << "没有这个功能，请输入上述展示功能对应的数字!" << endl;
        return;
    }
    if(tmp == "1")
    {
        string price;
        float p = 0;
        while(true) {
            cout << "请输入被修改商品的价格(保留一位小数): ";
            cin.sync();
            getline(cin, price);
            int tmp_count = 0;
            bool flag = false;
            for (auto& c : price) {
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
            catch (invalid_argument&) {
                cout << "请输入正确的浮点数！" << endl;
            }
            catch (...) {
                cout << "其他异常！" << endl;
            }
        }
        ostringstream sout;
        sout << setiosflags(ios::fixed);
        sout << setprecision(1) << p;
        sql_cmd = "UPDATE commodity SET 价格 = " + sout.str() + " WHERE 商品ID = " + com_id;
    }
    else if(tmp == "2")//tmp == 2
    {
        string des;
        while(true) {
            cout << "请输入被修改商品的描述(英文字母不超过200个): ";
//            cin >> des;
            cin.sync(); //清空缓冲区
            getline(cin, des);
            if(des.length() == 0){
                cout << "商品描述不能为空！" << endl;
                continue;
            }
            if(des.length() > 200) {
                cout << "商品描述不能超过200个字符!" << "这个长度: " << des.length() << endl;
                continue;
            }
            bool flag = false;
            for(auto& c: des){
                if(!isalpha(c) && c != ' '){
                    cout << "商品描述只能出现英文字符！" << "出错字符: " << c << endl;
                    flag = true;
                    break;
                }
                if(c == ' ') //这里决定把空格替换成下划线 变相支持空格嘻嘻
                {
                    c = '_';
                }
            }
            if(flag)
                continue;
            break;
        }
        sql_cmd = "UPDATE commodity SET 描述 = " + des + " WHERE 商品ID = " + com_id;
    }
    else //异常输入
    {
        cout << "没有这个功能，请输入上述展示功能对应的数字!" << endl;
        return ;
    }
    cout << "对应SQL命令为: " << sql_cmd << endl;
    m_sql_helper.sql_analyse(sql_cmd);
}

void Sellers::off_my_shelf() {
    string com_id;
    cout << "请输入要下架的商品ID: ";
    cin.sync();
    cin >> com_id;
    string sql_cmd = "UPDATE commodity SET 商品状态 = 已下架 WHERE 商品ID = " + com_id;
    cout << "对应SQL命令为: " << sql_cmd << endl;
    m_sql_helper.sql_analyse(sql_cmd);
}

