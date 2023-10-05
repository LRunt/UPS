/**
 * Client for reversing string received from server and sending it back
 *
 * @author Lukas Runt
 * @date 05-10-2023
 */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>

#define MAX_BUFFER_SIZE 255

void reverseString(char* string){
    //length -1 because of \n as last char
    int length = strlen(string) - 1;

    for(int i = 0; i < length /2; i++){
        char temp = string[i];
        string[i] = string[length - (i + 1)];
        string[length - (i +1)] = temp;
    }
}

int main(void){

    int client_socket;
    int return_value;
    char buffer[MAX_BUFFER_SIZE];
    struct sockaddr_in my_addr;

    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    memset(&my_addr, 0, sizeof(struct sockaddr_in));

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(2002);
    my_addr.sin_addr.s_addr = inet_addr("147.228.67.67");

    return_value = connect(client_socket, (struct sockaddr *)&my_addr,sizeof(struct sockaddr_in));
    if (return_value == 0)
        printf("Connect - OK\n");
    else {
        printf("Connect - ERR\n");
        return -1;
    }

    //receiving string
    int bytes_received = recv(client_socket, buffer, MAX_BUFFER_SIZE - 1, 0);
    if(bytes_received == -1){
        printf("Error in receiving data\n");
        return -1;
    }
    printf("Received from server: %s", buffer);

    //reversing received string
    reverseString(buffer);

    //sending reversed string back
    send(client_socket, buffer, strlen(buffer), 0);
    printf("Sent: %s", buffer);

    bytes_received = recv(client_socket, buffer, MAX_BUFFER_SIZE - 1, 0);
    if(bytes_received == -1){
        printf("Error in receiving data\n");
        return -1;
    }
    printf("Received from server: %s", buffer);

    close(client_socket);

    return 0;
}
