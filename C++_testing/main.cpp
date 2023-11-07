#include <iostream>
#include <memory>

#include "User.h"



int main(){
    int fd_lukas = 1;
    std::shared_ptr<User> newUser = std::make_shared<User>(fd_lukas);
    std::string message = "LOGIN|Lukas";
    newUser->execute_message(message, fd_lukas);
    int fd_petr = 2;
    std::shared_ptr<User> petr = std::make_shared<User>(fd_petr);
    message = "LOGIN|Petr";
    petr->execute_message(message, fd_petr);
    int fd_michal = 3;
    std::shared_ptr<User> michal = std::make_shared<User>(fd_michal);
    message = "LOGIN|Michal";
    michal->execute_message(message, fd_michal);
    int fd_jan = 4;
    std::shared_ptr<User> jan = std::make_shared<User>(fd_jan);
    message = "LOGIN|Jan";
    jan->execute_message(message, fd_jan);
    std::shared_ptr<User> user = User::getUserByFd(fd_petr);
    user->disconnect_user();
    cout << "Printing" << endl;
    User::printUsers();
    // own user
    int fd_my_user = 5;
    std::string user_input;
    std::shared_ptr<User> my_user = std::make_shared<User>(6);
    while(true){
        std::cout << "Enter message: ";
        std::getline(std::cin, user_input);
        my_user->execute_message(user_input, fd_my_user);
        User::printUsers();
    }
}
