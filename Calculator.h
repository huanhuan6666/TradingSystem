//
// Created by Administrator on 2022/2/24.
//

#ifndef PROJECT1_CALCULATOR_H
#define PROJECT1_CALCULATOR_H
#include <stack>
#include <iostream>
#include <map>
using namespace std;

class Calculator {
public:
    const string all_sym = "+-*/()."; //所有合法的字符
    map<char, int> pri_map;
    stack<char> opt;     //操作符栈
    stack<float> val;   //操作数栈
public:
    Calculator(); //初始化pri_map
    void display();
    //TODO: 需要对表达式进行合法性判断
    bool valid_sym(const char& i);
    bool is_valid(string &exp); //初步合法性判断，并且消除空格

    float cal_top(); //弹出两个val和一个opt计算
    bool calculate(string &exp, float &res); //计算表达式exp将结果放在res中
};


#endif //PROJECT1_CALCULATOR_H
