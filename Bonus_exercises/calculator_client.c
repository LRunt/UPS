#include <stdio.h>
#include <string.h>
#include <ctype.h>

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

void removeNewline(char *str) {
    int length = strlen(str);

    if (length > 0 && str[length - 1] == '\n') {
        str[length - 1] = '\0';
    }
}

void create_command(char* result, char* operation, char* operator1, char* operator2, char delimiter){
    sprintf(result, "%s%c%s%c%s", operation, delimiter, operator1, delimiter, operator2);
}

int main(void){

    char operation[MAX_BUFFER_SIZE];
    char operator1[MAX_BUFFER_SIZE];
    char operator2[MAX_BUFFER_SIZE];

    int isValid = false;
    while(!isValid){
        printf("Enter operation:\n");
        fgets(operation, sizeof(operation), stdin);
        printf("Entered: %s", operation);
        removeNewline(operation);
        isValid = validate_input_operation(operation);
    }


    isValid = false;
    while(!isValid){
        printf("Enter first operator:\n");
        fgets(operator1, sizeof(operator1), stdin);
        printf("Entered: %s", operator1);
        removeNewline(operator1);
        isValid = validate_input_operator(operator1);
    }

    isValid = false;
    while(!isValid){
        printf("Enter second operation:\n");
        fgets(operator2, sizeof(operator2), stdin);
        printf("Entered: %s", operator2);
        removeNewline(operator2);
        isValid = validate_input_operator(operator2);
    }

    int size_of_result = strlen(operation) + strlen(operator1) + strlen(operator2) + 2; //2 delimiters
    char result[size_of_result];
    create_command(result, operation, operator1, operator2, '|');

    printf("%s\n", result);

    return 0;
}
