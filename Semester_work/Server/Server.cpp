/**
 * Server for game TIC-TAC-TOE
 *
 * @author Lukas Runt
 * @date 25-10-2023
 * @version 0.0.1
 */

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <cstdlib>
#include <sys/ioctl.h>
#include <vector>
#include <sstream>

#include "User.h"


#define MAX_BUFFER_SIZE 1024
#define DEFAULT_PORT 10000
#define DEFAULT_MAX_USERS 10
#define NUMBER_OF_STREAMS 3 //(stdin,stdout, stderr)
#define DELIMITER '|'

std::vector<std::string> splitString(const std::string& text){
    std::vector<std::string> splitString;
    std::istringstream iss(text);
    std::string token;
    while(std::getline(iss, token, DELIMITER)){
        if (!token.empty())
            splitString.push_back(token);
    }

    return splitString;
}

int main(int argc, char *argv[]){
    int port = DEFAULT_PORT;
    int max_number_of_connected_users = DEFAULT_MAX_USERS;
    int server_socket;
    int client_socket, fd;
    int return_value;
    char buffer[MAX_BUFFER_SIZE];
    socklen_t len_addr;
    int a2read;
    struct sockaddr_in my_addr, peer_addr;
    fd_set client_socks, tests;
    User* connected_users[DEFAULT_MAX_USERS];

    //reading and parse arguments
    if(argc > 3){
        std::cerr << "Invalid number of parameters: " << argc << std::endl;
        std::cerr << "Parameters are:\n-c<number> = maximal number of connected users\n-p<number> = port" << std::endl;
        return EXIT_FAILURE;
    }

    for(int i = 1; i < argc; i++){
        const char *arg = argv[i];

        if(strlen(arg) >= 3 && arg[0] == '-'){
            char flag = arg[1]; //c or p
            char number_string[strlen(arg) - 1];

            strncpy(number_string, arg + 2, strlen(arg) - 2);
            number_string[strlen(arg) - 2] = '\0';
            int number;
            try{
                number = std::stoi(number_string);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid argument: " << e.what() << std::endl;
                return EXIT_FAILURE;
            } catch (const std::out_of_range& e) {
                std::cerr << "Out of range: " << e.what() << std::endl;
                return EXIT_FAILURE;
            }

            switch (flag){
                case 'c':
                    max_number_of_connected_users = number;
                    std::cout << "Maximal number of clients: " << number << std::endl;
                    break;
                case 'p':
                    port = number;
                    std::cout << "Maximal number of ports: " << number << std::endl;
                    break;
                default:
                    std::cerr << "Wrong flag!" << std::endl;
                    return EXIT_FAILURE;
            }

        }else{
            std::cerr << "Invalid argument format: " << arg << std::endl;
            return EXIT_FAILURE;
        }
    }

    //defining server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    memset(&my_addr, 0, sizeof(struct sockaddr_in));

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(port);
    my_addr.sin_addr.s_addr = INADDR_ANY;

    return_value = bind(server_socket, (struct sockaddr *) &my_addr, sizeof(struct sockaddr_in));

    if (return_value == 0)
        std::cout << "Bind - OK" << std::endl;
    else {
        std::cerr << "Bind - ERR" << std::endl;
        return -1;
    }

    return_value = listen(server_socket, 5);
    if (return_value == 0){
        std::cout << "Listen - OK"<< std::endl;
    } else {
        std::cerr << "Listen - ERR" << std::endl;
        return -1;
    }

    FD_ZERO(&client_socks);
    FD_SET(server_socket, &client_socks);

    for (;;) {
        tests = client_socks;

        return_value = select(FD_SETSIZE, &tests, (fd_set *) 0, (fd_set *) 0, (struct timeval *) 0);

        if (return_value < 0) {
            std::cerr << "Select - ERR" << std::endl;
            return -1;
        }

        for (fd = NUMBER_OF_STREAMS; fd < FD_SETSIZE; fd++) {
            if (FD_ISSET(fd, &tests)) {
                if (fd == server_socket) {
                    client_socket = accept(server_socket, (struct sockaddr *) &peer_addr, &len_addr);
                    FD_SET(client_socket, &client_socks);
                    User newUser;
                    connected_users[fd] = &newUser;
                    std::cout << "New client connected and added to the socket set" << std::endl;
                } else {
                    ioctl(fd, FIONREAD, &a2read);
                    if (a2read > 0) {
                        // Receive data into the buffer
                        int bytes_received = recv(fd, buffer, MAX_BUFFER_SIZE, 0);
                        if (bytes_received > 0) {
                            buffer[bytes_received] = '\0';  // Null-terminate the received data (assuming it's a string)
                            std::cout <<"Received: " <<  buffer << std::endl;

                            // Echo the received string back to the client
                            send(fd, buffer, strlen(buffer), 0);

                            if(connected_users[fd]->mState == 0){
                                std::string message(buffer);
                                std::vector<std::string> commands = splitString(message);
                                if(commands.size() > 0){
                                    if(commands.at(1) == "LOGIN"){
                                        std::cout << "OK" << std::endl;
                                    }
                                    else{
                                        std::cout << "WRONG" << std::endl;
                                    }
                                }else{
                                    std::cout << "WRONG" << std::endl;
                                }
                            }else{
                                std::cout << "New state" << std::endl;
                            }
                        }
                    } else {
                        close(fd);
                        FD_CLR(fd, &client_socks);
                        std::cout << "User disconnected and removed from the socket set" << std::endl;
                    }
                }
            }
        }
    }

    close(server_socket);
    return 0;
}
