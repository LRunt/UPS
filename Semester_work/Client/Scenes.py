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
RED = "#cf2121"
BLUE = "#6d59ff"
GRAY = "#946f6f"


class LoginScene(QWidget):
    def __init__(self):
        super().__init__()
        self.text_field_ip_address = QLineEdit(self)
        self.text_field_port = QLineEdit(self)
        self.text_field_username = QLineEdit(self)
        self.error_label = QLabel("Error", self)
        self.login_button = QPushButton('LOGIN', self)
        self.login_widget = QWidget(self)
        self.initUI()

    def initUI(self):
        self.error_label.setVisible(False)
        # Create labels and text fields for the first scene
        label_name_of_scene = QLabel("SERVER LOGIN")
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

        self.login_button.setStyleSheet("QPushButton {"
                                        "  background-color: #007BFF;"  # Blue background color
                                        "  color: black;"  # White text color
                                        "  border: 1px solid #007BFF;"  # Blue border
                                        "  border-radius: 5px;"  # Rounded corners
                                        "  padding: 10px 20px;"  # Increased padding around the text
                                        "  font-size: 24px;"  # Larger font size
                                        "  font-weight: bold;"  # Bold text
                                        "}"
                                        "QPushButton:hover {"
                                        "  background-color: #0056b3;"  # Darker blue on hover
                                        "}"
                                        "QPushButton:pressed {"
                                        "  background-color: #004080;"  # Even darker blue when pressed
                                        "}")

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
        login_layout.addWidget(self.login_button)
        login_layout.addSpacing(30)  # Add space at the bottom

        # Create widget for the first scene
        self.login_widget.setLayout(login_layout)


class LobbyScene(QWidget):
    def __init__(self):
        super().__init__()
        self.label_user = QLabel('User: Pepa', self)
        self.start_game_button = QPushButton('START', self)
        self.disconnect_button = QPushButton('DISCONNECT', self)
        self.lobby_widget = QWidget(self)
        self.initUI()

    def initUI(self):
        lobby_label = QLabel("LOBBY")
        lobby_label.setFont(QFont("Arial", 32, QFont.Bold))
        lobby_label.setAlignment(Qt.AlignCenter)
        lobby_label.setStyleSheet("QLabel {"
                                  "  color: #333333;"  # Set text color to a dark shade
                                  "}")

        self.label_user.setFont(QFont("Arial", 20))

        self.start_game_button.setStyleSheet("QPushButton {"
                                             "  background-color: #4CAF50;"  # Green background color
                                             "  color: black;"  # Black text color
                                             "  border: 1px solid #4CAF50;"  # Green border
                                             "  border-radius: 5px;"  # Rounded corners
                                             "  padding: 10px 20px;"  # Increased padding around the text
                                             "  font-size: 24px;"  # Larger font size
                                             "  font-weight: bold;"  # Bold text
                                             "}"
                                             "QPushButton:hover {"
                                             "  background-color: #45a049;"  # Darker green on hover
                                             "}"
                                             "QPushButton:pressed {"
                                             "  background-color: #3e8e41;"  # Even darker green when pressed
                                             "}")

        self.disconnect_button.setStyleSheet("QPushButton {"
                                             "  background-color: #f44336;"  # Red background color
                                             "  color: black;"  # Black text color
                                             "  border: 1px solid #f44336;"  # Red border
                                             "  border-radius: 5px;"  # Rounded corners
                                             "  padding: 10px 20px;"  # Increased padding around the text
                                             "  font-size: 24px;"  # Larger font size
                                             "  font-weight: bold;"  # Bold text
                                             "}"
                                             "QPushButton:hover {"
                                             "  background-color: #d32f2f;"  # Darker red on hover
                                             "}"
                                             "QPushButton:pressed {"
                                             "  background-color: #ba2727;"  # Even darker red when pressed
                                             "}")

        lobby_layout = QVBoxLayout()
        lobby_layout.addWidget(lobby_label)
        lobby_layout.addWidget(self.label_user, alignment=Qt.AlignCenter)
        lobby_layout.addWidget(self.start_game_button)
        lobby_layout.addWidget(self.disconnect_button)

        self.lobby_widget.setLayout(lobby_layout)


