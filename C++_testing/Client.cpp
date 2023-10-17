#include <iostream>
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
    if (connect(client_socket, reinterpret_cast<struct sockaddr *>(&server_addr), sizeof(server_addr)) == -1) {
        perror("Connection failed");
        close(client_socket);
        return -1;
    }

    std::cout << "Connected to server." << std::endl;

    while (1) {
        std::cout << "Enter a string to send (or 'exit' to quit): ";
        std::cin.getline(buffer, MAX_BUFFER_SIZE);

        // Send the input string to the server
        send(client_socket, buffer, strlen(buffer), 0);

        // If the user wants to exit, break the loop
        if (strcmp(buffer, "exit") == 0) {
            std::cout << "Exiting." << std::endl;
            break;
        }

        // Receive and display the echoed string from the server
        int bytes_received = recv(client_socket, buffer, MAX_BUFFER_SIZE, 0);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            std::cout << "Received from server: " << buffer << std::endl;
        }
    }

    // Close the socket and exit
    close(client_socket);
    return 0;
}