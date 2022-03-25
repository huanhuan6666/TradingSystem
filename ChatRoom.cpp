//
// Created by Administrator on 2022/3/25.
//

#include "ChatRoom.h"
#include <iostream>
#include <set>
#include "common.h"

using namespace std;

void ChatRoom::display_cmd() {
    cout << "=========================================================================" << endl;
    cout << "1.查看已发消息 2.查看收到的消息 3.发送消息 4.撤回消息 5.展示聊天记录 6.返回用户主界面" << endl;
    cout << "=========================================================================" << endl;
    cout << endl << "输入操作: ";
}

//向指定用户ID发送消息 不能给自己发！需添加聊天记录状态为未读！
void ChatRoom::send_message() {
    //需要给一个存在的ID发消息
    set<string> u_id; //存在的用户
    ifstream u_fin(user_file);
    if (!u_fin) {
        cout << "Error: open file failed! " << endl;
        return;
    }
    string line;
    getline(u_fin, line); //第一行表头
    while (getline(u_fin, line)) {
        vector<string> each;
        my_split(line, ',', each);
        Chat_t tmp(each);
        u_id.insert(tmp.c_id);
    }
    u_fin.close(); //得到用户ID池

    string recv_id; //接收者ID
    while(true){
        cout << "请输入接收者用户ID: ";
        getline(cin, recv_id);
        if(recv_id.empty()){
            cout << "输入不能为空!" << endl;
            continue;
        }
        if(recv_id == user_id){
            cout << "不能给自己发消息！" << endl;
            continue;
        }
        if(u_id.count(recv_id) == 0){
            cout << "该用户不存在！" << endl;
            return;
        }else //用户ID存在
            break;
    }
    string context;
    while(true){
        cout << "请输入消息: ";
        getline(cin, context);
        if(context.empty()){
            cout << "输入不能为空!" << endl;
            continue;
        }
        if(context.size() > 100){
            cout << "消息长度不能超过100个字节！" << endl;
            continue;
        }
        break;
    }

    //首先给消息获取一个新ID
    set<string> id_pool; //消息已存在的ID池
    ifstream fin(chat_file);
    if (!fin) {
        cout << "Error: open file failed! ";
        return;
    }
    getline(fin, line); //第一行表头
    while (getline(fin, line)) {
        vector<string> each;
        my_split(line, '|', each);
        Chat_t tmp(each);
        id_pool.insert(tmp.c_id);
    }
    fin.close(); //得到消息ID池

    //取id池中最大id + 1生成新的id, 用串IO实现3位编号补零
    ostringstream idout;
    string new_id;
    if(id_pool.empty()){ //一条消息都没有则直接X001
        new_id = "X001";
    }else {
        idout << setw(3) << setfill('0') << stoi((*id_pool.rbegin()).substr(1, 3)) + 1;
        new_id = "X" + idout.str();
    }
    //获取消息时间
    string cur_time(get_curtime());
    //生成新消息结构体
    vector<string> each_value;
    each_value.push_back(new_id);
    each_value.push_back(user_id);
    each_value.push_back(recv_id);
    each_value.push_back(context);
    each_value.push_back(cur_time);
    each_value.emplace_back("未读");
    Chat_t new_cht(each_value);

    //将新消息符加到消息文件中
    ofstream fout(chat_file, ios::app);
    if (!fout) {
        cout << "Error: open file failed! ";
        return;
    }
    fout << new_cht;
    fout.close();
    cout << "***************" << endl;
    cout << "消息已经成功发送！" << endl;
    cout << "***************" << endl;
}

void cout_tmp(const Chat_t &tmp){
    cout << left << setw(10) << tmp.c_id << left << setw(10) << tmp.sender << left << setw(10)
         << tmp.receiver
         << left << setw(25) << tmp.c_time << left << setw(10) << tmp.c_state << tmp.context << endl;
}

//查看已发送的消息
void ChatRoom::display_send() {
    vector<Chat_t> res;
    ifstream fin(chat_file);
    if (!fin) {
        cout << "Error: open file failed! " << user_file;
        return;
    }
    string line;
    getline(fin, line); //第一行表头
    while (getline(fin, line)) {
        vector<string> each;
        my_split(line, '|', each); //用|分隔聊天记录
        Chat_t tmp(each);
        if (tmp.sender == user_id) //只查看我发送的消息
            res.push_back(tmp); //添加消息到res中
    }
    if (res.empty()) {
        cout << "**************************" << endl;
        cout << "您没有发送过消息！返回初始界面" << endl;
        cout << "**************************" << endl;
        fin.close();
        return;
    }
    cout << "您发过的所有消息如下: " << endl;
    cout << endl << "********************************************************" << endl;
    cout << "消息ID    发送者ID   接收者ID      发送时间                状态      发送内容" << endl;
    for (const auto &cht: res) {
        cout_tmp(cht);
    }
    cout << "********************************************************" << endl;
}

