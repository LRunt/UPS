#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_BUFFER_SIZE 1024
#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT 10000

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[MAX_BUFFER_SIZE];

    // Create a socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Socket creation failed");
        return -1;
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDRESS, &server_addr.sin_addr);

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        close(client_socket);
        return -1;
    }

    printf("Connected to server.\n");

    while (1) {
        printf("Enter a string to send (or 'exit' to quit): ");
        fgets(buffer, MAX_BUFFER_SIZE, stdin);

        // Remove newline character at the end of the input
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }

        // Send the input string to the server
        send(client_socket, buffer, strlen(buffer), 0);

        // If the user wants to exit, break the loop
        if (strcmp(buffer, "exit") == 0) {
            printf("Exiting.\n");
            break;
        }

        // Receive and display the echoed string from the server
        int bytes_received = recv(client_socket, buffer, MAX_BUFFER_SIZE, 0);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            printf("Received from server: %s\n", buffer);
        }
    }

    // Close the socket and exit
    close(client_socket);
    return 0;
}