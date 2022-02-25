//
// Created by Administrator on 2022/2/21.
//

#ifndef PROJECT1_INFOMANAGER_H
#define PROJECT1_INFOMANAGER_H
#include "Users.h"
#include "Calculator.h"

// 在用户界面的 “个人信息管理” 功能用到，为了结构的一致性索性把这个界面也封装成类
// 当然内部包含计算器的实现
class InfoManager {
public:
    Users m_user; //信息管理器需要传入使用它的User
    Calculator m_cal; //计算器！
public:
    explicit InfoManager(Users &user);
    static void display_cmd();  //显示信息管理功能页面
    void display_my_info(); //查看个人信息
    void update_my_info();  //修改个人信息
    void recharge() const;    //充值
    void show_calcu();  //计算器
};


#endif //PROJECT1_INFOMANAGER_H
