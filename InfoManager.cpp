//
// Created by Administrator on 2022/2/21.
//

#include "InfoManager.h"
#include "common.h"
#include <iostream>
#include <set>

using namespace std;

void InfoManager::display_cmd() {
    cout << "=======================================" << endl;
    cout << "1.查看信息 2.修改信息 3.充值 4.返回用户主界面" << endl;
    cout << "=======================================" << endl;
    cout << endl << "输入操作: " ;
}

InfoManager::InfoManager(Users &user) {
    m_user = user;
}

void InfoManager::display_my_info() {

}

//TODO: 和注册用户那里一样，待补对于信息格式的判断
//更新个人信息，思路是先读用户文件得到用户名池，
//根据输入创建新的m_user，写临时文件时如果ID符号要改则<<m_user，其余直接<< line
void InfoManager::update_my_info() {
    set<string> name_pool; //用户表中已经存在的用户名池
    ifstream fin(user_file);
    if (!fin) {
        cout << "Error: open file failed! " << user_file;
        return;
    }
    string line;
    getline(fin, line); //第一行表头
    while (getline(fin, line)) {
        vector<string> each;
        my_split(line, ',', each);
        Users tmp(each);
        name_pool.insert(tmp.m_name);
    }
    fin.close(); //得到用户名池

    string choose;
    cout << "请选择修改的属性(1.用户名  2.联系方式  3.地址): " ;
    cin >> choose;

    if(choose == "1")
    {
        string name;
        while(true) {
            while (true) {
                cout << "请输入修改后的用户名(不超过10个字符): ";
                cin >> name;
                cout << "这个长度：" << name.length() << endl;
                if(name.length() > 10) {
                    cout << "用户名不能超过10个字符!" << endl;
                    continue;
                }else
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
        m_user.m_name = name;
    }
    else if(choose == "2")
    {
        string tel;
        while (true){
            cout << "请输入联系方式(不超过20个字符): ";
            cin >> tel;
            if(tel.length() > 20) {
                cout << "联系方式不能超过20个字符!" << endl;
                continue;
            } else{
                break;
            }
        }
        m_user.m_tel = tel;
    }
    else if(choose == "3")
    {
        string addr;
        while(true){
            cout << "请输入地址(不超过20个字符): ";
            cin >> addr;
            if(addr.length() > 20){
                cout << "地址不能超过20个字符!" << endl;
                continue;
            } else{
                break;
            }
        }
        m_user.m_addr = addr;
    }
    else //异常输入
    {
        cout << "没有这个功能，请输入上述展示功能对应的数字!" << endl;
        return ;
    }

    vector<Users> res; //这里的Users用的是class类
    fin.open(user_file);
    ofstream fout("tmp_user_file.txt");
    if (!fin || !fout) {
        cout << "Error: open file failed! " << user_file;
        return;
    }
    getline(fin, line); //第一行表头
    fout << line << endl; //表头别忘了写到临时文件里！
    while (getline(fin, line)) {
        vector<string> each;
        my_split(line, ',', each); //用,分隔user_file文件中的每一行填充each
        Users tmp(each); //得到当前这行代表的用户tmp
        if(tmp.m_id == m_user.m_id) //原本的行需要修改成m_user的样子
        {
            fout << m_user;
        }
        else //如果不是这一行直接写回文件
        {
            fout << line << endl;
        }
    }
    //重命名覆盖
    fin.close();
    fout.close();

    const char* oldname = "tmp_user_file.txt";
    const char* newname = user_file.c_str();
    rm_rename(newname, oldname);

    cout << "修改成功！" << endl;

}
