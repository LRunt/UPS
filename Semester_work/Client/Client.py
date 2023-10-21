import socket


if __name__ == "__main__":
    server_ip = "127.0.0.1"  # Change this to the IP address of your server
    server_port = 10000  # Change this to the port your server is listening on

    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect((server_ip, server_port))

    while True:
        message = input("Enter a message (or 'exit' to quit): ")
        if message == 'exit':
            break

        client_socket.send(message.encode())

        response = client_socket.recv(1024)
        print("Received: " + response.decode())

    client_socket.close()