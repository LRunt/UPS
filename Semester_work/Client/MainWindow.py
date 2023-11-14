from PyQt5.QtWidgets import QWidget, QLabel, QLineEdit, QVBoxLayout, QPushButton, QStackedWidget
import LoginScene

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
        self.login_scene = LoginScene.LoginScene()
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
        self.stacked_widget.addWidget(login_scene.get_scene())
        self.stacked_widget.addWidget(self.second_scene_widget)

        # Set up the main layout
        main_layout = QVBoxLayout(self)
        main_layout.addWidget(self.stacked_widget)

        self.setGeometry(300, 300, 300, 200)
        self.setWindowTitle('TIC-TAC-TOE')

    def login(self):
        print("IP address: ")
        self.stacked_widget.setCurrentIndex(scenes["Lobby"])

    def switch_scene(self):
        current_index = self.stacked_widget.currentIndex()
        new_index = (current_index + 1) % self.stacked_widget.count()
        self.stacked_widget.setCurrentIndex(new_index)
