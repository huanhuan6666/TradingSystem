//
// Created by Administrator on 2022/2/24.
//
#include "Calculator.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
using namespace std;

void Calculator::display() {
    string exp;
    float res;
    cout << "============欢迎使用计算器===========" << endl;
    cout << "输入exit即可退出" << endl;
    while (true) {
        cout << "请输入表达式(exit退出): ";
        cin.sync(); //清空缓冲区
        getline(cin, exp);
        if (exp == "exit") {
            cout << "======欢迎下次使用======" << endl;
            break;
        }
        if (calculate(exp, res)) {
            cout << setiosflags(ios::fixed);
            if (res == int(res)) //没有小数
                cout << "计算结果为(整数): " << int(res) << endl;
            else //有小数
                cout << "计算结果为(保留一位小数): " << setprecision(1) << res << endl;
        }
        //清空栈防止对后续操作造成干扰
        while (!val.empty()) val.pop();
        while (!opt.empty()) opt.pop();
    }
}

Calculator::Calculator() {
    pri_map[')'] = 0;
    pri_map['+'] = pri_map['-'] = 1;
    pri_map['*'] = pri_map['/'] = 2;
    pri_map['('] = 3;
}

bool Calculator::is_valid(string& exp) {
    if (!exp.empty())
    {
        int size = exp.size(), pre = 0, nxt = 0;
        vector<int> index;
        for (int i = 0; i < size; i++) {
            if (exp[i] >= '0' && exp[i] <= '9') {
                index.push_back(i);
                continue;
            }
            if(exp[i] == '.'){
                if(i-1>=0 && exp[i-1]==' ' ||
                    i+1<=size-1 && exp[i+1]==' '){
                    cout << "Error: 小数点两边不能有空格！" << endl;
                    return false;
                }
            }
        }
        int i_size = index.size();
        if (i_size > 1) {
            for (int i = 0; i < i_size - 1; i++) {
                int pre = index[i], nxt = index[i + 1];
                if (pre + 1 == nxt)
                    continue;
                else
                {
                    bool flag = true;
                    for (int j = pre + 1; j < nxt; j++) {
                        if (exp[j] != ' ' && exp[j] != '\t') {
                            flag = false;
                            break;
                        }
                    }
                    if (flag) //数字之间全是空格
                    {
                        cout << "Error: 相邻数字之间不能只有空格！" << endl;
                        return false;
                    }
                }
            }
        }
        int i = 0;
        while ((i = exp.find(' ', i)) != string::npos)
        {
            exp.erase(i, 1);
        }
    }
    //说不定全是空格
    if (exp.empty()) {
        cout << "Error: 不能输入空字符串！" << endl;
        return false;
    }
    int size = exp.size();
    int tmp_count = 0; //检测括号是否匹配
    char pre = exp[0]; //查看是否连续出现两个操作符
    for (int i = 0; i < size; i++) {
        if (tmp_count < 0) { //)括号太多
            cout << "Error: 圆括号不匹配！)太多" << endl;
            return false;
        }
        if (exp[i] == '(') {
            tmp_count++;
            if (pre >= '0' && pre <= '9'){ //2(缺省*
                cout << "Error: 数字和括号之间缺少乘号: " << pre << exp[i] << endl;
                return false;
            }
        }
        else if (exp[i] == ')') {
            tmp_count--;
            if (i + 1 < size && exp[i + 1] >= '0' && exp[i + 1] <= '9') { //)2缺少*
                cout << "Error: 数字和括号之间缺少乘号: " << exp[i] << exp[i+1] << endl;
                return false;
            }
        }
        if (!valid_sym(exp[i]) && !(exp[i]>='0'&&exp[i]<='9')) { //不是数字也不是合法的操作符
            cout << "Error: 式子中出现非法的字符: " << exp[i] << endl;
            return false;
        }
        if (i>0 && valid_sym(exp[i]) && valid_sym(pre)) {
            if (exp[i] != '(' && exp[i] != ')' && pre != '(' && pre != ')') {
                cout << "Error: 操作符误用！两个连续的操作符: " << pre << exp[i] << endl;
                return false;
            }
        }
        pre = exp[i];
    }
    if (tmp_count > 0) //(括号太多
    {
        cout << "Error: 圆括号不匹配！(太多" << endl;
        return false;
    }
    return true;
}

