#include <iostream>
#include <memory>

#include "User.h"



int main(){
    std::shared_ptr<User> newUser = std::make_shared<User>(1);
    std::string message = "LOGIN|Lukas";
    newUser->execute_message(message);
    newUser = std::make_shared<User>(2);
    User::users.push_back(newUser);
    newUser = std::make_shared<User>(10);
    User::users.push_back(newUser);
    User::printUsers();
    std::shared_ptr<User> user = User::getUserByFd(2);
    user->disconnect_user();
    std::cout << user->toString() << std::endl;
    std::shared_ptr<User> lukas = std::make_shared<User>(3);
    message = "LOGIN|Petr";
    lukas->execute_message(message);
    cout << "Printing" << endl;
    User::printUsers();

}
