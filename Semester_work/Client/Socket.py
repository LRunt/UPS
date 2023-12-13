"""
Author: Lukas Runt
Email: lrunt@students.zcu.cz
Date: 14.11.2023
Version: 0.1.0
Description: Module for communication with server
"""
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

    def load_data(self, server_ip_address, server_port):
        """
        Method for setting the parameters for connecting the server
        :param server_ip_address: ip address of server
        :param server_port: port where server runs
        """
        self.server_ip_address = server_ip_address
        self.server_port = server_port

    def connect(self):
        """
        Method for connecting to the server
        """
        print("Trying to connect")
        try:
            self.client_socket.connect((self.server_ip_address, self.server_port))
            print("Connected!")

            self.receive_thread = threading.Thread(target=self.receive)
            self.receive_thread.start()

        except Exception as e:
            print("Error: Connection failed!", str(e))
            raise

    def receive(self):
        """
        Method for receiving messages from server
        """
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
        """
        Method sends message to the server
        :param message: message what will be sent
        """
        try:
            self.client_socket.send(message.encode())
            print("Message sent: ", message)
        except Exception as e:
            print("Error sending message: ", str(e))
            raise

    def disconnect(self):
        """
        Method for disconnecting from the server
        """
        try:
            self.client_socket.close()
            print("Disconnected!")

            if hasattr(self, 'receive_thread') and self.receive_thread.is_alive():
                self.receive_thread._stop()

        except Exception as e:
            print("Error disconnecting: ", str(e))
            raise

