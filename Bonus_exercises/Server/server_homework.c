#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#define MAX_BUFFER_SIZE 100
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

    int port = 10000;
    int server_socket;
    int client_socket, fd;
    int return_value;
    int len_addr;
    int a2read;
    char input[MAX_BUFFER_SIZE];
    char newCommand[MAX_COMMAND_SIZE];
    char buffer[MAX_BUFFER_SIZE];
    struct sockaddr_in my_addr, peer_addr;
    fd_set client_socks, tests;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    memset(&my_addr, 0, sizeof(struct sockaddr_in));

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(port);
    my_addr.sin_addr.s_addr = INADDR_ANY;

    return_value = bind(server_socket, (struct sockaddr *) &my_addr, \
		sizeof(struct sockaddr_in));

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
        printf("Listen - ER\n");
    }

    // vyprazdnime sadu deskriptoru a vlozime server socket
    FD_ZERO( &client_socks );
    FD_SET( server_socket, &client_socks );

    for (;;){
        tests = client_socks;
        // sada deskriptoru je po kazdem volani select prepsana sadou deskriptoru kde se neco delo
        return_value = select( FD_SETSIZE, &tests, ( fd_set *)0, ( fd_set *)0, ( struct timeval *)0 );

        if (return_value < 0) {
            printf("Select - ERR\n");
            return -1;
        }
        // vynechavame stdin, stdout, stderr
        for( fd = 3; fd < FD_SETSIZE; fd++ ){
            // je dany socket v sade fd ze kterych lze cist ?
            if( FD_ISSET( fd, &tests ) ){
                // je to server socket ? prijmeme nove spojeni
                if (fd = server_socket){
                    client_socket = accept(server_socket, (struct sockaddr *) &peer_addr, &len_addr);
                    FD_SET( client_socket, &client_socks );
                    printf("New client!\n");
                    int bytes_received = recv(fd, buffer, MAX_BUFFER_SIZE - 1, 0);
                    if(bytes_received == -1){
                        printf("ERROR");
                        close(fd); //uzaviram spojeni s clientem
                        FD_CLR(fd, &client_socks);
                    }
                    remove_newline(buffer);
                    printf("Recieved: %s", buffer);

                    if(!strcmp(input, START_CONNECTION)){
                        printf("String %s a %s jsou stejne.\n", input, START_CONNECTION);
                    }else{
                        printf("String %s a %s jsou rozdilne.\n", input, START_CONNECTION);
                    }
                }
                    // je to klientsky socket ? prijmem data
                else {
                    // pocet bajtu co je pripraveno ke cteni
                    ioctl( fd, FIONREAD, &a2read );
                    // mame co cist
                    if (a2read > 0){
                        int bytes_received = recv(fd, buffer, MAX_BUFFER_SIZE - 1, 0);
                        if(bytes_received == -1){
                            printf("ERROR");
                            close(fd); //uzaviram spojeni s clientem
                            FD_CLR(fd, &client_socks);
                        }
                        remove_newline(buffer);
                        printf("Recieved: %s", buffer);

                        if(!strcmp(input, START_CONNECTION)){
                            printf("String %s a %s jsou stejne.\n", input, START_CONNECTION);
                        }else{
                            printf("String %s a %s jsou rozdilne.\n", input, START_CONNECTION);
                        }
                    }
                        // na socketu se stalo neco spatneho
                    else {
                        close(fd);
                        FD_CLR( fd, &client_socks );
                        printf("Klient se odpojil a byl odebran ze sady socketu\n");
                    }
                }
            }
        }
    }



    //fgets(input, sizeof(input), stdin);
    //remove_newline(input);

    /*if(!strcmp(input, START_CONNECTION)){
        printf("String %s a %s jsou stejne.\n", input, START_CONNECTION);
    }else{
        printf("String %s a %s jsou rozdilne.\n", input, START_CONNECTION);
    }*/

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
