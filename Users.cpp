//
// Created by Administrator on 2022/2/21.
//

#include "Users.h"
#include <iostream>
#include <set>
#include "common.h"
using namespace std;

Users::Users(vector<string> &each) {
    m_id    =  each[0];
    m_name  =  each[1];
    m_pass  =  each[2];
    m_tel   =  each[3];
    m_addr  =  each[4];
    m_money =  stof(each[5]);
    m_state =  each[6];
    m_sql_helper.user_id = m_id; //向SQL解析器转入自己的ID
}

//调用这个成员函数的User将会进行登录检测，成功登录的话该User对象的属性将全部按照文件内容填写
//即真正代表某个确定的用户
bool Users::check_pass() {
    string name, pass, line;
    cout << "请输入用户名: ";
    cin.sync();
    cin >> name;
    ifstream fin(user_file);
    if(!fin)
    {
        cout << "Error: open file failed! " << user_file << endl;
        return false;
    }
    getline(fin, line); //第一行表头
    while(getline(fin, line))
    {
            vector<string> each;
            my_split(line, ',', each);
            Users tmp(each);
        if(tmp.m_name == name) //用户名存在
        {
            if(tmp.m_state == "封禁")
            {
                cout << "当前用户已被封禁，无法登录" << endl;
                fin.close();
                return false;
            }
            cout << "请输入密码: ";
            cin.sync();
            cin >> pass;
            if(pass == tmp.m_pass)
            {
                cout << "-----登录成功-----" << endl;
                *this = tmp; //!!!填写用户的全部信息，得到独一无二的用户实例
                fin.close();
                return true;
            }
            else
            {
                cout << "-----密码错误!-----" << endl;
                fin.close();
                return false;
            }
        }
        else
            continue;
    }
    cout << "用户表中不存在此用户名" << endl;
    fin.close();
    return false;

}

void Users::display_cmd() {
    cout << "=================================================" << endl;
    cout << "1.注销登录 2.我是买家 3.我是卖家 4.个人信息管理 5.聊天室" << endl;
    cout << "=================================================" << endl;
    cout << endl << "输入操作: " ;

}

ostream &operator<<(ostream& out, Users &u)
{
    out << u.m_id << ',' << u.m_name << ',' << u.m_pass << ',' << u.m_tel << ','
        << u.m_addr << ',' << u.m_money << ',' << u.m_state << endl;
    return out;
}

Users &Users::operator=(const Users &tmp) {
    m_id    =  tmp.m_id   ;
    m_name  =  tmp.m_name ;
    m_pass  =  tmp.m_pass ;
    m_tel   =  tmp.m_tel  ;
    m_addr  =  tmp.m_addr ;
    m_money =  tmp.m_money;
    m_state =  tmp.m_state;
    m_sql_helper.user_id = m_id;
    return *this;
}

//卖家查看自己作为卖家的订单
//买家查看自己作为买家的订单
void Users::display_my_order() {
    string sql_cmd = "SELECT * FROM order";
    cout << "对应SQL命令为: " << sql_cmd << endl;
    m_sql_helper.sql_analyse(sql_cmd);
}

//卖家是查看自己ID发布的商品
//买家则是看所有销售中的商品
void Users::display_my_good() {
    string sql_cmd = "SELECT * FROM commodity";
    cout << "对应SQL命令为: " << sql_cmd << endl;
    m_sql_helper.sql_analyse(sql_cmd);
}

