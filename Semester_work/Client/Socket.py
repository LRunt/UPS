import socket
import threading
from Logger import logger
from PyQt5.QtCore import QObject, pyqtSignal

CONNECTION_TIMEOUT = 5 #seconds


class SocketSignals(QObject):
    message_received = pyqtSignal(str)


class Socket:
    def __init__(self):
        self.client_socket = None
        self.signals = SocketSignals()
        self.server_ip_address = ""
        self.server_port = 0
        self.connection = False
        self.receive_thread = None

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
        logger.info("Trying to connect ...")
        logger.info(f"server IP: {self.server_ip_address}, Port: {self.server_port}")
        try:
            self.client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.client_socket.settimeout(CONNECTION_TIMEOUT)
            self.client_socket.connect((self.server_ip_address, self.server_port))
            logger.info("Connection success")
            self.connection = True

            self.receive_thread = threading.Thread(target=self.receive)
            self.receive_thread.start()

        except Exception as e:
            logger.error(f"Connection failed: {str(e)}")
            raise

    def receive(self):
        """
        Method for receiving messages from server
        """
        try:
            while True:
                message = self.client_socket.recv(1024).decode()
                if not message:
                    break
                logger.info(f"Received message: {message}")
                self.signals.message_received.emit(message)
        except Exception as e:
            logger.error(f"Receiving message failed: {str(e)}")
        finally:
            logger.info("Connection lost")
            self.disconnect()

    def send(self, message):
        """
        Method sends message to the server
        :param message: message what will be sent
        """
        try:
            self.client_socket.send(message.encode())
            logger.info(f"Sending message: {message}")
        except Exception as e:
            logger.error(f"Sending message failed: {str(e)}")
            raise

    def disconnect(self):
        """
        Method for disconnecting from the server
        """
        try:
            self.client_socket.close()
            self.receive_thread.join()
            self.connection = False
        except Exception as e:
            logger.error(f"Disconnecting failed: {str(e)}")
            raise
        finally:
            logger.info("Disconnect success")
