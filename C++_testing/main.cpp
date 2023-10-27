#include <iostream>
#include <memory>

#include "User.h"



int main(void){
    std::unique_ptr<User> active_users[10];

    std::cout << "Hello world!" << std::endl;

    if(0 < 1){
        active_users[0] = make_unique<User>();
    }
    User nextUser;
    nextUser.mState = 50;
    std::cout << active_users[0]->mState << std::endl;
    return EXIT_SUCCESS;
}
