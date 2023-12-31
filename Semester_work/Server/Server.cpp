/**
 * Server for game TIC-TAC-TOE
 *
 * @author Lukas Runt
 * @date 27-10-2023
 * @version 0.0.2
 */

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <cstdlib>
#include <sys/ioctl.h>
#include <memory>

#include "Logger.h"
#include "User.h"
#include <array>

#define MAX_BUFFER_SIZE 1024
#define DEFAULT_PORT 10000
#define DEFAULT_MAX_USERS 10
#define NUMBER_OF_STREAMS 3 //(stdin,stdout, stderr)
#define MESSAGE_MAX_USERS ""

int countNewlines(const std::string& input) {
    int count = 0;
    for (char c : input) {
        if (c == '\n') {
            count++;
        }
    }
    return count;
}

std::vector<std::string> splitStringByNewline(const std::string& input) {
    std::vector<std::string> result;
    std::istringstream iss(input);

    // Split the string by newline character
    for (std::string line; std::getline(iss, line, '\n'); ) {
        result.push_back(line);
    }

    return result;
}

int main(int argc, char *argv[]){
    int port = DEFAULT_PORT;
    int max_number_of_connected_users = DEFAULT_MAX_USERS;
    int server_socket;
    int client_socket, fd;
    int return_value;
    char buffer[MAX_BUFFER_SIZE];
    socklen_t len_addr;
    int a2read;
    struct sockaddr_in my_addr, peer_addr;
    fd_set client_socks, tests;
    std::array<std::string, DEFAULT_MAX_USERS> messages;

    // Set the log file
    Logger& logger = Logger::instance();
    logger.setLogFile("my_log_file.log");

    // Reading and parse arguments
    if(argc > 3){
        logger.log(LogLevel::ERROR, "Invalid number of parameters: " + std::to_string(argc));
        logger.log(LogLevel::ERROR, "Parameters are:\n-c<number> = maximal number of connected users\n-p<number> = port");
        return EXIT_FAILURE;
    }

    for(int i = 1; i < argc; i++){
        const char *arg = argv[i];

        if(strlen(arg) >= 3 && arg[0] == '-'){
            char flag = arg[1]; //c or p
            char number_string[strlen(arg) - 1];

            strncpy(number_string, arg + 2, strlen(arg) - 2);
            number_string[strlen(arg) - 2] = '\0';
            int number;
            try{
                number = std::stoi(number_string);
            } catch (const std::invalid_argument& e) {
                logger.log(LogLevel::ERROR, "Invalid argument: " + std::string(e.what()));
                return EXIT_FAILURE;
            } catch (const std::out_of_range& e) {
                logger.log(LogLevel::ERROR, "Out of range: " + std::string(e.what()));
                return EXIT_FAILURE;
            }

            switch (flag){
                case 'c':
                    max_number_of_connected_users = number;
                    logger.log(LogLevel::INFO, "Maximal number of clients: " + std::to_string(number));
                    break;
                case 'p':
                    port = number;
                    logger.log(LogLevel::INFO, "Port number: " + std::to_string(number));
                    break;
                default:
                    logger.log(LogLevel::ERROR, "Wrong flag!");
                    return EXIT_FAILURE;
            }

        }else{
            logger.log(LogLevel::ERROR, "Invalid argument format: " + std::string(arg));
            return EXIT_FAILURE;
        }
    }

    // Defining server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    memset(&my_addr, 0, sizeof(struct sockaddr_in));

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(port);
    my_addr.sin_addr.s_addr = INADDR_ANY;

    return_value = bind(server_socket, (struct sockaddr *) &my_addr, sizeof(struct sockaddr_in));

    if (return_value == 0){
        logger.log(LogLevel::INFO, "Bind - OK");
    }
    else {
        logger.log(LogLevel::ERROR, "Bind - ERR");
        return -1;
    }

    return_value = listen(server_socket, 5);
    if (return_value == 0){
        logger.log(LogLevel::INFO, "Listen - OK");
    } else {
        logger.log(LogLevel::ERROR, "Listen - ERR");
        return -1;
    }

    FD_ZERO(&client_socks);
    FD_SET(server_socket, &client_socks);

    for (;;) {
        tests = client_socks;

        return_value = select(FD_SETSIZE, &tests, (fd_set *) 0, (fd_set *) 0, (struct timeval *) 0);

        if (return_value < 0) {
            logger.log(LogLevel::ERROR, "Select - ERR");
            return -1;
        }

        for (fd = NUMBER_OF_STREAMS; fd < FD_SETSIZE; fd++) {
            if (FD_ISSET(fd, &tests)) {
                if (fd == server_socket) {
                    client_socket = accept(server_socket, (struct sockaddr *) &peer_addr, &len_addr);
                    // Testing maximal number of connected users
                    if(client_socket > (max_number_of_connected_users - NUMBER_OF_STREAMS)){
                        send(client_socket, MESSAGE_MAX_USERS, strlen(MESSAGE_MAX_USERS), 0);
                    }else{
                        FD_SET(client_socket, &client_socks);
                        logger.log(LogLevel::INFO, "File descriptor fd: " + std::to_string(client_socket));
                        messages[client_socket] = "";
                    }
                } else {
                    ioctl(fd, FIONREAD, &a2read);
                    if (a2read > 0) {
                        // Receive data into the buffer
                        int bytes_received = recv(fd, buffer, MAX_BUFFER_SIZE, 0);
                        if (bytes_received > 0) {
                           buffer[bytes_received] = '\0';  // Null-terminate the received data (assuming it's a string)

                            std::string message(buffer, bytes_received + 1);
				            logger.log(LogLevel::INFO, "Received message: " + message);
			                logger.log(LogLevel::INFO, "File descriptor: " + std::to_string(fd));
				            logger.log(LogLevel::INFO, "Message before: " + messages[fd]);
                            messages[fd] += message;
				            logger.log(LogLevel::INFO, "Message after: " + messages[fd]);
                            int number_of_messages = countNewlines(messages[fd]);
                            std::vector<std::string> messages_to_execute = splitStringByNewline(messages[fd]);
                            if(messages_to_execute.size() == number_of_messages){
                                messages[fd] = "";
                            }else{
                                messages[fd] = messages_to_execute.back();
                            }
                            for(int i = 0; i < number_of_messages; i++){
                                logger.log(LogLevel::INFO, "Message in for cycle: " + messages_to_execute[i]);
                                std::string response = User::execute_message(messages_to_execute[i], fd);
                                if (response == "LOGIN|2" || response == "LOGIN|3" || response == "LOGIN|4" || response == "LOGIN|5"){
                                    logger.log(LogLevel::ERROR, "Login failed - disconnecting user")
                                    send(fd, response.c_str(), static_cast<int>(response.size()), 0);
                                    close(fd);
                                    FD_CLR(fd, &client_socks);
                                }else if (response == "ERROR"){
                                    logger.log(LogLevel::ERROR, "Response ERROR")
                                    close(fd);
                                    FD_CLR(fd, &client_socks);
                                }else{
                                    logger.log(LogLevel::INFO, "Sending response: " + response);
                                    send(fd, response.c_str(), static_cast<int>(response.size()), 0);
                                }
                            }
                        }
                    } else {
                        // Setting user disconnected
                        std::shared_ptr<User> user = User::get_user_by_fd(fd);
                        user->set_user_disconnected();
                        close(fd);
                        FD_CLR(fd, &client_socks);
                        logger.log(LogLevel::INFO, "User disconnected and removed from the socket set");
                    }
                }
            }
        }
    }

    close(server_socket);
    return 0;
}