//弹出val栈顶2个元素与opt栈顶1个元素执行运算并回写到res中
bool Calculator::cal_top(float &res) {
    //弹出操作符
    char c = opt.top();
    opt.pop();
    //弹出操作数
    float a, b;
    if (val.empty()) {//val栈为空还想弹，操作数不够的时候会出现这种情况，比如 -, 3-这样子
        cout << "Error: 解析出错: " << c << endl;
        return false;
    }
    else {
        b = val.top();
        val.pop();
    }
    //再弹一个操作数
    if (val.empty()) {
        cout << "Error: 解析出错: " << c << endl;
        return false;
    }
    else {
        a = val.top();
        val.pop();
    }

    switch (c) {
        case '+': {
            res = a + b;
            break;
        }
        case '-': {
            res = a - b;
            break;
        }
        case '*': {
            res = a * b;
            break;
        }
        case '/': {
            if (b == 0) {//除零异常！
                cout << "Error: 除零异常！" << endl;
                return false;
            }
            else
                res = a / b;
            break;
        }
        default: break;
    }
    return true;
}

bool Calculator::calculate(string& exp, float& res) {
    if (!is_valid(exp)) //消除了所有空格并且粗略认为合法
        return false;
    opt.push('(');
    exp += ")"; //设置哨兵，最后的)可以保证让opt栈清空
    int size = exp.size();
    for (int i = 0; i < size; i++) {
        if (isdigit(exp[i])) { //如果是数字则获取
            float ret = exp[i] - '0';
            while (i + 1 <= size && isdigit(exp[i + 1])) {
                ret = ret * 10 + exp[i + 1] - '0';
                i++;
            }
            if (exp[i + 1] == '.') //如果是小数点
            {
                i++;
                float ex = 0.1;
                while (i + 1 <= size) {
                    if (isdigit(exp[i + 1])) {
                        ret += (exp[i + 1] - '0') * ex;
                        ex *= ex;
                        i++;
                    }
                    else{
                        if (exp[i + 1] == '.') {
                            cout << "Error: 操作数异常，小数点表示错误" << endl;
                            return false;
                        }
                        else
                            break;
                    }
                }
            }
            val.push(ret);
        }
        else { //操作符和负数的处理
            if ((i == 0 || exp[i - 1] == '(') && exp[i] == '-') { //处理负数问题， -1 或者(-1)可以等效为'-'前加个0
                val.push(0);
                opt.push('-');
                continue;
            }
            //opt栈中pri>=str[i]的都要弹出计算，保证了同等优先级从左到右的计算顺序,弹到空或者碰到(为止
            while (!opt.empty() && opt.top() != '(' && pri_map[opt.top()] >= pri_map[exp[i]]) {
                float tmpres = 0;
                if(cal_top(tmpres)) //会判断除零以及操作数不够的异常
                    val.push(tmpres);
                else {
                    return false;
                }
            }
            if (!opt.empty() && opt.top() == '(' && exp[i] == ')') //若)则让(也弹出
                opt.pop();         //消去括号
            else  //opt能弹的都弹完了则压入exp[i]
                opt.push(exp[i]);
        }
    }
    if(val.empty()){
        cout << "无效表达式！" << endl;
        return false;
    }
    res = val.top();//由于此前的哨兵处理，最后num栈中只有唯一一个数，即最后结果
    while (!val.empty()) val.pop(); //清空栈
    while (!opt.empty()) opt.pop();
    return true;
}

bool Calculator::valid_sym(const char& i) {
    return (all_sym.find(i) != string::npos);
}


