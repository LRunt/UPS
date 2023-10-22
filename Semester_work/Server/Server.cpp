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

int main(void){
    int server_socket;
    int client_socket, fd;
    int return_value;
    char buffer[MAX_BUFFER_SIZE];
    socklen_t len_addr;
    int a2read;
    struct sockaddr_in my_addr, peer_addr;
    fd_set client_socks, tests;
    std::vector<User> connectedUsers;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    memset(&my_addr, 0, sizeof(struct sockaddr_in));

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(10000);
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
                    connectedUsers.insert(connectedUsers.begin() + (fd - NUMBER_OF_STREAMS), newUser);
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

                            if(connectedUsers.at(fd - NUMBER_OF_STREAMS).state == 0){
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
