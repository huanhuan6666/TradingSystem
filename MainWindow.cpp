//
// Created by Administrator on 2022/2/21.
//

#include "MainWindow.h"
#include "common.h"
#include <iostream>
#include <fstream>

using namespace std;

void MainWindow::cmd_display() {
    cout << "\033[1;36m===================================================\033[0m" << endl;
    cout << "\033[1;36m1.管理员登录 2.用户注册 3.用户登录 4.退出程序 5.计算器\033[0m" << endl;
    cout << "\033[1;36m===================================================\033[0m" << endl;
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
    start_1[0] = "\033[33mHHHHHHHHH     HHHHHHHHH                   lllllll lllllll                   !!!\033[0m";
    start_1[1] = "\033[33mH:::::::H     H:::::::H                   l:::::l l:::::l                  !!:!!\033[0m";
    start_1[2] = "\033[33mH:::::::H     H:::::::H                   l:::::l l:::::l                  !:::!\033[0m";
    start_1[3] = "\033[33mHH::::::H     H::::::HH                   l:::::l l:::::l                  !:::!\033[0m";
    start_1[4] = "\033[33m  H:::::H     H:::::H      eeeeeeeeeeee    l::::l  l::::l    ooooooooooo   !:::!\033[0m";
    start_1[5] = "\033[33m  H:::::H     H:::::H    ee::::::::::::ee  l::::l  l::::l  oo:::::::::::oo !:::!\033[0m";
    start_1[6] = "\033[33m  H::::::HHHHH::::::H   e::::::eeeee:::::eel::::l  l::::l o:::::::::::::::o!:::!\033[0m";
    start_1[7] = "\033[33m  H:::::::::::::::::H  e::::::e     e:::::el::::l  l::::l o:::::ooooo:::::o!:::!\033[0m";
    start_1[8] = "\033[33m  H:::::::::::::::::H  e:::::::eeeee::::::el::::l  l::::l o::::o     o::::o!:::!\033[0m";
    start_1[9] = "\033[33m  H::::::HHHHH::::::H  e:::::::::::::::::e l::::l  l::::l o::::o     o::::o!:::!\033[0m";
    start_1[10] = "\033[33m  H:::::H     H:::::H  e::::::eeeeeeeeeee  l::::l  l::::l o::::o     o::::o!!:!!\033[0m";
    start_1[11] = "\033[33m  H:::::H     H:::::H  e:::::::e           l::::l  l::::l o::::o     o::::o !!!\033[0m";
    start_1[12] = "\033[33mHH::::::H     H::::::HHe::::::::e         l::::::ll::::::lo:::::ooooo:::::o\033[0m";
    start_1[13] = "\033[33mH:::::::H     H:::::::H e::::::::eeeeeeee l::::::ll::::::lo:::::::::::::::o !!!\033[0m";
    start_1[14] = "\033[33mH:::::: H     H:::::::H  ee:::::::::::::e l::::::ll::::::l oo:::::::::::oo !!:!!\033[0m";
    start_1[15] = "\033[33mHHHHHHHHH     HHHHHHHHH    eeeeeeeeeeeeee llllllllllllllll   ooooooooooo    !!!\033[0m";
    for (int i = 0; i < 16; i++) {
        cout << start_1[i] << endl;
        system("sleep 0.3");
    }
    cout << "================================初始化完成！！！=================================" << endl;
    cout << "即将进入系统" << flush;
    for(int i = 0; i < 6; i++){
        cout << "." << flush;
        system("sleep 0.3");
    }
    cout << endl;
    system("clear");
}

void MainWindow::end_show() {
    system("clear");
    string end[8];
    end[0] = "\033[34m:'######::::'#######:::'#######::'########::'########::'##:::'##:'########:\033[0m";
    end[1] = "\033[34m'##... ##::'##.... ##:'##.... ##: ##.... ##: ##.... ##:. ##:'##:: ##.....::\033[0m";
    end[2] = "\033[34m ##:::..::: ##:::: ##: ##:::: ##: ##:::: ##: ##:::: ##::. ####::: ##:::::::\033[0m";
    end[3] = "\033[34m ##::'####: ##:::: ##: ##:::: ##: ##:::: ##: ########::::. ##:::: ######:::\033[0m";
    end[4] = "\033[34m ##::: ##:: ##:::: ##: ##:::: ##: ##:::: ##: ##.... ##:::: ##:::: ##...::::\033[0m";
    end[5] = "\033[34m ##::: ##:: ##:::: ##: ##:::: ##: ##:::: ##: ##:::: ##:::: ##:::: ##:::::::\033[0m";
    end[6] = "\033[34m. ######:::. #######::. #######:: ########:: ########::::: ##:::: ########:\033[0m";
    end[7] = "\033[34m:......:::::.......::::.......:::........:::........::::::..:::::........::\033[0m";
    for (int i = 0; i < 8; i++) {
        cout << end[i] << endl;
        system("sleep 0.3");
    }
    cout << "================================再见！！！==================================" << endl;
}
