//
// Created by Administrator on 2022/2/21.
//

#include "MainWindow.h"
#include "common.h"
#include <iostream>
#include <fstream>

using namespace std;

void MainWindow::cmd_display() {
    cout << "===============================================" << endl;
    cout << "1.管理员登录 2.用户注册 3.用户登录 4.退出程序 5.计算器" << endl;
    cout << "===============================================" << endl;
    cout << endl << "输入操作: ";

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
    if (!in_file) {
        out_file.open(order_file);
        out_file << "订单ID,商品ID,交易单价,数量,交易时间,卖家ID,买家ID" << endl;
        out_file.close();
    }
    in_file.close();
    //初始化命令文件
    in_file.open(commands_file);
    if (!in_file) {
        out_file.open(commands_file);
        out_file.close();
    }
    in_file.close();
    //初始化商品文件
    in_file.open(commodity_file);
    if (!in_file) {
        out_file.open(commodity_file);
        out_file << "商品ID,名称,价格,数量,描述,卖家ID,上架时间,商品状态" << endl;
        out_file.close();
    }
    in_file.close();
    //初始化用户文件
    in_file.open(user_file);
    if (!in_file) {
        out_file.open(user_file);
        out_file << "用户ID,用户名,密码,联系方式,地址,钱包余额,用户状态" << endl;
        out_file.close();
    }
    in_file.close();
    //初始化充值文件
    in_file.open(recharge_file);
    if (!in_file) {
        out_file.open(recharge_file);
        out_file << "用户ID,充值金额,充值时间" << endl;
        out_file.close();
    }
    in_file.close();
    //初始化聊天日志文件
    in_file.open(chat_file);
    if (!in_file) {
        out_file.open(chat_file);
        out_file << "消息ID,发送者ID,接收者ID,发送时间,状态" << endl;
        out_file.close();
    }
}

void MainWindow::begin_show() {
    string start_1[17];
    start_1[0] = "HHHHHHHHH     HHHHHHHHH                   lllllll lllllll                   !!!";
    start_1[1] = "H:::::::H     H:::::::H                   l:::::l l:::::l                  !!:!!";
    start_1[2] = "H:::::::H     H:::::::H                   l:::::l l:::::l                  !:::!";
    start_1[3] = "HH::::::H     H::::::HH                   l:::::l l:::::l                  !:::!";
    start_1[4] = "  H:::::H     H:::::H      eeeeeeeeeeee    l::::l  l::::l    ooooooooooo   !:::!";
    start_1[5] = "  H:::::H     H:::::H    ee::::::::::::ee  l::::l  l::::l  oo:::::::::::oo !:::!";
    start_1[6] = "  H::::::HHHHH::::::H   e::::::eeeee:::::eel::::l  l::::l o:::::::::::::::o!:::!";
    start_1[7] = "  H:::::::::::::::::H  e::::::e     e:::::el::::l  l::::l o:::::ooooo:::::o!:::!";
    start_1[8] = "  H:::::::::::::::::H  e:::::::eeeee::::::el::::l  l::::l o::::o     o::::o!:::!";
    start_1[9] = "  H::::::HHHHH::::::H  e:::::::::::::::::e l::::l  l::::l o::::o     o::::o!:::!";
    start_1[10] = "  H:::::H     H:::::H  e::::::eeeeeeeeeee  l::::l  l::::l o::::o     o::::o!!:!!";
    start_1[11] = "  H:::::H     H:::::H  e:::::::e           l::::l  l::::l o::::o     o::::o !!!";
    start_1[12] = "HH::::::H     H::::::HHe::::::::e         l::::::ll::::::lo:::::ooooo:::::o";
    start_1[13] = "H:::::::H     H:::::::H e::::::::eeeeeeee l::::::ll::::::lo:::::::::::::::o !!!";
    start_1[14] = "H:::::: H     H:::::::H  ee:::::::::::::e l::::::ll::::::l oo:::::::::::oo !!:!!";
    start_1[15] = "HHHHHHHHH     HHHHHHHHH    eeeeeeeeeeeeee llllllllllllllll   ooooooooooo    !!!";
    for (int i = 0; i < 16; i++) {
        cout << start_1[i] << endl;
        _sleep(300);
    }
    cout << "================================初始化完成！！！=================================" << endl;
    cout << "即将进入系统" << flush;
    for(int i = 0; i < 6; i++){
        cout << "." << flush;
        _sleep(300);
    }
    cout << endl;
//    system("clear");
}

void MainWindow::end_show() {
//    system("clear");
    string end[8];
    end[0] = ":'######::::'#######:::'#######::'########::'########::'##:::'##:'########:";
    end[1] = "'##... ##::'##.... ##:'##.... ##: ##.... ##: ##.... ##:. ##:'##:: ##.....::";
    end[2] = " ##:::..::: ##:::: ##: ##:::: ##: ##:::: ##: ##:::: ##::. ####::: ##:::::::";
    end[3] = " ##::'####: ##:::: ##: ##:::: ##: ##:::: ##: ########::::. ##:::: ######:::";
    end[4] = " ##::: ##:: ##:::: ##: ##:::: ##: ##:::: ##: ##.... ##:::: ##:::: ##...::::";
    end[5] = " ##::: ##:: ##:::: ##: ##:::: ##: ##:::: ##: ##:::: ##:::: ##:::: ##:::::::";
    end[6] = ". ######:::. #######::. #######:: ########:: ########::::: ##:::: ########:";
    end[7] = ":......:::::.......::::.......:::........:::........::::::..:::::........::";
    for (int i = 0; i < 8; i++) {
        cout << end[i] << endl;
        _sleep(300);
    }
    cout << "================================再见！！！==================================" << endl;
}
