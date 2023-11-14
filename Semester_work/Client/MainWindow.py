from PyQt5.QtWidgets import QWidget, QVBoxLayout, QPushButton, QStackedWidget
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

    def login(self):
        print("IP address: ", end="")
        print(self.login_scene.text_field_IP_address.text())
        print("Port: ", end="")
        print(self.login_scene.text_field_port.text())
        print("Username: ", end="")
        print(self.login_scene.text_field_username.text())
        self.stacked_widget.setCurrentIndex(scenes["Lobby"])

    def switch_scene(self):
        current_index = self.stacked_widget.currentIndex()
        new_index = (current_index + 1) % self.stacked_widget.count()
        self.stacked_widget.setCurrentIndex(new_index)
