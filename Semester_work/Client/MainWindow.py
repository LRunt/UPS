"""
Author: Lukas Runt
Email: lrunt@students.zcu.cz
Date: 14.11.2023
Version: 0.1.0
Description: Securing and managing data of the user, definition of window and its content
"""
from PyQt5.QtWidgets import QWidget, QVBoxLayout, QStackedWidget, QApplication
import Scenes
import Socket
from MessageBoxes import show_error_message
from Logger import logger


class User:
    """
    Class user saves data about user
    """

    def __init__(self):
        self.user_state = user_state["Disconnect"]
        self.user_name = ""


# Possible user states
user_state = {
    "Disconnect": -1,
    "Logged": 0,
    "Waiting": 1,
    "In_Game": 2,
    "Result_screen": 3
}

# List of scenes
scenes = {
    "Login": 0,
    "Lobby": 1,
    "Waiting": 2,
    "Game": 3,
    "Result": 4
}

results = {
    1: "WIN",
    2: "LOSE",
    3: "DRAW"
}


def convert_string_to_integer(string):
    """
    Function converts string to integer,
    If string is not an integer then returns -1
    :param string: String what will be converted to integer
    :return: integer or -1 (wrong input)
    """
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
        self.lobby_scene = Scenes.LobbyScene()
        self.waiting_scene = Scenes.WaitingScene()
        self.game_scene = Scenes.GameScene()
        self.result_screen = Scenes.ResultScene()
        self.initUI()
        self.user = User()

    def initUI(self):
        """
        Initialization of user interface
        """
        # Defining method for buttons
        self.login_scene.login_button.clicked.connect(self.login)
        self.lobby_scene.start_game_button.clicked.connect(self.start_searching_game)
        self.lobby_scene.disconnect_button.clicked.connect(self.disconnect)
        self.waiting_scene.storno_button.clicked.connect(self.cancel_searching)
        for i, button in enumerate(self.game_scene.fields):
            button.clicked.connect(lambda checked, index=i: self.on_button_clicked(index))

        # Create stacked widget to manage different scenes
        self.stacked_widget = QStackedWidget(self)
        self.stacked_widget.addWidget(self.login_scene.login_widget)
        self.stacked_widget.addWidget(self.lobby_scene.lobby_widget)
        self.stacked_widget.addWidget(self.waiting_scene.waiting_widget)
        self.stacked_widget.addWidget(self.game_scene.game_widget)
        self.stacked_widget.addWidget(self.result_screen.result_widget)

        # Set up the main layout
        main_layout = QVBoxLayout(self)
        main_layout.addWidget(self.stacked_widget)

        self.setGeometry(300, 300, 300, 200)
        self.setWindowTitle('TIC-TAC-TOE')

    def closeEvent(self, event):
        QApplication.instance().quit()
        if self.socket.connection:
            self.socket.disconnect()

    def login(self):
        """
        Method for logging the user after clicking login button,
        method check all information from user and then try to connect to the server
        """
        server_ip_address = self.login_scene.text_field_ip_address.text()
        server_port = convert_string_to_integer(self.login_scene.text_field_port.text())
        username = self.login_scene.text_field_username.text()
        if server_ip_address == "" or server_port == "" or username == "":
            logger.warning("Some fields are not filed!")
            show_error_message("Some fields are not filed!")
        elif server_port == -1:
            logger.warning("Port address is not a number")
        else:
            try:
                self.socket.load_data(server_ip_address, server_port)
                self.socket.connect()
                self.socket.send(f"LOGIN|{username}")
                self.user.user_name = username
            except Exception as e:
                print("Error: connection failed!", str(e))

    def start_searching_game(self):
        """
        Sends message to start looking for opponents
        """
        self.socket.send(f"START")
        self.stacked_widget.setCurrentIndex(scenes["Waiting"])
        self.user.user_state = user_state["Waiting"]

    def disconnect(self):
        """
        Sends message for disconnect the server
        """
        self.socket.disconnect()
        self.stacked_widget.setCurrentIndex(scenes["Login"])

    def cancel_searching(self):
        """
        Sends message for cancel searching opponent
        """
        self.socket.send(f"STORNO")

    def on_button_clicked(self, index):
        print(f"Button {index} clicked!")
        self.socket.send(f"TURN|{str(index)}")

    def handle_received_message(self, message):
        """
        Method handles message what have come form server
        :param message: message from server
        """
        split_message = message.split('|')
        if self.user.user_state == user_state["Disconnect"]:
            logger.info("User state: Disconnect")
            if split_message[0] == "LOGIN":
                self.login_result(split_message)
        if self.user.user_state == user_state["Logged"]:
            logger.info("User state: Logged")
        if self.user.user_state == user_state["Waiting"]:
            logger.info("User state: Waiting")
            if split_message[0] == "STORNO":
                self.stacked_widget.setCurrentIndex(scenes["Lobby"])
                self.user.user_state = user_state["Logged"]
            if split_message[0] == "GAME":
                self.stacked_widget.setCurrentIndex(scenes["Game"])
                self.user.user_state = user_state["In_Game"]
        if self.user.user_state == user_state["In_Game"]:
            if split_message[0] == "GAME":
                self.parse_game_message(split_message)
            if split_message[0] == "RESULT":
                self.stacked_widget.setCurrentIndex(scenes["Result"])
                self.parse_result_message(split_message)
            logger.info("User state: In game")
        if self.user.user_state == user_state["Result_screen"]:
            logger.info("User state: Result screen")

    def login_result(self, split_message):
        """
        Method parsing login message what have come and processing corresponding action
        :param split_message: split message what have come from server
        """
        if len(split_message) != 2:
            logger.error("Wrong number of parameters")
        else:
            if split_message[1] == "0":
                self.lobby_scene.label_user.setText(f"User: {self.user.user_name}")
                self.stacked_widget.setCurrentIndex(scenes["Lobby"])
                self.user.user_state = user_state["Logged"]
                logger.info("Login success")
            elif split_message[1] == "1":
                print("exit offline user")
                print("Loading")
            elif split_message[1] == "2":
                print("Exit online user")
            elif split_message[1] == '3':
                print("Illegal characters")
            elif split_message[1] == '4':
                print("Short username")
            elif split_message[1] == '5':
                print("Long username")
            else:
                print("Wrong code")

    def parse_game_message(self, params):
        """
        Method for parsing message of game state in format:
        GAME|<side_of_player>|<opponent_name>|<number_of_turn>|<game_status>|<play_field1>|...|<play_fieldN>
        :param params: received message with game status
        """
        if params[1] == "1":
            self.game_scene.player_1.setText(f"X - {self.user.user_name} (YOU)")
            self.game_scene.player_2.setText(f"O - {params[2]}")
        else:
            self.game_scene.player_1.setText(f"X - {params[2]}")
            self.game_scene.player_2.setText(f"O - {self.user.user_name} (YOU)")
        turn = convert_string_to_integer(params[3])
        if turn % 2 == 0:
            self.game_scene.turn.setText("Na tahu je hráč: O")
        else:
            self.game_scene.turn.setText("Na tahu je hráč: X")
        game_state = convert_string_to_integer(params[4])
        # Filling game play board
        for i in range(len(self.game_scene.fields)):
            index = i + 5
            play_field = convert_string_to_integer(params[index])
            if play_field == 0:
                self.game_scene.clean_field(i)
            elif play_field % 2 == 1:
                self.game_scene.draw_X(i)
            else:
                self.game_scene.draw_O(i)

    def parse_result_message(self, message):
        """
        Method for parsing message with result in format:
        RESULT|<game_result>|<index1>|...|indexN>|<winIndex1>|...|<winIndex3>
        :param message: message with result
        """
        self.result_screen.result.setText(results[convert_string_to_integer(message[1])])
        # Filling game play board
        for i in range(len(self.result_screen.fields)):
            index = i + 2
            play_field = convert_string_to_integer(message[index])
            if play_field == 0:
                self.result_screen.clean_field(i)
            elif play_field % 2 == 1:
                self.result_screen.draw_X(i)
            else:
                self.result_screen.draw_O(i)
