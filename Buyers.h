//
// Created by Administrator on 2022/2/21.
//

#ifndef PROJECT1_BUYERS_H
#define PROJECT1_BUYERS_H
#include "Users.h"
#include <iostream>

using namespace std;

class Buyers : public Users{
public:
    Buyers();
    Buyers(Users &u);
    void display_cmd() override;

};


#endif //PROJECT1_BUYERS_H
