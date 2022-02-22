//
// Created by Administrator on 2022/2/21.
//

#include "InfoManager.h"
#include "common.h"
#include <iostream>

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

//更新个人信息，思路是得到此ID的一个修改后的User对象m_user
//然后找到对应行向文件直接<<这个新的对象即可
void InfoManager::update_my_info() {
    string choose;
    cout << "请选择修改的属性(1.用户名  2.联系方式  3.地址): " ;
    cin >> choose;
    if(choose == "1")
    {
        string name;
        cout << "请输入修改后的用户名: ";
        cin >> name;
        //TODO: 用户名不能重复！
    }
    else if(choose == "2")
    {
        string tel;
        cout << "请输入修改后的联系方式: ";
        cin >> tel;
        m_user.m_tel = tel; //生成一个修改后的m_user
    }
    else if(choose == "3")
    {
        string addr;
        cout << "请输入修改后的地址: ";
        cin >> addr;
        m_user.m_addr = addr;
    }
    else //异常输入
    {
        cout << "没有这个功能，请输入上述展示功能对应的数字!" << endl;
        return ;
    }

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
}
