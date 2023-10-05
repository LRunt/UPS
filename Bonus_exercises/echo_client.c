#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>

#define MAX_BUFFER_SIZE 255

/**
 * Program for connecting server, sending string, receive string and compare these two strings
 *
 * @author Lukas Runt
 * @date 05-10-2023
 * @return 0 - Program ends success, -1 Error
 */
int main(void){

    int client_socket;
    int return_value;
    char buffer[MAX_BUFFER_SIZE];
    char user_input[MAX_BUFFER_SIZE];
    struct sockaddr_in my_addr;

    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    memset(&my_addr, 0, sizeof(struct sockaddr_in));

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(2001);
    my_addr.sin_addr.s_addr = inet_addr("147.228.67.67");

    return_value = connect(client_socket, (struct sockaddr *)&my_addr,sizeof(struct sockaddr_in));
    if (return_value == 0)
        printf("Connect - OK\n");
    else {
        printf("Connect - ERR\n");
        return -1;
    }

    printf("Enter a string:\n");
    fgets(user_input, sizeof(user_input), stdin);

    send(client_socket, user_input, strlen(user_input), 0);
    printf("Sent: %s", user_input);

    int bytes_received = recv(client_socket, buffer, MAX_BUFFER_SIZE - 1, 0);
    if(bytes_received == -1){
        printf("Error in receiving data\n");
        return -1;
    }
    buffer[bytes_received] = '\0';
    printf("Received from server: %s", buffer);

    int result = strcmp(user_input, buffer);

    if(result == 0){
        printf("The string are equal!\n");
    } else{
        printf("The strings are not equal!\n");
    }

    return 0;
}
