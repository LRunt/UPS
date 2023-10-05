#include <stdio.h>
#include <string.h>

#define MAX_BUFFER_SIZE 255

void reverseString(char* string){
    int length = strlen(string);

    for(int i = 0; i < length /2; i++){
        char temp = string[i];
        string[i] = string[length - (i + 1)];
        string[length - (i +1)] = temp;
    }
}

int main(void){

    char str1[MAX_BUFFER_SIZE];

    printf("Enter a string:\n");
    fgets(str1, sizeof(str1), stdin);

    reverseString(str1);

    printf("Reversed string: %s", str1);

    return 0;
}
