//
// Created by Administrator on 2022/3/25.
//

#ifndef PROJECT1_CHATROOM_H
#define PROJECT1_CHATROOM_H
#include <iostream>

using namespace std;


class ChatRoom {
public:
    explicit ChatRoom(string u_id): user_id(u_id) { ; }

    string user_id; //当前聊天的用户ID
    void display_cmd();
    void send_message(); //发消息
    void display_send(); //查看自己已经发送的消息 -> 不改变消息状态
    void display_recv(); //查看收到的消息 ->将状态从已发送改成已读
    void widthdraw(); //撤回消息 ->只能撤回自己发送且状态为已发送的消息
    void display_log(); //展示聊天记录
};


#endif //PROJECT1_CHATROOM_H
