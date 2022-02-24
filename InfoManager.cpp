//
// Created by Administrator on 2022/2/21.
//

#include "InfoManager.h"
#include "common.h"
#include <iostream>
#include <set>
#include <map>

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

//传入的num字符串是个整数比如10.0则返回10，若10.2返回10.2
string& get_number(string& num)
{
    int rbegin = num.length()-1;
    if(num[rbegin] == '0' && num[rbegin-1] == '.') {
        num = num.substr(0, rbegin-1);
    }
    return num;
}
void InfoManager::display_my_info() {
    //用户名 地址 联系方式 全在m_user组件里，谁使用Info管理器谁就把信息传给它
    //因此主要工作就是用计算器计算余额
    string line, exp; //exp就是生成的计算表达式
    //需要扫描充值文件和订单文件(购买和出售，并且合并数量相同的！)
    ifstream re_in(recharge_file);
    if(!re_in){
        cout << "Error: open file failed! " << recharge_file;
        return ;
    }

    vector<string> re_add; //充值所得
    getline(re_in, line); //第一行表头
    while(getline(re_in, line)){
        vector<string> each;
        my_split(line, ',', each); //each里包含 用户ID 充值金额 充值时间
        if(each[0] == m_user.m_id) { //记录
            re_add.push_back(each[1]);
        } else{
            continue;
        }
    }
    re_in.close();
    //生成计算表达式的 开始全为+的内容 得到 a + b + c的形式
    for(auto it : re_add){
        exp += get_number(it) + " + ";
    }
    //需要去除最后的" + "!
    exp = exp.substr(0, exp.length()-3);

    //扫描订单文件
    ifstream odr_in(order_file);
    if(!odr_in){
        cout << "Error: open file failed! " << order_file;
        return ;
    }
    //为了区分余额是+还是-，同时更方便地生成计算式子定义如下结构
    enum symbol{ //有1和-1两种，代表正负
        POS = 1,
        NEG = -1
    };
    typedef struct {
        symbol s;
        string price;
    }price_t;

    price_t p_tmp;
    map<string, vector<price_t>> odr_map;
    getline(odr_in, line); //第一行表头
    while(getline(odr_in, line)){
        vector<string> each;
        my_split(line, ',', each);
        if(each[5] == m_user.m_id) {//作为卖家
            p_tmp.s = POS;
            p_tmp.price = each[2];
            odr_map[each[3]].push_back(p_tmp);
        }
        else if(each[6] == m_user.m_id) {//作为买家
            p_tmp.s = NEG;
            p_tmp.price = each[2];
            odr_map[each[3]].push_back(p_tmp);
        }
    }
    //填写a + b + c之后的内容
    for(auto it : odr_map){
        if(it.first == "1"){//系数为1不需要括号和乘号
            for(auto tmp : it.second){
                if(tmp.s == POS){//卖出商品
                    exp += " + " + get_number(tmp.price);
                }else{ //NEG
                    exp += " - " + get_number(tmp.price);
                }
            }
        }
        else {//系数大于1需要括号和乘号
            //TODO: 正数负数公因数的提取，比如 3*(-1-2) 还是 -3*(1+2)？？？
            bool all_neg = false; //是否全部为负数
            bool be_first = true; //是否是()内第一个数字
            for(const auto& tmp : it.second){
                if(tmp.s == POS){
                    all_neg = true;
                    break;
                }
            }
            int the_count = it.second.size(); //second就是成交数量一致的那些商品单价
            if(all_neg){ //全为负数并且个数>1则将符号提到括号外 即 -c*(
                if( the_count > 1)
                    exp += " - " + it.first + " * (";
                else if(the_count == 1) //只有一个数字就不加括号
                    exp += " - " + it.first + " * ";
                else ;
            }else{ //不全为负数则 +c*(
                if( the_count > 1)
                    exp += " + " + it.first + " * (";
                else if(the_count == 1)
                    exp += " + " + it.first + " * ";
                else ;
            }
            //括号内的填写都是先填符号再填数字，若第一个为正数则不用填 + 号
            for(auto tmp : it.second){
                if(all_neg) { //全为负数的话符号已经提前了
                    if(be_first) { //第一个正数不写 + 号
                        exp += get_number(tmp.price);
                        be_first = false;
                    }else{
                        exp += " + " + get_number(tmp.price);
                    }
                }else{ //不全为负数则判断
                    if(tmp.s == POS){
                        if(be_first){ //第一个正数不写 + 号
                            exp += get_number(tmp.price);
                            be_first = false;
                        }else{
                            exp += " + " + get_number(tmp.price);
                        }
                    }
                    else{ //负数都要写 - 号
                        exp += " - " + get_number(tmp.price);
                    }
                }
            }
            if(the_count > 1) //同样数字个数>1 才补上右括号
                exp += ")";
        }
    }
    cout << "生成式子化简后为: " << exp << endl;
    //下面就是调用计算器计算余额了
    //TODO: 就是说生成的式子还有点问题，比如多个购买一次的商品价钱一样但是没有用乘法。。
    float money_res = 0;
    if(m_cal.calculate(exp, money_res)){
        cout << setiosflags(ios::fixed);
        cout << "*************************" << endl;
        cout << "用户名: " << m_user.m_name << endl;
        cout << "联系方式: " << m_user.m_tel << endl;
        cout << "地址: " << m_user.m_addr << endl;
        cout << "钱包余额: " << setprecision(1) << money_res << endl;
        cout << "*************************" << endl;
    }
    else{
        cout << "计算器出问题啦wwww" << endl;
        return;
    }

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

//充值金额  我的处理是将充值记录到充值文件中，格式为[用户id,充值金额,充值时间]
//用户文件的余额也修改，但是查看信息的时候是根据充值记录和订单卖出记录和购买记录 通过计算器生成的
void InfoManager::recharge() const {
    string add_money;

    float p = 0;
    while(true){
        cout << "请输入充值金额(保留一位小数): ";
        cin >> add_money;
        try {
            p = stof(add_money); //用串IO保留一位小数
            break;
        }
        catch (invalid_argument&) {
            cout << "请输入正确的浮点数！" << endl;
        }
        catch (...) {
            cout << "其他异常！" << endl;
        }
    }
    ostringstream sout;
    sout << setiosflags(ios::fixed);
    sout << setprecision(1) << p;
    add_money = sout.str();  //保留一位小数

    //充值记录附加填写
    ofstream re_out(recharge_file, ios::app);
    if(!re_out) {
        cout << "Usage: open file failed!" << recharge_file << endl;
        return ;
    }
    re_out << m_user.m_id + "," << add_money << "," + get_curtime() << endl;
    re_out.close();

    //修改用户文件的余额
    ifstream fin(user_file);
    ofstream fout("tmp_user_file.txt");
    if (!fin || !fout) {
        cout << "Error: open file failed! " << user_file;
        return;
    }
    string line;
    float balance;
    getline(fin, line); //第一行表头
    fout << line << endl; //表头别忘了写到临时文件里！
    while(getline(fin, line)) {
        vector<string> each;
        my_split(line, ',', each); //用,分隔user_file文件中的每一行填充each
        Users tmp(each); //得到当前这行代表的用户tmp
        if(tmp.m_id == m_user.m_id) { //需要修改这个用户的余额
            tmp.m_money += stof(add_money);
            balance = tmp.m_money;
            fout << tmp;
        }else{ //不需要修改直接写
            fout << line << endl;
        }
    }
    fin.close();
    fout.close();
    const char* oldname = "tmp_user_file.txt";
    const char* newname = user_file.c_str();
    rm_rename(newname, oldname);
    cout << "充值成功，当前余额: " << balance << endl;

}
