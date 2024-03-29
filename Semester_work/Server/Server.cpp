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
#include "Constants.h"
#include <array>

/**
 * Method counts number of chars in string
 * @param input string where the characters will be counted
 * @param character character what we are counting
 * @return number of characters in the string
 */
int count_characters(const std::string& input, char character) {
    int count = 0;
    for (char c : input) {
        if (c == character) {
            count++;
        }
    }
    return count;
}

/**
 * Method splits string by character
 * @param input string what will we split
 * @return vector of strings
 */
std::vector<std::string> split_string_by_newline(const std::string& input) {
    std::vector<std::string> result;
    if(input == "\n"){
        Logger::instance().log(LogLevel::INFO, "Only \\n in input.");
        result.push_back(MESSAGE_NEW_LINE);
        return result;
    }
    std::istringstream iss(input);

    // Split the string by newline character
    for (std::string line; std::getline(iss, line, END_OF_MESSAGE); ) {
        if(line.size() > 0){
            result.push_back(line);
        }
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
    std::array<std::string, DEFAULT_MAX_USERS + NUMBER_OF_STREAMS> messages;

    // Set the log file
    Logger& logger = Logger::instance();
    logger.setLogFile(NAME_OF_LOGFILE);

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
                    if(number < MINIMAL_NUMBER_OF_USERS){
                        logger.log(LogLevel::ERROR, "Minimal number of players are: " + std::to_string(MINIMAL_NUMBER_OF_USERS));
                        return EXIT_FAILURE;
                    }else{
                        max_number_of_connected_users = number;
                        logger.log(LogLevel::INFO, "Setting maximal number of clients: " + std::to_string(number));
                    }
                    break;
                case 'p':
                    if(number < MINIMAL_PORT_NUMBER || MAXIMAL_PORT_NUMBER < number){
                        logger.log(LogLevel::ERROR, "Port out of bounds. Minimal port number: " + std::to_string(MINIMAL_PORT_NUMBER) + ", Maximal port number: " + std::to_string(MAXIMAL_PORT_NUMBER));
                        return EXIT_FAILURE;
                    }else{
                        port = number;
                        logger.log(LogLevel::INFO, "Setting port number: " + std::to_string(number));
                    }
                    break;
                default:
                    logger.log(LogLevel::ERROR, &"Wrong flag!" [ flag]);
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
                    logger.log(LogLevel::INFO, "New client: " + std::to_string(client_socket));
                    // Testing maximal number of connected users
                    if(client_socket > (max_number_of_connected_users + NUMBER_OF_STREAMS)){
                        logger.log(LogLevel::WARNING, "Server is full, new client: " + std::to_string(client_socket) + "is not accepted.");
                        send(client_socket, MESSAGE_MAX_USERS.c_str(), MESSAGE_MAX_USERS.size(), 0);
                        close(client_socket);
                        FD_CLR(client_socket, &client_socks);
                    }else{
                        FD_SET(client_socket, &client_socks);
                        logger.log(LogLevel::INFO, "New client connected, fd: " + std::to_string(client_socket));
                        messages[client_socket] = "";
                    }
                } else {
                    ioctl(fd, FIONREAD, &a2read);
                    if (a2read > 0) {
                        // Receive data into the buffer
                        int bytes_received = recv(fd, buffer, MAX_BUFFER_SIZE, 0);
                        if (bytes_received > 0 && bytes_received < MAX_BUFFER_SIZE) {
                           buffer[bytes_received] = '\0';  // Null-terminate the received data (assuming it's a string)

                            std::string message(buffer, bytes_received);
				            logger.log(LogLevel::INFO, "Received message: " + message + ", from fd: " + std::to_string(fd));
                            messages[fd] += message;
                            int number_of_messages = count_characters(messages[fd], END_OF_MESSAGE);
                            logger.log(LogLevel::INFO, "Starting parsing messages.");
                            std::vector<std::string> messages_to_execute = split_string_by_newline(messages[fd]);
                            if(messages_to_execute.size() == number_of_messages){
                                messages[fd] = "";
                            }else{
                                messages[fd] = messages_to_execute.back();
                            }
                            logger.log(LogLevel::INFO, "Starting executing messages.");
                            for(int i = 0; i < number_of_messages; i++){
                                std::string response = User::execute_message(messages_to_execute[i], fd);
                                if (response == MESSAGE_EXIST_ONLINE_USER || response == MESSAGE_ILLEGAL_CHARS_IN_USERNAME || response == MESSAGE_SHORT_USERNAME || response == MESSAGE_LONG_USERNAME){
                                    response += END_OF_MESSAGE;
                                    logger.log(LogLevel::INFO, "Sending response: " + response + ", to the fd: " + std::to_string(fd));
                                    send(fd, response.c_str(), static_cast<int>(response.size()), 0);
                                    logger.log(LogLevel::WARNING, "Login failed - disconnecting user");
                                    close(fd);
                                    FD_CLR(fd, &client_socks);
                                }else if (response == MESSAGE_ERROR){
                                    logger.log(LogLevel::WARNING, "Invalid message - disconnecting user");
                                    close(fd);
                                    FD_CLR(fd, &client_socks);
                                }else{
                                    logger.log(LogLevel::INFO, "Sending response: " + response + ", to the fd: " + std::to_string(fd));
                                    response += END_OF_MESSAGE;
                                    // Handling the case where the send operation failed (connection lost)
                                    if (send(fd, response.c_str(), static_cast<int>(response.size()), 0) == -1) {
                                        close(fd);
                                        FD_CLR(fd, &client_socks);
                                        logger.log(LogLevel::WARNING, "Connection lost for fd: " + std::to_string(fd));
                                    }
                                }
                            }
                        } else{
                            std::shared_ptr<User> user = User::get_user_by_fd(fd);
                            user->set_user_disconnected();
                            close(fd);
                            FD_CLR(fd, &client_socks);
                            logger.log(LogLevel::INFO, "User" + user->mUsername + " fd(" + std::to_string(user->mFd) + ") disconnected and removed from the socket set");
                        }
                    } else {
                        // Setting user disconnected
                        std::shared_ptr<User> user = User::get_user_by_fd(fd);
                        user->set_user_disconnected();
                        close(fd);
                        FD_CLR(fd, &client_socks);
                        logger.log(LogLevel::INFO, "User" + user->mUsername + " fd(" + std::to_string(user->mFd) + ") disconnected and removed from the socket set");
                    }
                }
            }
        }
    }

    close(server_socket);
    return EXIT_SUCCESS;
}
