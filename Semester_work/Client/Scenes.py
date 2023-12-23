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
        self.error_label = QLabel("Error", self)
        self.login_button = QPushButton('Login', self)
        self.login_widget = QWidget(self)
        self.initUI()

    def initUI(self):
        self.error_label.setVisible(False)
        # Create labels and text fields for the first scene
        label_name_of_scene = QLabel("Server login")
        label_ip_address = QLabel('IP address:')
        label_port = QLabel('Port:')
        label_username = QLabel('Username:')

        # Set fonts and styles
        label_name_of_scene.setFont(QFont("Arial", 20, QFont.Bold))
        label_ip_address.setFont(QFont("Arial", 12, QFont.Bold))
        label_port.setFont(QFont("Arial", 12, QFont.Bold))
        label_username.setFont(QFont("Arial", 12, QFont.Bold))

        self.error_label.setStyleSheet("background-color: #f8d7da; color: #721c24; border: 1px solid #f5c6cb; "
                                       "padding: 8px; border-radius: 4px;")

        # Create layout for the first scene
        form_layout = QFormLayout()
        form_layout.addRow(label_ip_address, self.text_field_ip_address)
        form_layout.addRow(label_port, self.text_field_port)
        form_layout.addRow(label_username, self.text_field_username)

        form_widget = QWidget()
        form_widget.setLayout(form_layout)
        form_widget.setMaximumWidth(1000)

        login_layout = QVBoxLayout(self)
        login_layout.addSpacing(30)  # Add space at the top
        login_layout.addWidget(label_name_of_scene, alignment=Qt.AlignCenter)
        login_layout.addSpacing(20)  # Add space between the title and the form
        login_layout.addWidget(form_widget, alignment=Qt.AlignCenter)
        login_layout.addSpacing(20)  # Add space between the form and the error label
        login_layout.addWidget(self.error_label, alignment=Qt.AlignCenter)
        login_layout.addWidget(self.login_button, alignment=Qt.AlignCenter)
        login_layout.addSpacing(30)  # Add space at the bottom

        # Create widget for the first scene
        self.login_widget.setLayout(login_layout)


class LobbyScene(QWidget):
    def __init__(self):
        super().__init__()
        self.label_user = QLabel('', self)
        self.start_game_button = QPushButton('START', self)
        self.disconnect_button = QPushButton('DISCONNECT', self)
        self.initUI()

    def initUI(self):
        lobby_label = QLabel("LOBBY")
        lobby_label.setFont(QFont("Arial", 32, QFont.Bold))
        lobby_label.setAlignment(Qt.AlignCenter)
        lobby_label.setStyleSheet("QLabel {"
                                  "  color: #333333;"  # Set text color to a dark shade
                                  "}")

        self.start_game_button.setStyleSheet("QPushButton {"
                                             "  background-color: #4CAF50;"  # Green background color
                                             "  color: black;"  # Black text color
                                             "  border: 1px solid #4CAF50;"  # Green border
                                             "  border-radius: 5px;"  # Rounded corners
                                             "  padding: 8px 16px;"  # Increased padding around the text
                                             "  font-size: 16px;"  # Larger font size
                                             "  font-weight: bold;"  # Bold text
                                             "}"
                                             "QPushButton:hover {"
                                             "  background-color: #45a049;"  # Darker green on hover
                                             "}")

        self.disconnect_button.setStyleSheet("QPushButton {"
                                             "  background-color: #f44336;"  # Red background color
                                             "  color: black;"  # Black text color
                                             "  border: 1px solid #f44336;"  # Red border
                                             "  border-radius: 5px;"  # Rounded corners
                                             "  padding: 8px 16px;"  # Increased padding around the text
                                             "  font-size: 16px;"  # Larger font size
                                             "  font-weight: bold;"  # Bold text
                                             "}"
                                             "QPushButton:hover {"
                                             "  background-color: #d32f2f;"  # Darker red on hover
                                             "}")

        lobby_layout = QVBoxLayout()
        lobby_layout.addWidget(lobby_label)
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

        self.result = QLabel("WIN")

        self.result.setFont(QFont("Arial", 20, QFont.Bold))

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

        self.play_again_button = QPushButton("Rematch")
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
            self.text_label.setText("Opponent don't wanna play again.")
            self.play_again_button.setEnabled(False)
            self.exit_button.setEnabled(True)
        if mode == 2:
            self.text_label.setText("Opponent want a rematch.")
            self.play_again_button.setEnabled(True)
            self.exit_button.setEnabled(True)
        if mode == 4:
            self.text_label.setText("Waiting for opponent.")
            self.play_again_button.setEnabled(False)
            self.exit_button.setEnabled(False)
