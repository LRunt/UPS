"""
Author: Lukas Runt
Email: lrunt@students.zcu.cz
Date: 14.11.2023
Version: 0.1.0
Description: Definition of all scenes
"""
from PyQt5.QtCore import Qt
from PyQt5.QtGui import QFont
from PyQt5.QtWidgets import QWidget, QLabel, QLineEdit, QVBoxLayout, QPushButton, QGridLayout, QHBoxLayout, QFormLayout

NUMBER_OF_FIELDS = 9


class LoginScene(QWidget):
    def __init__(self):
        super().__init__()
        self.text_field_ip_address = QLineEdit(self)
        self.text_field_port = QLineEdit(self)
        self.text_field_username = QLineEdit(self)
        self.login_button = QPushButton('Login', self)
        self.initUI()

    def initUI(self):
        # Create labels and text fields for the first scene
        label_ip_address = QLabel('IP address:')
        label_port = QLabel('Port:')
        label_username = QLabel('Username:')

        # Create layout for the first scene
        form_layout = QFormLayout()
        form_layout.addRow(label_ip_address, self.text_field_ip_address)
        form_layout.addRow(label_port, self.text_field_port)
        form_layout.addRow(label_username, self.text_field_username)

        login_layout = QVBoxLayout()
        login_layout.addLayout(form_layout)
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
        self.game_widget = None
        self.fields = [0] * NUMBER_OF_FIELDS
        self.player_1 = None  # Player X
        self.player_2 = None  # Player O
        self.turn = None
        self.initUI()

    def initUI(self):
        play_board_layout = QGridLayout()
        play_board_layout.setSpacing(0)

        # initializing fields
        for i in range(NUMBER_OF_FIELDS):
            button = QPushButton("")
            button.setFixedSize(100, 100)
            button.setStyleSheet("background-color: white; border: 1px solid black;")
            font = QFont("Arial Rounded MT Bold", 72)
            button.setFont(font)

            self.fields[i] = button

            play_board_layout.addWidget(self.fields[i], i % 3, i / 3)

        self.player_1 = QLabel("X - ")
        self.player_2 = QLabel("O - ")
        self.turn = QLabel("Na tahu: X")
        players = QVBoxLayout()
        players.addWidget(self.player_1)
        players.addWidget(self.player_2)
        players.addWidget(self.turn)

        play_board_widget = QWidget()
        play_board_widget.setLayout(play_board_layout)

        game_scene = QHBoxLayout()
        game_scene.addWidget(play_board_widget, alignment=Qt.AlignCenter)
        game_scene.addLayout(players)

        self.game_widget = QWidget()
        self.game_widget.setLayout(game_scene)

    def clean_field(self, index):
        self.fields[index].setText("")

    def draw_X(self, index):
        self.fields[index].setText("×")
        self.fields[index].setStyleSheet(
            "color: red; font-size: 108px; background-color: white; border: 1px solid black;")

    def draw_O(self, index):
        self.fields[index].setText("O")
        self.fields[index].setStyleSheet(
            "color: blue; font-size: 72px; background-color: white; border: 1px solid black;font-weight: bold;")


class ResultScene(QWidget):
    def __init__(self):
        super().__init__()
        self.result = None
        self.fields = [0] * NUMBER_OF_FIELDS
        self.play_again_button = None
        self.exit_button = None
        self.text_label = None
        self.initUI()

    def initUI(self):
        result_layout = QVBoxLayout()

        self.result = QLabel("Výhra")

        font = self.result.font()
        font.setPointSize(20)
        font.setBold(True)
        self.result.setFont(font)

        # Play board
        play_board_layout = QGridLayout()
        play_board_layout.setSpacing(0)

        # initializing fields
        for i in range(NUMBER_OF_FIELDS):
            button = QPushButton("")
            button.setFixedSize(100, 100)
            button.setStyleSheet("background-color: white; border: 1px solid black;")
            font = QFont("Arial Rounded MT Bold", 72)
            button.setFont(font)

            self.fields[i] = button

            play_board_layout.addWidget(self.fields[i], i % 3, i / 3)

        play_board = QHBoxLayout()
        lab1 = QLabel("")
        lab2 = QLabel("")

        play_board.addWidget(lab1)
        play_board.addLayout(play_board_layout)
        play_board.addWidget(lab2)

        play = QVBoxLayout()
        self.text_label = QLabel("")
        play.addLayout(play_board)
        play.addWidget(self.text_label, alignment=Qt.AlignCenter)

        # Buttons under play board - State no-one respond
        buttons = QHBoxLayout()

        self.play_again_button = QPushButton("Odveta")
        self.exit_button = QPushButton("Lobby")

        buttons.addWidget(self.play_again_button)
        buttons.addWidget(self.exit_button)

        result_layout.addWidget(self.result, alignment=Qt.AlignCenter)
        result_layout.addLayout(play)
        result_layout.addLayout(buttons)

        self.result_widget = QWidget()
        self.result_widget.setLayout(result_layout)

    def reset_fields(self):
        for i in range(len(self.fields)):
            self.fields[i].setStyleSheet("background-color: white; border: 1px solid black;")

    def clean_field(self, index):
        self.fields[index].setText("")

    def draw_X(self, index):
        self.fields[index].setText("×")
        self.fields[index].setStyleSheet(
            "color: red; font-size: 108px; background-color: white; border: 1px solid black;")

    def draw_O(self, index):
        self.fields[index].setText("O")
        self.fields[index].setStyleSheet(
            "color: blue; font-size: 72px; background-color: white; border: 1px solid black;font-weight: bold;")

    def color_winner_field(self, index):
        self.fields[index].setStyleSheet(self.fields[index].styleSheet() + "background-color: lightyellow;")

    def change_widget_scene(self, mode):
        if mode == 0:
            self.text_label.setText("")
            self.play_again_button.setEnabled(True)
            self.exit_button.setEnabled(True)
        if mode == 1:
            self.text_label.setText("Protihráč nechce hrát znovu.")
            self.play_again_button.setEnabled(False)
            self.exit_button.setEnabled(True)
        if mode == 2:
            self.text_label.setText("Protihráč chce hrát znovu.")
            self.play_again_button.setEnabled(True)
            self.exit_button.setEnabled(True)
        if mode == 4:
            self.text_label.setText("Čekám na odpověď protihráče.")
            self.play_again_button.setEnabled(False)
            self.exit_button.setEnabled(False)
