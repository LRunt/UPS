#include <iostream>
#include <memory>

#include "User_test.h"

int main(){
    std::shared_ptr<User> newUser = std::make_shared<User>(1, "Pavel");
    User::users.push_back(newUser);
    newUser = std::make_shared<User>(2, "Radim");
    User::users.push_back(newUser);
    newUser = std::make_shared<User>(10, "Ptáček");
    User::users.push_back(newUser);
}