class WaitingScene(QWidget):
    def __init__(self):
        super().__init__()
        self.time_label = QLabel('Time in queue:', self)
        self.cancel_button = QPushButton('CANCEL', self)
        self.waiting_widget = QWidget(self)
        self.initUI()

    def initUI(self):
        label_waiting = QLabel("WAITING FOR THE GAME")
        label_waiting.setFont(QFont("Arial", 18, QFont.Bold))

        self.time_label.setFont(QFont("Arial", 20))

        self.cancel_button.setStyleSheet("QPushButton {"
                                         "  background-color: #f44336;"  # Red background color
                                         "  color: black;"  # Black text color
                                         "  border: 1px solid #f44336;"  # Red border
                                         "  border-radius: 5px;"  # Rounded corners
                                         "  padding: 10px 20px;"  # Increased padding around the text
                                         "  font-size: 24px;"  # Larger font size
                                         "  font-weight: bold;"  # Bold text
                                         "}"
                                         "QPushButton:hover {"
                                         "  background-color: #d32f2f;"  # Darker red on hover
                                         "}"
                                         "QPushButton:pressed {"
                                         "  background-color: #ba2727;"  # Even darker red when pressed
                                         "}")

        waiting_layout = QVBoxLayout()
        waiting_layout.addWidget(label_waiting, alignment=Qt.AlignCenter)
        waiting_layout.addWidget(self.time_label, alignment=Qt.AlignCenter)
        waiting_layout.addWidget(self.cancel_button)

        self.waiting_widget.setLayout(waiting_layout)


