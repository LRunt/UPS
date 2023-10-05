#include <stdio.h>
#include <string.h>

#define MAX_BUFFER_SIZE 255

int main(void){

    char str1[MAX_BUFFER_SIZE];

    printf("Enter a string:\n");
    fgets(str1, sizeof(str1), stdin);

    strrev(str1);

    printf("%s\n", str1);

    return 0;
}
