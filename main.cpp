#include <iostream>
#include "MainWindow.h"
#include "Administrator.h"
#include "Users.h"
#include "Buyers.h"
#include "Sellers.h"
#include "InfoManager.h"

using namespace std;

void manage_choose_Buyer(string& cmd, Buyers& buyer)
{
    if(cmd == "1") //查看商品列表
    {

    }
    else if(cmd == "2") //购买商品
    {

    }
    else if(cmd == "3") //搜索商品
    {

    }
    else if(cmd == "4") //查看历史订单
    {

    }
    else if(cmd == "5") //查看商品详细信息
    {

    }
    else //异常输入
    {
        cout << "没有这个功能，请输入上述展示功能对应的数字!" << endl;
    }
}

void manage_choose_Seller(string& cmd, Sellers& seller)
{
    if(cmd == "1") //发布商品
    {

    }
    else if(cmd == "2") //查看发布商品
    {
        seller.display_my_good();
    }
    else if(cmd == "3") //修改商品信息
    {
        seller.update_my_good();
    }
    else if(cmd == "4") //下架商品
    {
        seller.off_my_shelf();
    }
    else if(cmd == "5") //查看历史订单
    {

    }
    else //异常输入
    {
        cout << "没有这个功能，请输入上述展示功能对应的数字!" << endl;
    }

}

void manage_choose_InfoManager(string &cmd, InfoManager &info_mag) {
    //1.查看信息 2.修改信息 3.充值 4.返回用户
    if (cmd == "1")
    {

    }
    else if (cmd == "2")
    {

    }
    else if(cmd == "3")
    {

    }
    else //异常输入
    {
        cout << "没有这个功能，请输入上述展示功能对应的数字!" << endl;
    }
}
void manage_choose_User(string& cmd, Users &my_user)
{
    string choose;
    if(cmd == "2") //我是买家
    {
        //1.查看商品列表 2.购买商品 3.搜索商品 4.查看历史订单 5.查看商品详细信息 6.返回用户主界面
        Buyers my_buyer(my_user); //通过User初始化Buyer，Buyer从User继承了用户的数据
        my_buyer.display_cmd(); //展示买家主界面
        cin >> choose;
        while(choose != "6") //退出到用户主界面
        {
            manage_choose_Buyer(choose, my_buyer);
            my_buyer.display_cmd();
            cin >> choose;
        }
        cout << "您已经成功切换到用户主界面" << endl;
    }
    else if(cmd == "3") //我是卖家
    {
        //1.发布商品 2.查看发布商品 3.修改商品信息 4.下架商品 5.查看历史订单 6.返回用户主界面
        Sellers my_seller(my_user);
        my_seller.display_cmd(); //展示买家主界面
        cin >> choose;
        while(choose != "6") //退出到用户主界面
        {
            manage_choose_Seller(choose, my_seller);
            my_seller.display_cmd();
            cin >> choose;
        }
        cout << "您已经成功切换到用户主界面" << endl;
    }
    else if(cmd == "4") //个人信息管理
    {
        //1.查看信息 2.修改信息 3.充值 4.返回用户
        InfoManager info_mag;
        info_mag.display_cmd();
        cin >> choose;
        while(choose != "4") //退出到用户主界面
        {
            manage_choose_InfoManager(choose, info_mag);
            info_mag.display_cmd();
            cin >> choose;
        }
        cout << "您已经成功切换到用户主界面" << endl;
    }
    else //异常输入
    {
        cout << "没有这个功能，请输入上述展示功能对应的数字!" << endl;
    }

}
void manage_choose_Admin(string& cmd, Administrator &admin)
{
//1.查看所有商品 2.搜索商品 3.查看所有订单 4.查看所有用户 5.封禁用户 6.下架商品 7.注销
    if(cmd == "1") //查看所有商品
    {
        admin.display_all_goods();
    }
    else if(cmd == "2") //搜索商品
    {
        admin.search_goods();
    }
    else if(cmd == "3") //查看所有订单
    {
        admin.display_all_orders();
    }
    else if(cmd == "4") //查看所有用户
    {
        admin.display_all_users();
    }
    else if(cmd == "5") //封禁用户
    {
        admin.block_users();
    }
    else if(cmd == "6") //下架商品
    {
        admin.off_the_shelf();
    }
    else //非法输入
    {
        cout << "没有这个功能，请输入上述展示功能对应的数字!" << endl;
    }

}

void manage_choose_UI(string& choose)
{
    string cmd; //选择命令
    if(choose == "1") //管理员登录
    {
        //1.查看所有商品 2.搜索商品 3.查看所有订单 4.查看所有用户 5.删除用户 6.下架商品 7.注销
        Administrator my_admin;
        if(my_admin.check_pass())
        {
            my_admin.cmd_display(); //展示管理员界面
            cin >> cmd;
            while(cmd != "7") //注销管理员
            {
                manage_choose_Admin(cmd, my_admin);
                my_admin.cmd_display(); //展示管理员界面
                cin >> cmd;
            }
            cout << "您已经成功注销管理员登录" << endl;
        }

    }
    else if(choose == "2") //用户注册
    {

    }
    else if(choose == "3") //用户登录
    {
        // 1.注销登录 2.我是买家 3.我是卖家 4.个人信息管理
        Users my_user;
        if(my_user.check_pass()) //登录成功则my_user表示确定的用户
        {
            my_user.display_cmd(); //展示用户主界面
            cin >> cmd;
            while(cmd != "1") //1表示注销登录
            {
                manage_choose_User(cmd, my_user);
                my_user.display_cmd();
                cin >> cmd;
            }
            cout << "您已经成功注销用户登录" << endl;
        }

    }
    else // 非法输入
    {
        cout << "没有这个功能，请输入上述展示功能对应的数字!" << endl;
    }

}

int main() {
    MainWindow UI;
    string choose;
    //1.管理员登录 2.用户注册 3.用户登录 4.退出程序
    UI.cmd_display(); //展示主界面菜单
    cin >> choose;
    while(choose != "4") //4退出程序
    {
        manage_choose_UI(choose); //处理选择
        UI.cmd_display();
        cin >> choose;
    }
    cout << "您已经成功退出程序" << endl;

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
