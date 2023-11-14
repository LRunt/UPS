from PyQt5.QtWidgets import QWidget, QVBoxLayout, QPushButton, QStackedWidget
import Scenes
import Socket

scenes = {
    "Login": 0,
    "Lobby": 1,
    "Waiting": 2,
    "Game": 3,
    "Result": 4
}


def convert_string_to_integer(string):
    try:
        integer_value = int(string)
        return integer_value
    except ValueError:
        return -1


class MainWindow(QWidget):
    def __init__(self):
        super().__init__()
        self.socket = Socket.Socket()
        self.socket.signals.message_received.connect(self.handle_received_message)
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
        server_port = convert_string_to_integer(self.login_scene.text_field_port.text())
        username = self.login_scene.text_field_username.text()
        if server_ip_address == "" or server_port == "" or username == "":
            print("Error: Some fields are not filed!")
            #TODO message error box
        elif server_port == -1:
            print("Error: Port address is not a number")
        else:
            print("IP address: ", end="")
            print(self.login_scene.text_field_IP_address.text())
            print("Port: ", end="")
            print(self.login_scene.text_field_port.text())
            print("Username: ", end="")
            print(self.login_scene.text_field_username.text())
            try:
                self.socket.load_data(server_ip_address, server_port)
                self.socket.connect()
                #client_socket.connect((server_ip_address, server_port))
                message = f"LOGIN|{username}"
                self.socket.send(message)
                #client_socket.send(message.encode())
                self.stacked_widget.setCurrentIndex(scenes["Lobby"])
            except Exception as e:
                print("Error: connection failed!", str(e))

    def handle_received_message(self, message):
        print("Received message: " + message)
        #TODO handle message
