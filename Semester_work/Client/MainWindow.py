from PyQt5.QtWidgets import QWidget, QVBoxLayout, QPushButton, QStackedWidget
import socket
import Scenes

scenes = {
    "Login": 0,
    "Lobby": 1,
    "Waiting": 2,
    "Game": 3,
    "Result": 4
}

class MainWindow(QWidget):
    def __init__(self):
        super().__init__()
        self.login_scene = Scenes.LoginScene()
        self.initUI()

    def initUI(self):
        self.login_scene.login_button.clicked.connect(self.login)
        # Create button for the second scene
        back_button = QPushButton('Back to First Scene', self)
        back_button.clicked.connect(self.switch_scene)

        # Create layout for the second scene
        layout2 = QVBoxLayout()
        layout2.addWidget(back_button)

        # Create widget for the second scene
        self.second_scene_widget = QWidget()
        self.second_scene_widget.setLayout(layout2)

        # Create stacked widget to manage different scenes
        self.stacked_widget = QStackedWidget(self)
        self.stacked_widget.addWidget(self.login_scene.login_widget)
        self.stacked_widget.addWidget(self.second_scene_widget)

        # Set up the main layout
        main_layout = QVBoxLayout(self)
        main_layout.addWidget(self.stacked_widget)

        self.setGeometry(300, 300, 300, 200)
        self.setWindowTitle('TIC-TAC-TOE')

    def switch_scene(self):
        current_index = self.stacked_widget.currentIndex()
        new_index = (current_index + 1) % self.stacked_widget.count()
        self.stacked_widget.setCurrentIndex(new_index)

    def login(self):
        server_ip_address = self.login_scene.text_field_IP_address.text()
        server_port = self.login_scene.text_field_port.text()
        username = self.login_scene.text_field_username.text()
        if server_ip_address == "" or server_port == "" or username == "":
            print("Error: Some fields are not filed!")
            #TODO message error box
        else:
            print("IP address: ", end="")
            print(self.login_scene.text_field_IP_address.text())
            print("Port: ", end="")
            print(self.login_scene.text_field_port.text())
            print("Username: ", end="")
            print(self.login_scene.text_field_username.text())
            try:
                client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                client_socket.connect((server_ip_address, server_port))
                message = f"LOGIN|{username}"
                client_socket.send(message.encode())
                self.stacked_widget.setCurrentIndex(scenes["Lobby"])
            except Exception as e:
                print("Error: connection failed!")


    """
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
    """
