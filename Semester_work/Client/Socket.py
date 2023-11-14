import socket

class Socket:
    def __init__(self):
        self.client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.user_state = 0
        print("[+] Socket created")

    def load_data(self, server_ip_address, server_port):
        self.server_ip_address = server_ip_address
        self.server_port = server_port

    def connect(self):
        print("[=] trying to connect")
        try:
            self.client_socket.connect((self.server_ip_address, self.server_port))
            return True
        except:
            return False

    def receive(self):
        print("Receiving")

    def send(self, message):
        print("Sending" + message)