//TODO: 对于属于信息的格式判断: 比如只能出现数字和字母以及'汉字算一个字符'，准备用正则表达式实现
//用户注册功能，这部分在课件说不需要生成SQL指令，就按正常流程走了
void Users::sign_up() {
    //得到用户ID池和用户名池，用于之后的ID生成和用户名重复判断
    ifstream fin(user_file);
    string line;
    set<string> id_pool, name_pool; //用户表中已经存在的用户ID池和用户名池
    if (!fin) {
        cout << "Error: open file failed! " << user_file;
        return;
    }

    getline(fin, line); //第一行表头
    while (getline(fin, line)) {
        vector<string> each;
        my_split(line, ',', each);
        Users tmp(each); //得到当前这行代表的用户tmp
        id_pool.insert(tmp.m_id);
        name_pool.insert(tmp.m_name);
    }
    fin.close();

    string name, pass, tel, addr, money;
    while(true) {
        while (true) {
            cout << "请输入新用户名(英文字母不超过10个): ";
            cin.sync();
            cin >> name;
            if(name.length() > 10) {
                cout << "用户名不能超过10个字符!";
                cout << "这个长度：" << name.length() << endl;
                continue;
            }
            bool flag = false;
            for(auto c:name){
                if(!isalpha(c)){
                    cout << "用户名中只能出现英文字母！" << "出错字符: " << c << endl;
                    flag = true;
                    break;
                }
            }
            if(flag)
                continue;
            break;
        }
        //查看用户名是否重复
        if (name_pool.count(name) == 1) //用户名存在
        {
            cout << "当前用户名已经存在，请重新输入！" << endl;
            continue;
        }
        else {
            break;
        }
    }

    while (true){
        cout << "请输入用户密码(小写字母和数字不超过20个): ";
        cin.sync();
        cin >> pass;
        if(pass.length() > 20) {
            cout << "密码不能超过20个字符! ";
            cout << "这个长度: " << pass.length() << endl;
            continue;
        }
        bool flag = false;
        for(auto c:pass){
            if(!isdigit(c) && !islower(c)){
                cout << "密码中只能出现小写字母和数字！" << "出错字符: " << c << endl;
                flag = true;
                break;
            }
        }
        if(flag)
            continue;
        break;

    }

    while (true){
        cout << "请输入联系方式(数字不超过20个): ";
        cin.sync();
        cin >> tel;
        if(tel.length() > 20) {
            cout << "联系方式不能超过20个字符!" << "这个长度: " << tel.length() << endl;
            continue;
        }
        bool flag = false;
        for(auto n : tel){
            if(!isdigit(n)){
                cout << "联系方式中只能出现数字！" << "出错字符: " << n << endl;
                flag = true;
                break;
            }
        }
        if(flag)
            continue;
        break;

    }

    while(true){
        cout << "请输入地址(英文字母不超过40个): ";
        cin.sync();
        cin >> addr;
        if(addr.length() > 40){
            cout << "地址不能超过40个字符!" << "这个长度: " << addr.length() << endl;
            continue;
        }
        bool flag = false;
        for(auto c:addr){
            if(!isalpha(c)){
                cout << "地址中只能出现英文字母!" << "出错字符: " << c << endl;
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
        cout << "请输入首充金额(保留一位小数): ";
        //TODO: 这里之后可以来个首充优惠啥的 还有什么充值方式之类的
        cin.sync();
        getline(cin, money);
        int tmp_count = 0;
        bool flag = false;
        for (auto& c : money) {
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
            p = stof(money); //用串IO保留一位小数
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
    money = sout.str();  //保留一位小数

    //取id池中最大id + 1生成新的id, 用串IO实现3位编号补零
    ostringstream idout;
    string new_id;
    if(id_pool.empty()) { //一个用户都没有需要生成U000
        new_id = "U001";
    }else { //有用户的话则按顺序生成
        idout << setw(3) << setfill('0') << stoi((*id_pool.rbegin()).substr(1, 3)) + 1;
        new_id = "U" + idout.str();
    }
    //填写对象属性则新用户创建完成！
    m_id    =   new_id;
    m_name  =   name;
    m_pass  =   pass;
    m_tel   =   tel;
    m_addr  =   addr;
    m_money =   stof(money);
    m_state =   "正常";

    ofstream fout(user_file, ios::app);
    if (!fout) {
        cout << "Error: open file failed! " << user_file;
        return;
    }
    fout << *this; //在文件尾部符加本新用户
    fout.close();

    //充值记录填写
    ofstream re_out(recharge_file, ios::app);
    if(!re_out) {
        cout << "Error: open file failed!" << recharge_file << endl;
        return ;
    }
    //格式为 用户id,充值金额,充值时间
    re_out << m_id + "," << money << "," + get_curtime() << endl;
    re_out.close();

    cout << "用户创建成功!" << endl;

}
