#pragma once

#include "user.hpp"

//群组用户，多了一个角色信息，可从user类直接继承，复用user的其他信息
class GroupUser : public User
{
public:
    void setRole(string role) { this->role = role; }
    string getRole() { return this->role; }

private:
    string role;
};