/**
 * Client for sending data to calculator server and validating its result
 *
 * @author Lukas Runt
 * @date 06-10-2023
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>

typedef int bool;

#define true 1
#define false 0
#define MAX_BUFFER_SIZE 100
#define plus "plus"
#define minus "minus"
#define division "division"
#define multiply "multiply"

bool validate_input_operation(char* user_input){
    if (strcmp(plus, user_input) == 0 || strcmp(minus, user_input) == 0 || strcmp(multiply, user_input) == 0 || strcmp(division, user_input) == 0){
        return true;
    }else{
        fprintf(stderr, "Error! Invalid operation!\n");
        printf("Valid operation are: plus|minus|multiply|division\n");
        return false;
    }
}

bool validate_input_operator(char* user_input){
    int length = strlen(user_input);

    for (int i = 0; i < length; i++) {
        if (!isdigit(user_input[i])) {
            fprintf(stderr, "Error! Invalid operator!\n");
            printf("Only numbers are valid!\n");
            return false;
        }
    }
    return true;
}

void remove_newline(char *str) {
    int length = strlen(str);

    if (length > 0 && str[length - 1] == '\n') {
        str[length - 1] = '\0';
    }
}

void input_and_valid_digit(char* operator, int number){
    bool isValid = false;
    while(!isValid){
        printf("Enter %d. operator: ", number);
        fgets(operator, sizeof(operator), stdin);
        remove_newline(operator);
        isValid = validate_input_operator(operator);
    }
}

/**
 * Method message in format for server (format: Operation|Operator|Operator)
 */
void create_command(char* result, char* operation, char* operator1, char* operator2, char delimiter){
    sprintf(result, "%s%c%s%c%s\n", operation, delimiter, operator1, delimiter, operator2);
}

int main(void){

    char operation[MAX_BUFFER_SIZE];
    char operator1[MAX_BUFFER_SIZE];
    char operator2[MAX_BUFFER_SIZE];
    char buffer[MAX_BUFFER_SIZE];
    int client_socket;
    int return_value;
    struct sockaddr_in my_addr;

    //connecting to server
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
        return -1;
    }

    buffer[bytes_received] = '\0';  // Null-terminate the received data to make it a valid string
    printf("%s", buffer);

    bytes_received = recv(client_socket, buffer, MAX_BUFFER_SIZE - 1, 0);

    if (bytes_received == -1) {
        printf("Error in receiving data\n");
        return -1;
    }

    buffer[bytes_received] = '\0';  // Null-terminate the received data to make it a valid string
    printf("%s\n", buffer);

    int isValid = false;
    while(!isValid){
        printf("Enter operation: ");
        fgets(operation, sizeof(operation), stdin);
        remove_newline(operation);
        isValid = validate_input_operation(operation);
    }

    input_and_valid_digit(operator1, 1);
    input_and_valid_digit(operator2, 2);

    int size_of_result = strlen(operation) + strlen(operator1) + strlen(operator2) + 3; //2 delimiters and \n
    char result[size_of_result];
    create_command(result, operation, operator1, operator2, '|');


    printf("%s", result);

    send(client_socket, result, strlen(result), 0);
    printf("Sent: %s", result);

    bytes_received = recv(client_socket, buffer, MAX_BUFFER_SIZE - 1, 0);

    if (bytes_received == -1) {
        printf("Error in receiving data\n");
        return -1;
    }

    buffer[bytes_received] = '\0';  // Null-terminate the received data to make it a valid string
    printf("%s\n", buffer);

    close(client_socket);

    return 0;
}
