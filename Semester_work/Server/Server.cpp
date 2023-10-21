#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/ioctl.h>


#define MAX_BUFFER_SIZE 1024

int main(void){
    int server_socket;
    int client_socket, fd;
    int return_value;
    char buffer[MAX_BUFFER_SIZE];
    socklen_t len_addr;
    int a2read;
    struct sockaddr_in my_addr, peer_addr;
    fd_set client_socks, tests;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    memset(&my_addr, 0, sizeof(struct sockaddr_in));

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(10000);
    my_addr.sin_addr.s_addr = INADDR_ANY;

    return_value = bind(server_socket, (struct sockaddr *) &my_addr, sizeof(struct sockaddr_in));

    if (return_value == 0)
        std::cout << "Bind - OK" << std::endl;
    else {
        std::cout << "Bind - ERR" << std::endl;
        return -1;
    }

    return_value = listen(server_socket, 5);
    if (return_value == 0){
        std::cout << "Listen - OK"<< std::endl;
    } else {
        std::cout << "Listen - ERR" << std::endl;
    }

    FD_ZERO(&client_socks);
    FD_SET(server_socket, &client_socks);

    for (;;) {
        tests = client_socks;

        return_value = select(FD_SETSIZE, &tests, (fd_set *) 0, (fd_set *) 0, (struct timeval *) 0);

        if (return_value < 0) {
            std::cout << "Select - ERR" << std::endl;
            return -1;
        }

        for (fd = 3; fd < FD_SETSIZE; fd++) {
            if (FD_ISSET(fd, &tests)) {
                if (fd == server_socket) {
                    client_socket = accept(server_socket, (struct sockaddr *) &peer_addr, &len_addr);
                    FD_SET(client_socket, &client_socks);
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
                        }
                    } else {
                        close(fd);
                        FD_CLR(fd, &client_socks);
                        std::cout << "Client disconnected and removed from the socket set" << std::endl;
                    }
                }
            }
        }
    }

    close(server_socket);
    return 0;
}
