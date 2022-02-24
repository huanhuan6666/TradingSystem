//
// Created by Administrator on 2022/2/21.
//

#include "MainWindow.h"
#include "common.h"
#include <iostream>
#include <fstream>
using namespace std;

void MainWindow::cmd_display() {
    cout << "========================================" << endl;
    cout << "1.管理员登录 2.用户注册 3.用户登录 4.退出程序" << endl;
    cout << "========================================" << endl;
    cout << endl << "输入操作: " ;

}

void MainWindow::register_user() {

}

void MainWindow::admin_login() {

}

void MainWindow::user_login() {

}

void MainWindow::exit_program() {

}

MainWindow::MainWindow() { //程序启动创建UI的时候就检测各种文件，没有则创建并写入相应信息
    ifstream in_file;
    ofstream out_file;
    //初始化订单文件
    in_file.open(order_file);
    if(!in_file)
    {
        out_file.open(order_file);
        out_file << "订单ID,商品ID,交易单价,数量,交易时间,卖家ID,买家ID" << endl;
        out_file.close();
    }
    in_file.close();
    //初始化命令文件
    in_file.open(commands_file);
    if(!in_file)
    {
        out_file.open(commands_file);
        out_file.close();
    }
    in_file.close();
    //初始化商品文件
    in_file.open(commodity_file);
    if(!in_file)
    {
        out_file.open(commodity_file);
        out_file << "商品ID,名称,价格,数量,描述,卖家ID,上架时间,商品状态" << endl;
        out_file.close();
    }
    in_file.close();
    //初始化用户文件
    in_file.open(user_file);
    if(!in_file)
    {
        out_file.open(user_file);
        out_file << "用户ID,用户名,密码,联系方式,地址,钱包余额,用户状态" << endl;
        out_file.close();
    }
    in_file.close();
    //初始化充值文件
    in_file.open(recharge_file);
    if(!in_file)
    {
        out_file.open(recharge_file);
        out_file << "用户ID,充值金额,充值时间" << endl;
        out_file.close();
    }
    in_file.close();
}
