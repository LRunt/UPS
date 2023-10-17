#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#define MAX_BUFFER_SIZE 1024
#define MAX_CLIENTS 10
#define STATE_NEW_USER 0
#define STATE_WAITING_FOR_NUMBER 1
#define STATE_RIGHT_ANSWER 2
#define START_COMMUNICATION "HELLO"

struct Client {
    int id;
    int state;
    int received_number;
};

int generate_random_number(){
    srand(time(NULL));
    return (rand() % 100) + 1;
}

void create_message(char *str, int number){
    char random_number_string[20];
    sprintf(random_number_string, "NUM:%d\n", number);
    strcat(str, random_number_string);
}

int main(void) {
    int server_socket;
    int client_socket, fd;
    int return_value;
    char buffer[MAX_BUFFER_SIZE];
    int len_addr;
    int a2read;
    int received_number_from_client;
    int number_of_clients = 0;
    char newCommand[MAX_BUFFER_SIZE];
    struct sockaddr_in my_addr, peer_addr;
    fd_set client_socks, tests;

    //creating array for clients
    struct Client clients[MAX_CLIENTS];

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    memset(&my_addr, 0, sizeof(struct sockaddr_in));

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(10000);
    my_addr.sin_addr.s_addr = INADDR_ANY;

    return_value = bind(server_socket, (struct sockaddr *) &my_addr, sizeof(struct sockaddr_in));

    if (return_value == 0)
        printf("Bind - OK\n");
    else {
        printf("Bind - ERR\n");
        return -1;
    }

    return_value = listen(server_socket, 5);
    if (return_value == 0){
        printf("Listen - OK\n");
    } else {
        printf("Listen - ERR\n");
    }

    FD_ZERO(&client_socks);
    FD_SET(server_socket, &client_socks);

    for (;;) {
        tests = client_socks;

        return_value = select(FD_SETSIZE, &tests, (fd_set *) 0, (fd_set *) 0, (struct timeval *) 0);

        if (return_value < 0) {
            printf("Select - ERR\n");
            return -1;
        }

        for (fd = 3; fd < FD_SETSIZE; fd++) {
            if (FD_ISSET(fd, &tests)) {
                if (fd == server_socket) {
                    client_socket = accept(server_socket, (struct sockaddr *) &peer_addr, &len_addr);
                    FD_SET(client_socket, &client_socks);
                    //printf("New client %d connected.", fd);
                    printf("New connection , socket fd is %d , ip is : %d , port : %d\n" , client_socket , inet_ntoa(peer_addr.sin_addr) , ntohs(peer_addr.sin_port));
                    struct Client newClient = {client_socket, STATE_NEW_USER};
                    clients[client_socket] = newClient;
                    number_of_clients++;

                } else {
                    ioctl(fd, FIONREAD, &a2read);
                    if (a2read > 0) {
                        // Receive data into the buffer
                        int bytes_received = recv(fd, buffer, MAX_BUFFER_SIZE, 0);
                        if (bytes_received > 0) {
                            buffer[bytes_received] = '\0';  // Null-terminate the received data (assuming it's a string)
                            printf("Received form fd %d: %s\n", fd ,buffer);

                            // Echo the received string back to the client
                            send(fd, buffer, strlen(buffer), 0);
                        }

                        switch(clients[fd].state){
                            //New user, expected HELLO
                            case 0:
                                if(!strcmp(buffer, START_COMMUNICATION)){
                                    printf("Welcome %d to the server!\n", fd);
                                    clients[fd].received_number = generate_random_number();

                                    create_message(newCommand, clients[fd].received_number);
                                    printf("New message: %s", newCommand);
                                    newCommand[0] = '\0';
                                    send(fd, newCommand, strlen(newCommand), 0);
                                }else{
                                    printf("Wrong first message!\nDisconnecting...\n");
                                    close(fd);
                                    FD_CLR(fd, &client_socks);
                                }
                                clients[fd].state++;
                                break;
                            case 1:
                                received_number_from_client = atoi(buffer);
                                if(received_number_from_client == 2 * clients[fd].received_number){
                                    printf("OK\n");
                                }else{
                                    printf("WRONG\n");
                                }
                                clients[fd].state++;
                                break;
                            case 2:
                                printf("Case 2\n");
                                break;
                            default:
                                printf("Invalid state of user!\n");
                        }
                    } else {
                        close(fd);
                        FD_CLR(fd, &client_socks);
                        printf("Client disconnected and removed from the socket set\n");
                    }
                }
            }
        }
    }

    close(server_socket);
    return 0;
}
