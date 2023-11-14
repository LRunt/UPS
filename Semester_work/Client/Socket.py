import socket
import threading
from PyQt5.QtCore import QObject, pyqtSignal

class SocketSignals(QObject):
    message_received = pyqtSignal(str)

class Socket:
    def __init__(self):
        self.client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.signals = SocketSignals()
        self.server_ip_address = ""
        self.server_port = 0
        self.user_state = 0
        print("[+] Socket created")

    def load_data(self, server_ip_address, server_port):
        self.server_ip_address = server_ip_address
        self.server_port = server_port

    def connect(self):
        print("[=] trying to connect")
        try:
            self.client_socket.connect((self.server_ip_address, self.server_port))
            print("[+] Connected!")

            self.receive_thread = threading.Thread(target=self.receive)
            self.receive_thread.start()

        except Exception as e:
            print("Error: Connection failed!", str(e))
            raise


    def receive(self):
        try:
            while True:
                message = self.client_socket.recv(1024).decode()
                self.signals.message_received.emit(message)
        except Exception as e:
            print("Error receiving message: ", str(e))
            raise
        finally:
            self.client_socket.close()


    def send(self, message):
        try:
            self.client_socket.send(message.encode())
            print("Message sent: ", message);
        except Exception as e:
            print("Error sending message: ", str(e))
            raise
