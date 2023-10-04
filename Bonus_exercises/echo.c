#include <stdio.h>
#include <string.h>

int main(void){
    char str1[] = "Hello world\n";
    char str2[100];

    printf("%s", str1);

    printf("Enter a string:\n");
    fgets(str2, sizeof(str2), stdin);
    printf("You entered: %s", str2);

    int result = strcmp(str1, str2);

    if(result == 0){
        printf("The string are equal!");
    } else{
        printf("The strings are not equal!");
    }
    return 0;
}