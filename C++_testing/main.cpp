#include <iostream>
#include <memory>

#include "User.h"
#include "Game.h"



int main(){

    /*int fd_lukas = 1;
    User::execute_message("LOGIN|Lukas", fd_lukas);
    std::shared_ptr<User> newUser = std::make_shared<User>("Lukas",fd_lukas);
    int fd_petr = 2;
    std::shared_ptr<User> petr = std::make_shared<User>("Petr", fd_petr);
    User::execute_message("LOGIN|Petr", fd_petr);
    int fd_michal = 3;
    std::shared_ptr<User> michal = std::make_shared<User>("Michal", fd_michal);
    User::execute_message("LOGIN|Michal", fd_michal);
    int fd_jan = 4;
    std::shared_ptr<User> jan = std::make_shared<User>("Jan",fd_jan);
    User::execute_message("LOGIN|Jan", fd_jan);
    std::shared_ptr<User> user = User::get_user_by_fd(fd_petr);
    user->disconnect_user();
    cout << "Printing" << endl;
    User::print_users();*/

    Game game("Pepa", "Vojta");

    // own user
    /*int fd_my_user = 5;
    std::string user_input;
    while(true){
        std::cout << "Enter message: ";
        std::getline(std::cin, user_input);
        User::execute_message(user_input, fd_my_user);
        User::print_users();
    }*/
}
