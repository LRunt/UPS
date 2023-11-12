#include <iostream>
#include <memory>

#include "User_test.h"
#include "Game_test.h"



int main(){

    int fd_lukas = 1;
    User_test::execute_message("LOGIN|Lukas", fd_lukas);
    std::shared_ptr<User_test> newUser = std::make_shared<User_test>("Lukas", fd_lukas);
    int fd_petr = 2;
    std::shared_ptr<User_test> petr = std::make_shared<User_test>("Petr", fd_petr);
    User_test::execute_message("LOGIN|Petr", fd_petr);
    int fd_michal = 3;
    std::shared_ptr<User_test> michal = std::make_shared<User_test>("Michal", fd_michal);
    User_test::execute_message("LOGIN|Michal", fd_michal);
    int fd_jan = 4;
    std::shared_ptr<User_test> jan = std::make_shared<User_test>("Jan", fd_jan);
    User_test::execute_message("LOGIN|Jan", fd_jan);
    User_test::execute_message("START", fd_jan);
    std::shared_ptr<User_test> user = User_test::get_user_by_fd(fd_petr);
    user->disconnect_user();
    cout << "Printing" << endl;
    User_test::print_users();

    //Game_test game("Pepa", "Vojta");

    // own user
    int fd_my_user = 5;
    std::string user_input;
    while(true){
        std::cout << "Enter message: ";
        std::getline(std::cin, user_input);
        string response = User_test::execute_message(user_input, fd_my_user);
        std::cout << "response: " << response << std::endl;
        User_test::print_users();
    }
}
