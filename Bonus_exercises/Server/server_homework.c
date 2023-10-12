#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_INPUT_SIZE 100
#define MAX_COMMAND_SIZE 50
#define START_CONNECTION "HELLO"

void create_message(char *str, int number){
    char random_number_string[20];
    sprintf(random_number_string, "NUM:%d\n", number);
    strcat(str, random_number_string);
}

int generate_random_number(){
    srand(time(NULL));
    return (rand() % 100) + 1;
}

void remove_newline(char *str) {
    int length = strlen(str);

    if (length > 0 && str[length - 1] == '\n') {
        str[length - 1] = '\0';
    }
}

int main(void){

    char input[MAX_INPUT_SIZE];
    char newCommand[MAX_COMMAND_SIZE];

    fgets(input, sizeof(input), stdin);
    remove_newline(input);

    if(!strcmp(input, START_CONNECTION)){
        printf("String %s a %s jsou stejne.\n", input, START_CONNECTION);
    }else{
        printf("String %s a %s jsou rozdilne.\n", input, START_CONNECTION);
    }

    int random_number = generate_random_number();

    create_message(newCommand, random_number);
    printf("New message: %s", newCommand);

    printf("Enter number: ");
    fgets(input, sizeof(input), stdin);
    remove_newline(input);
    printf("Entered: %s\n", input);
    int received = atoi(input);
    if(received == 2 * random_number){
        printf("OK\n");
    }else{
        printf("WRONG\n");
    }
    return 0;
}
