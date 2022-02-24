//
// Created by Administrator on 2022/2/24.
//

#include "Calculator.h"

void Calculator::display() {
    string exp;
    float res;
    cout << "============欢迎使用计算器===========" << endl;
    cout << "输入exit即可退出" << endl;
    while(true) {
        cout << "请输入表达式(exit退出):" << endl;
        cin >> exp;
        if(exp == "exit"){
            cout << "欢迎下次使用" << endl;
            break;
        }
        if(calculate(exp, res))
            cout << "计算结果为: " << res;
    }
}

Calculator::Calculator() {
    pri_map[')'] = 0;
    pri_map['+'] = pri_map['-'] = 1;
    pri_map['*'] = pri_map['/'] = 2;
    pri_map['('] = 3;
}

bool Calculator::is_valid(string &exp) {
    int index = 0;
    if(!exp.empty())
    {
        while( (index = exp.find(' ',index)) != string::npos)
        {
            exp.erase(index,1);
        }
    }
    //说不定全是空格
    if(exp.empty()){
        cout << "Error: 不能输入空字符串！" << endl;
        return false;
    }
    int tmp_count = 0; //检测括号是否匹配
    char pre = exp[0]; //查看是否连续出现两个操作符
    for(int i = 1; i < exp.length(); i++){
        if(tmp_count < 0){ //)括号太多
            cout << "Error: 圆括号不匹配！" << endl;
            return false;
        }
        if(exp[i] == '('){
            tmp_count++;
        }else if(exp[i] == ')'){
            tmp_count--;
        }
        if(valid_sym(exp[i]) && isdigit(exp[i])) { //不是数字也不是合法的操作符
            cout << "Error: 非法的字符: " << exp[i] << endl;
            return false;
        }
        if(valid_sym(exp[i]) && valid_sym(pre)){
            if(exp[i]!='('&&exp[i]!=')' && pre!='('&&pre!=')') {
                cout << "Error: 操作符误用！两个连续的操作符: " << pre << exp[i] << endl;
                return false;
            }
        }
        pre = exp[i];
    }
    if(tmp_count > 0) //(括号太多
    {
        cout << "Error: 圆括号不匹配！" << endl;
        return false;
    }
    return true;
}

//弹出val栈顶2个元素与opt栈顶1个元素执行运算并返回
float Calculator::cal_top() {
    float b = val.top();
    val.pop();
    float a = val.top();
    val.pop();
    char c = opt.top();
    opt.pop();

    switch (c) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        default: break;
    }
    return 0;
}

bool Calculator::calculate(string &exp, float &res) {
    if(!is_valid(exp)) //消除了所有空格并且粗略认为合法
        return false;
    opt.push('(');
    exp += ")"; //设置哨兵！？？
    for(int i = 0; i< exp.length();i++){
        if(isdigit(exp[i])) { //如果是数字则获取
            float ret = exp[i] - '0';
            while (i + 1 <= exp.size() && isdigit(exp[i + 1])) {
                ret = ret * 10 + exp[i + 1] - '0';
                i++;
            }
            if (exp[i + 1] == '.') //如果是小数点
            {
                i++;
                float ex = 0.1;
                while (i + 1 <= exp.size() && isdigit(exp[i + 1])) {
                    ret += (exp[i + 1] - '0') * ex;
                    ex *= ex;
                    i++;
                }
            }
            val.push(ret);
        }
        else { //其他操作符和负数的处理
            if ((i == 0 || exp[i - 1] == '(') && exp[i] == '-') { //处理负数问题， -1 或者(-1)可以等效为'-'前加个0
                val.push(0);
                opt.push('-');
                continue;
            }
            //当str[i]不为'('且优先级更低时，代表之前的运算符已可以计算
            while (opt.top() != '(' && pri_map[opt.top()] >= pri_map[exp[i]]) {
                val.push(cal_top()); //由于式子末尾有')'哨兵, 优先级为0，故遍历到末尾')'能将所有运算符计算完毕
            }
            if (opt.top() == '(' && exp[i] == ')')
                opt.pop();         //消去括号
            else
                opt.push(exp[i]);
        }
    }
    res = val.top();//由于此前的哨兵处理，最后num栈中只有唯一一个数，即最后结果
    return true;
}

bool Calculator::valid_sym(const char &i) {
    return (all_sym.find(i) != string::npos);
}


