#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>

#define MAX_BUFFER_SIZE 1024

int main(void){
    int client_socket;
    int return_value;
    char buffer[MAX_BUFFER_SIZE];
    int len_addr;
    struct sockaddr_in my_addr;

    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    memset(&my_addr, 0, sizeof(struct sockaddr_in));

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(2000);
    my_addr.sin_addr.s_addr = inet_addr("147.228.67.67");

    return_value = connect(client_socket, (struct sockaddr *)&my_addr,sizeof(struct sockaddr_in));
    if (return_value == 0)
        printf("Connect - OK\n");
    else {
        printf("Connect - ERR\n");
        return -1;
    }

    int bytes_received = recv(client_socket, buffer, MAX_BUFFER_SIZE - 1, 0);

    if (bytes_received == -1) {
        printf("Error in receiving data\n");
        return(EXIT_FAILURE);
    }

    buffer[bytes_received] = '\0';  // Null-terminate the received data to make it a valid string
    printf("Received from server:\n%s", buffer);

    bytes_received = recv(client_socket, buffer, MAX_BUFFER_SIZE - 1, 0);

    if (bytes_received == -1) {
        printf("Error in receiving data\n");
        return(EXIT_FAILURE);
    }

    buffer[bytes_received] = '\0';  // Null-terminate the received data to make it a valid string
    printf("%s\n", buffer);

    send(client_socket, "plus|10|10\n", strlen("plus|10|10"), 0);
    printf("Poslano: plus|10|10\n");

    bytes_received = recv(client_socket, buffer, MAX_BUFFER_SIZE - 1, 0);

    if (bytes_received == -1) {
        printf("Error in receiving data\n");
        return(EXIT_FAILURE);
    }

    buffer[bytes_received] = '\0';  // Null-terminate the received data to make it a valid string
    printf("Received from server: %s\n", buffer);

    close(client_socket);

    return 0;
}