from PyQt5.QtWidgets import QWidget, QLabel, QLineEdit, QVBoxLayout, QPushButton


class LoginScene(QWidget):
    def __init__(self):
        super().__init__()
        self.text_field_IP_address = QLineEdit(self)
        self.text_field_port = QLineEdit(self)
        self.text_field_username = QLineEdit(self)
        self.login_button = QPushButton('Login', self)
        self.initUI()

    def initUI(self):
        # Create labels and text fields for the first scene
        label_IP_address = QLabel('IP address:')
        label_port = QLabel('Port:')
        label_username = QLabel('Username:')

        # Create button to switch to the next scene
        # switch_button.clicked.connect(self.login_widget)

        # Create layout for the first scene
        login_layout = QVBoxLayout()
        login_layout.addWidget(label_IP_address)
        login_layout.addWidget(self.text_field_IP_address)
        login_layout.addWidget(label_port)
        login_layout.addWidget(self.text_field_port)
        login_layout.addWidget(label_username)
        login_layout.addWidget(self.text_field_username)
        login_layout.addWidget(self.login_button)

        # Create widget for the first scene
        self.login_widget = QWidget()
        self.login_widget.setLayout(login_layout)


class LobbyScene(QWidget):
    def __init__(self):
        super().__init__()
        self.label_user = QLabel()
        self.start_game_button = QPushButton('Start', self)
        self.disconnect_button = QPushButton('Disconnect', self)
        self.initUI()

    def initUI(self):
        lobby_layout = QVBoxLayout()
        lobby_layout.addWidget(self.label_user)
        lobby_layout.addWidget(self.start_game_button)
        lobby_layout.addWidget(self.disconnect_button)

        self.lobby_widget = QWidget()
        self.lobby_widget.setLayout(lobby_layout)


class WaitingScene(QWidget):
    def __init__(self):
        super().__init__()
        self.time_label = QLabel()
        self.storno_button = QPushButton('Storno', self)
        self.initUI()

    def initUI(self):
        label_waiting = QLabel("Waiting for the game")

        waiting_layout = QVBoxLayout()
        waiting_layout.addWidget(label_waiting)
        waiting_layout.addWidget(self.time_label)
        waiting_layout.addWidget(self.storno_button)

        self.waiting_widget = QWidget()
        self.waiting_widget.setLayout(waiting_layout)

class GameScene(QWidget):
    def __init__(self):
        super().__init__()
        self.initUI()

    def initUI(self):
        game_layout = QVBoxLayout()

        self.game_widget = QWidget()
        self.game_widget.setLayout(game_layout)

class ResultScene(QWidget):
    def __init__(self):
        super().__init__()
        self.initUI()

    def initUI(self):
        result_layout = QVBoxLayout()

        self.result_widget = QWidget()
        self.result_widget.setLayout(result_layout)