//查看收到的消息
void ChatRoom::display_recv() {
    string choose;
    while (true) {
        cout << "请输入查看范围(1.只看未读 2.查看所有): ";
        getline(cin, choose);
        if (choose.empty()) {
            cout << "选择不能为空!" << endl;
            continue;
        }
        if (choose != "1" && choose != "2") {
            cout << "只能选择1 或 2！" << endl;
            continue;
        }
        break;
    }

    vector<Chat_t> res;
    ifstream fin(chat_file);
    ofstream fout("tmp_chat_file.txt");
    if (!fin || !fout) {
        cout << "Error: open file failed! " << endl;
        return;
    }
    string line;
    getline(fin, line); //第一行表头
    fout << line << endl;

    while (getline(fin, line)) {
        vector<string> each;
        my_split(line, '|', each); //用|分隔聊天记录
        Chat_t tmp(each);
        if (tmp.receiver == user_id) {//只查看发给我的消息
            if (choose == "1") {//只看未读
                if (tmp.c_state == "未读") {
                    res.push_back(tmp); //添加消息到res中
                    tmp.c_state = "已读";
                }
            } else {//查看所有
                res.push_back(tmp);
                if (tmp.c_state == "未读")
                    tmp.c_state = "已读";
            }
        }
        fout << tmp;
    }
    if (res.empty()) {
        if (choose == "1") { //未读消息
            cout << "************************" << endl;
            cout << "您没有未读消息！返回初始界面" << endl;
            cout << "************************" << endl;
        } else { //所有消息
            cout << "*************************" << endl;
            cout << "您没有收到过消息！返回初始界面" << endl;
            cout << "*************************" << endl;
        }
        goto END;
    }
    cout << endl << "********************************************************" << endl;
    cout << "消息ID    发送者ID   接收者ID      发送时间                状态      发送内容" << endl;
    for (const auto &cht: res) {
        cout_tmp(cht);
    }
    cout << "********************************************************" << endl;
END:
    //重命名覆盖
    fin.close();
    fout.close();

    const char *oldname = "tmp_chat_file.txt";
    const char *newname = chat_file.c_str();
    rm_rename(newname, oldname);
}

void ChatRoom::widthdraw() {
    string del_id;
    while (true) {
        cout << "请输入要撤回的消息ID: ";
        getline(cin, del_id);
        if (del_id.empty()) {
            cout << "输入不能为空！" << endl;
            continue;
        }
        break;
    }

    vector<Chat_t> res;
    ifstream fin(chat_file);
    ofstream fout("tmp_chat_file.txt");
    if (!fin || !fout) {
        cout << "Error: open file failed! " << endl;
        return;
    }
    bool flag = false; //这个消息是否是我发的
    string line;
    getline(fin, line); //第一行表头
    fout << line << endl;

    while (getline(fin, line)) {
        vector<string> each;
        my_split(line, '|', each); //用|分隔聊天记录
        Chat_t tmp(each);
        if (del_id == tmp.c_id) { //要删除
            if (tmp.sender == user_id) { //确实是我发的
                flag = true; //存在这条消息
                if (tmp.c_state == "未读") { //未读可以撤回
                    string ch;
                    cout << "查询到您发送的该消息信息如下，确认撤回？" << endl;
                    cout << "***************************************************************************" << endl;
                    cout << "消息ID    发送者ID   接收者ID      发送时间                状态      发送内容" << endl;
                    cout_tmp(tmp);
                    cout << "***************************************************************************" << endl;

                    while (true) {
                        cout << "请选择(y/n), 输入非y代表放弃本次操作: ";
                        getline(cin, ch);
                        if (ch.empty()) {
                            cout << "输入不能为空！" << endl;
                            continue;
                        }
                        if (ch != "y") {
                            cout << "操作已放弃！" << endl;
                        }
                        break;
                    }
                    if (ch == "y") { //确认撤回
                        //直接删除
                        cout << "****************" << endl;
                        cout << "消息已经成功撤回！" << endl;
                        cout << "****************" << endl;
                    } else //不撤回还得写
                        fout << tmp;
                } else { //消息已经被读取了不能撤回
                    cout << "************************" << endl;
                    cout << "该消息已经被读取，无法撤回！" << endl;
                    cout << "************************" << endl;
                    fout << tmp;
                }
            }
        } else //不用删的直接写
            fout << tmp;
    }

    if (!flag) { //根本没有这条消息
        cout << "***********************************" << endl;
        cout << "您没有发送该ID的消息，可按 1 查看已发消息" << endl;
        cout << "***********************************" << endl;
    }

    //重命名覆盖
    fin.close();
    fout.close();

    const char *oldname = "tmp_chat_file.txt";
    const char *newname = chat_file.c_str();
    rm_rename(newname, oldname);
}

//展示和某用户的聊天记录
//不会修改消息的状态  该未读还是未读  这不是bug！
void ChatRoom::display_log() {
    string recv_id;
    while(true){
        cout << "请输入对方的用户ID: ";
        getline(cin, recv_id);
        if(recv_id.empty()){
            cout << "输入不能为空！" << endl;
            continue;
        }
        break;
    }

    vector<Chat_t> res;
    ifstream fin(chat_file);
    if (!fin) {
        cout << "Error: open file failed! " << endl;
        return;
    }
    string line;
    getline(fin, line); //第一行表头

    while (getline(fin, line)) {
        vector<string> each;
        my_split(line, '|', each); //用|分隔聊天记录
        Chat_t tmp(each);
        if (tmp.sender == user_id && tmp.receiver == recv_id ||
            tmp.sender == recv_id && tmp.receiver == user_id) { //是双方的聊天记录
            res.push_back(tmp);
        }
    }
    fin.close();

    if (res.empty()) {
        cout << "*************************" << endl;
        cout << "没有相关聊天记录！返回初始界面" << endl;
        cout << "*************************" << endl;
        return;
    }
    cout << "查询到聊天记录如下: " << endl;
    cout << "=========================================================================" << endl;
    for (const auto &cht: res) {
        if(cht.sender == user_id){ //我发的消息
            cout << "←[" << cht.c_time << "]" << endl;
            cout << "←" << cht.context << endl << endl;
        }
        if(cht.receiver == user_id){ //我收到的消息
            cout << "→[" << cht.c_time << "]" << endl;
            cout << "→" << cht.context << endl << endl;
        }
    }
    cout << "=========================================================================" << endl;
}

