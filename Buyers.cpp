//
// Created by Administrator on 2022/2/21.
//

#include "Buyers.h"
#include <set>

void Buyers::display_cmd() {
    cout << "===========================================================================" << endl;
    cout << "1.查看商品列表 2.购买商品 3.搜索商品 4.查看历史订单 5.查看商品详细信息 6.返回用户主界面" << endl;
    cout << "===========================================================================" << endl;
    cout << endl << "输入操作: " ;
}

Buyers::Buyers() {
    m_sql_helper.user_status = STATUS_BUYER;;
}

//通过User实例化Buyer，是因为设计框架将User派生出Buyer，main中User确定用户身份为Buyer后将调用该函数成为一个买家对象
//在卖家类中也是如此
Buyers::Buyers(Users &u) {
    m_id    =  u.m_id   ;
    m_name  =  u.m_name ;
    m_pass  =  u.m_pass ;
    m_tel   =  u.m_tel  ;
    m_addr  =  u.m_addr ;
    m_money =  u.m_money;
    m_state =  u.m_state;
    m_sql_helper.user_id = m_id;
    m_sql_helper.user_status = STATUS_BUYER;
}

void Buyers::buy_goods() {
    //获取商品ID对应的单价和卖家ID，以及判断数量是否为0生成下架指令
    vector<Commodity_t> all_com; //这里读一次文件记录所有商品的信息防止多次输入商品ID频繁开关文件
    ifstream com_fin(commodity_file);
    if (!com_fin) {
        cout << "Error: open file failed! " << user_file;
        return;
    }
    string line;
    getline(com_fin, line); //第一行表头
    while (getline(com_fin, line)) {
        vector<string> each;
        my_split(line, ',', each);
        Commodity_t tmp(each);
        all_com.push_back(tmp);
    }
    com_fin.close(); //得到所有商品的信息

    string com_id, the_count, the_price, the_m_id;
    Commodity_t *get_it; //通过指针访问数组中的此商品
    while(true) {
        cout << "请输入要购买的商品ID: ";
        cin >> com_id;
        for(auto& com : all_com){
            if(com.c_id == com_id && com.c_state == "销售中") { //商品ID存在则获取，注意用户只能看到销售中的产品！
                get_it = &com;
                ostringstream sout;
                sout << setiosflags(ios::fixed);
                sout << setprecision(1) << com.c_price;
                the_price = sout.str();  //保留一位小数
                the_m_id = com.m_id;
                goto OUT;
            }
        }
        cout << "该商品ID当前不在销售！" << endl;
        return ;
    }
OUT:
    int q = 0;
    while(true){
        cout << "请输入购买数量(正整数): ";
        cin.sync();
        getline(cin, the_count);
        if(the_count.empty()){
            cout << "输入不能为空！" << endl;
            continue;
        }
        bool flag = false;
        for(auto &c:the_count){
            if(!isdigit(c) && c!=' ' || c=='.'){ //有小数点就报错
                cout << "请输入正确的正整数！" << endl;
                flag = true;
                break;
            }
        }
        if(flag){
            continue;
        }
        try {
            q = stoi(the_count); //用串IO保留一位小数
            break;
        }
        catch (invalid_argument&) {
            cout << "请输入正确的正整数！" << endl;
        }
        catch (...) {
            cout << "其他异常！" << endl;
        }
    }
    if(get_it->c_count < q) { //商品数量不够
        cout << "此商品只剩余" << get_it->c_count << "个！" << endl;
        return ;
    }
    else if ((get_it->c_price) * (float)q > m_money){ //余额不足
        cout << "您的余额不足，仅剩" << m_money << "元" << endl;
        return;
    }
    else{ //修改数组中商品数量和自己的余额，其实没什么意义，比较SQL解析的时候只根据指令重新扫描文件
        get_it->c_count -= q;
        m_money -= (get_it->c_price) * (float)q;
    }

    the_count = to_string(q); //获取购买数量字符串
    //数量合法后才产生新的订单ID并生成sql指令
    //生成新的订单ID
    set<string> odrid_pool;
    ifstream fin(order_file);
    if (!fin) {
        cout << "Error: open file failed! " << user_file;
        return;
    }
    getline(fin, line); //第一行表头
    while (getline(fin, line)) {
        vector<string> each;
        my_split(line, ',', each);
        Order_t tmp(each);
        odrid_pool.insert(tmp.o_id);
    }
    fin.close(); //得到订单ID池

    //取id池中最大id + 1生成新的id, 用串IO实现3位编号补零
    ostringstream idout;
    string new_id;
    if(odrid_pool.empty()){ //如果没有订单则T001
        new_id = "T001";
    }else {
        idout << setw(3) << setfill('0') << stoi((*odrid_pool.rbegin()).substr(1, 3)) + 1;
        new_id = "T" + idout.str();
    }
    //获取上架时间 年-月-日
    time_t t = time(nullptr);
    char tmp[32] = { 0 };
    strftime(tmp, sizeof(tmp), "%Y-%m-%d", localtime(&t));
    string cur_time(tmp);

    string sql_cmd = "INSERT INTO order VALUES (" + new_id + "," + com_id + "," + the_price + ","
                    + the_count + "," + cur_time + "," + the_m_id + "," + m_id + ")";
    cout << "对应SQL命令为: " << sql_cmd << endl;
    m_sql_helper.sql_analyse(sql_cmd);

    if(get_it->c_count == q) //如果商品数量为0需要生成一条下架指令
    {
        //这里做了一步优化，在SqlHelper中如果发现商品数量为0就会捎带把状态设置成下架
        //也就是这个指令会生成，会写到指令文件，但是并没有交给SqlHelper再去分析执行，没必要重复工作
        sql_cmd = "UPDATE commodity SET 商品状态 = 已下架 WHERE 商品ID = " + get_it->m_id;
        cout << "对应SQL命令为: " << sql_cmd << endl;
        write_order(sql_cmd); //记录到order文件
    }
}

void Buyers::search_goods() {
    string com_name, sql_cmd;
    cout << "请输入商品名称: ";
    cin >> com_name;
    sql_cmd = "SELECT * FROM commodity WHERE 名称 CONTAINS " + com_name;
    cout << "对应SQL命令为: " << sql_cmd << endl;
    m_sql_helper.sql_analyse(sql_cmd);
}

void Buyers::display_good_info() {
    string com_id, sql_cmd;;
    cout << "请输入您想要查看的商品ID: ";
    cin >> com_id;
    sql_cmd = "SELECT * FROM commodity WHERE 商品ID CONTAINS " + com_id;
    cout << "对应SQL命令为: " << sql_cmd << endl;
    m_sql_helper.sql_analyse(sql_cmd);
}