class GameScene(QWidget):
    def __init__(self):
        super().__init__()
        self.game_widget = QWidget(self)
        self.fields = [0] * NUMBER_OF_FIELDS
        self.player_1 = QLabel("X - Hráč1", self)  # Player X
        self.player_2 = QLabel("O - Hráč2", self)  # Player O
        self.turn = QLabel("X is on the turn (Hráč1)", self)
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

        self.player_1.setStyleSheet("QLabel {"
                                    "  color: white;"  # White text color for X
                                    f"  background-color: {RED};"  # Red background color
                                    "  padding: 5px;"  # Padding around the text
                                    "  font-size: 20px;"  # Font size
                                    "  font-family: Arial;"  # Font family
                                    "  font-weight: bold;"  # Bold text
                                    "}"
                                    "QLabel::border {"
                                    f"  border: 1px solid {RED};"  # Border color around the text
                                    "  border-radius: 5px;"  # Rounded corners
                                    "  padding: 2px;"  # Adjust the padding around the border
                                    "}")
        self.player_2.setStyleSheet("QLabel {"
                                    "  color: white;"  # White text color for O
                                    f"  background-color: {BLUE};"  # Blue background color
                                    "  padding: 5px;"  # Padding around the text
                                    "  font-size: 20px;"  # Font size
                                    "  font-family: Arial;"  # Font family
                                    "  font-weight: bold;"  # Bold text
                                    "}"
                                    "QLabel::border {"
                                    f"  border: 1px solid {BLUE};"  # Border color around the text
                                    "  border-radius: 5px;"  # Rounded corners
                                    "  padding: 2px;"  # Adjust the padding around the border
                                    "}")
        players = QVBoxLayout()

        self.player_1.setMaximumHeight(100)
        self.player_2.setMaximumHeight(100)

        players.addWidget(QLabel("    "))
        players.addWidget(self.player_1, alignment=Qt.AlignCenter)
        players.addWidget(self.player_2, alignment=Qt.AlignCenter)
        players.addWidget(QLabel("    "))
        players.addWidget(QLabel("    "))

        play_board_widget = QWidget()
        play_board_widget.setLayout(play_board_layout)

        game_and_turn = QVBoxLayout()
        self.set_turn_color("X")

        self.turn.setAlignment(Qt.AlignCenter)

        self.turn.setMaximumHeight(100)

        game_and_turn.addWidget(play_board_widget, alignment=Qt.AlignCenter)
        game_and_turn.addWidget(self.turn)

        game_scene = QHBoxLayout()
        game_scene.addLayout(game_and_turn)
        game_scene.addLayout(players)

        self.game_widget.setLayout(game_scene)

    def set_turn_color(self, turn):
        if turn == "Error":
            self.turn.setStyleSheet("QLabel {"
                                    "  color: white;"  # White text color for the turn label
                                    f"  background-color: {GRAY};"  # Light red background color (Bootstrap error)
                                    "  padding: 5px;"  # Padding around the text
                                    "  font-size: 18px;"  # Font size
                                    "  font-family: Arial;"  # Font family
                                    "  font-weight: bold;"  # Bold text
                                    "}"
                                    "QLabel::border {"
                                    f"  border: 1px solid {GRAY};"  # Dark red border color (Bootstrap error)
                                    "  border-radius: 5px;"  # Rounded corners
                                    "  padding: 2px;"  # Adjust the padding around the border
                                    "}")
        elif turn == "X":
            self.turn.setStyleSheet("QLabel {"
                                    "  color: white;"  # Black text color for the turn label
                                    f"  background-color: {RED};"  # Red background color
                                    "  padding: 5px;"  # Padding around the text
                                    "  font-size: 18px;"  # Font size
                                    "  font-family: Arial;"  # Font family
                                    "  font-weight: bold;"  # Bold text
                                    "}"
                                    "QLabel::border {"
                                    f"  border: 1px solid {RED};"  # Red border color around the text
                                    "  border-radius: 5px;"  # Rounded corners
                                    "  padding: 2px;"  # Adjust the padding around the border
                                    "}")
        else:
            self.turn.setStyleSheet("QLabel {"
                                    "  color: white;"  # Black text color for the turn label
                                    f"  background-color: {BLUE};"  # Blue background color
                                    "  padding: 5px;"  # Padding around the text
                                    "  font-size: 18px;"  # Font size
                                    "  font-family: Arial;"  # Font family
                                    "  font-weight: bold;"  # Bold text
                                    "}"
                                    "QLabel::border {"
                                    f"  border: 1px solid {BLUE};"  # Blue border color around the text
                                    "  border-radius: 5px;"  # Rounded corners
                                    "  padding: 2px;"  # Adjust the padding around the border
                                    "}")

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
        self.result = QLabel("", self)
        self.fields = [0] * NUMBER_OF_FIELDS
        self.play_again_button = QPushButton("REMATCH", self)
        self.exit_button = QPushButton("LOBBY", self)
        self.text_label = QLabel("", self)
        self.result_widget = QWidget(self)
        self.initUI()

    def initUI(self):
        self.play_again_button.setStyleSheet("QPushButton {"
                                             "  background-color: #28a745;"  # Green background color
                                             "  color: white;"  # White text color
                                             "  border: 1px solid #28a745;"  # Green border
                                             "  border-radius: 5px;"  # Rounded corners
                                             "  padding: 8px 16px;"  # Increased padding around the text
                                             "  font-size: 16px;"  # Larger font size
                                             "  font-weight: bold;"  # Bold text
                                             "}"
                                             "QPushButton:hover {"
                                             "  background-color: #218838;"  # Darker green on hover
                                             "}"
                                             "QPushButton:pressed {"
                                             "  background-color: #1e7e34;"  # Even darker green when pressed
                                             "}"
                                             "QPushButton:disabled {"
                                             "  background-color: #6c757d;"  # Gray background for disabled state
                                             "  color: #ced4da;"  # Light text color for disabled state
                                             "  border: 1px solid #6c757d;"  # Gray border for disabled state
                                             "}"
                                             )

        self.exit_button.setStyleSheet("QPushButton {"
                                       "  background-color: #dc3545;"  # Red background color
                                       "  color: white;"  # White text color
                                       "  border: 1px solid #dc3545;"  # Red border
                                       "  border-radius: 5px;"  # Rounded corners
                                       "  padding: 8px 16px;"  # Increased padding around the text
                                       "  font-size: 16px;"  # Larger font size
                                       "  font-weight: bold;"  # Bold text
                                       "}"
                                       "QPushButton:hover {"
                                       "  background-color: #c82333;"  # Darker red on hover
                                       "}"
                                       "QPushButton:pressed {"
                                       "  background-color: #bd2130;"  # Even darker red when pressed
                                       "}"
                                       "QPushButton:disabled {"
                                       "  background-color: #6c757d;"  # Gray background for disabled state
                                       "  color: #ced4da;"  # Light text color for disabled state
                                       "  border: 1px solid #6c757d;"  # Gray border for disabled state
                                       "}"
                                       )

        result_layout = QVBoxLayout()

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
        play.addLayout(play_board)
        play.addWidget(self.text_label, alignment=Qt.AlignCenter)

        # Buttons under play board - State no-one respond
        buttons = QHBoxLayout()

        buttons.addWidget(self.play_again_button)
        buttons.addWidget(self.exit_button)

        result_layout.addWidget(self.result, alignment=Qt.AlignCenter)
        result_layout.addLayout(play)
        result_layout.addLayout(buttons)

        self.result_widget.setLayout(result_layout)

    def reset_fields(self):
        for i in range(len(self.fields)):
            self.fields[i].setStyleSheet("background-color: white; border: 1px solid black;")

    def clean_field(self, index):
        self.fields[index].setText("")

    def draw_X(self, index):
        self.fields[index].setText("×")
        self.fields[index].setStyleSheet(
            f"color: {RED}; font-size: 108px; background-color: white; border: 1px solid black;")

    def draw_O(self, index):
        self.fields[index].setText("O")
        self.fields[index].setStyleSheet(
            f"color: {BLUE}; font-size: 72px; background-color: white; border: 1px solid black;font-weight: bold;")

    def color_winner_field(self, index):
        self.fields[index].setStyleSheet(self.fields[index].styleSheet() + "background-color: lightyellow;")

    def change_widget_scene(self, mode, timeout):
        if mode == 0:
            self.text_label.setText("")
            self.play_again_button.setEnabled(True)
            self.exit_button.setEnabled(True)
        if mode == 1:
            self.text_label.setText("Opponent don't wanna play again.")
            self.play_again_button.setEnabled(False)
            self.exit_button.setEnabled(True)
        if mode == 2:
            self.text_label.setText(f"Opponent want a rematch: {timeout}")
            self.play_again_button.setEnabled(True)
            self.exit_button.setEnabled(True)
        if mode == 4:
            self.text_label.setText(f"Waiting for opponent: {timeout}")
            self.play_again_button.setEnabled(False)
            self.exit_button.setEnabled(False)
