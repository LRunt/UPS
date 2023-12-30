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
from Logger import logger
from MessageBoxes import show_error_message


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
    "Result_screen": 3,
    "Loading": 4
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
    3: "DRAW",
    4: "CONNECTION ERROR"
}

HEARTBEAT_TIME = 0.25


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
        self.result_scene = Scenes.ResultScene()
        self.stacked_widget = QStackedWidget(self)
        self.required_disconnection = False
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
        self.waiting_scene.cancel_button.clicked.connect(self.cancel_searching)
        for i, button in enumerate(self.game_scene.fields):
            button.clicked.connect(lambda checked, index=i: self.on_button_clicked(index))
        self.result_scene.play_again_button.clicked.connect(self.play_again)
        self.result_scene.exit_button.clicked.connect(self.leave_to_lobby)

        # Create stacked widget to manage different scenes
        self.stacked_widget.addWidget(self.login_scene.login_widget)
        self.stacked_widget.addWidget(self.lobby_scene.lobby_widget)
        self.stacked_widget.addWidget(self.waiting_scene.waiting_widget)
        self.stacked_widget.addWidget(self.game_scene.game_widget)
        self.stacked_widget.addWidget(self.result_scene.result_widget)

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
            self.show_login_error("Error: Some fields are not filled in!")
            logger.warning("Some fields are not filed in!")
        elif server_port == -1:
            self.show_login_error("Error: Port address is not a number.")
            logger.warning("Port address is not a number")
        else:
            try:
                self.socket.load_data(server_ip_address, server_port)
                self.socket.connect()
                self.socket.send(f"LOGIN|{username}")
                self.user.user_name = username
            except Exception as e:
                self.show_login_error("Error: Connection failed.")
                logger.error(f"Connection failed: {str(e)}")

    def start_searching_game(self):
        """
        Sends message to start looking for opponents
        """
        self.socket.send(f"START")
        self.change_state("Waiting", "Waiting")

    def disconnect(self):
        """
        Sends message for disconnect the server
        """
        self.required_disconnection = True
        self.socket.disconnect()
        self.change_state("Login", "Disconnect")

    def cancel_searching(self):
        """
        Sends message for cancel searching opponent
        """
        self.socket.send(f"STORNO")

    def on_button_clicked(self, index):
        logger.info(f"User clicked button with index {index}")
        self.socket.send(f"TURN|{str(index)}")

    def play_again(self):
        logger.info(f"User {self.user.user_name} want rematch")
        self.socket.send(f"REMATCH|1")

    def leave_to_lobby(self):
        logger.info(f"User {self.user.user_name} do not want rematch")
        self.socket.send(f"REMATCH|0")

    def handle_received_message(self, message):
        """
        Method handles message what have come form server
        :param message: message from server
        """
        split_message = message.split('|')
        if split_message[0] == "CONNECTION_LOST":
            self.connection_lost()
        if self.user.user_state == user_state["Disconnect"]:
            logger.info("User state: Disconnect")
            if split_message[0] == "LOGIN":
                self.login_result(split_message)
        if self.user.user_state == user_state["Logged"]:
            logger.info("User state: Logged")
        if self.user.user_state == user_state["Waiting"]:
            logger.info("User state: Waiting")
            if split_message[0] == "WAITING":
                self.ticks_to_time_string(convert_string_to_integer(split_message[1]))
            if split_message[0] == "STORNO":
                self.change_state("Lobby", "Logged")
            if split_message[0] == "GAME":
                self.change_state("Game", "In_Game")
        if self.user.user_state == user_state["In_Game"]:
            if split_message[0] == "GAME":
                self.parse_game_message(split_message)
            if split_message[0] == "RESULT":
                self.change_state("Result", "Result_screen")
                self.parse_result_message(split_message)
            logger.info("User state: In game")
        if self.user.user_state == user_state["Result_screen"]:
            if split_message[0] == "RESULT":
                self.parse_result_message(split_message)
            if split_message[0] == "LOGGED":
                self.result_scene.reset_fields()
                self.change_state("Lobby", "Logged")
            if split_message[0] == "GAME":
                self.result_scene.reset_fields()
                self.change_state("Game", "In_Game")
                self.parse_game_message(split_message)
            logger.info("User state: Result screen")
        if self.user.user_state == user_state["Loading"]:
            if split_message[0] == "LOGGED":
                self.change_state("Lobby", "Logged")
            if split_message[0] == "WAITING":
                self.change_state("Waiting", "Waiting")
            if split_message[0] == "GAME":
                self.change_state("Game", "In_Game")
            if split_message[0] == "RESULT":
                self.change_state("Result", "Result_screen")

    def change_state(self, screen, state):
        """
        Method sets state and screen where will player play
        :param state: new state of the user
        :param screen: screen what will be shoved to the user
        """
        self.stacked_widget.setCurrentIndex(scenes[screen])
        self.user.user_state = user_state[state]

    def connection_lost(self):
        """
        Method handle connection lost
        """
        if not self.required_disconnection:
            self.change_state("Login", "Disconnect")
            show_error_message("Connection lost.")


    def login_result(self, split_message):
        """
        Method parsing login message what have come and processing corresponding action
        :param split_message: split message what have come from server
        """
        if len(split_message) != 2:
            logger.error("Wrong number of parameters")
        else:
            if split_message[1] == "0":
                self.login_scene.error_label.setVisible(False)
                self.lobby_scene.label_user.setText(f"User: {self.user.user_name}")
                self.stacked_widget.setCurrentIndex(scenes["Lobby"])
                self.user.user_state = user_state["Logged"]
                logger.info("Login success")
            elif split_message[1] == "1":
                self.login_scene.error_label.setVisible(False)
                self.user.user_state = user_state["Loading"]
                self.lobby_scene.label_user.setText(f"User: {self.user.user_name}")
                logger.info("Loading user data")
                print("Loading")
            elif split_message[1] == "2":
                self.show_login_error("Error: There is online user with same name.")
                logger.warning("Exist online user with same username")
            elif split_message[1] == '3':
                self.show_login_error("Error: There are illegal characters in your name.")
                logger.warning("Illegal characters")
            elif split_message[1] == '4':
                self.show_login_error("Error: Username is too short.")
                logger.warning("Short username")
            elif split_message[1] == '5':
                self.show_login_error("Error: Username is too long.")
                logger.warning("Long username")
            else:
                logger.info("Wrong code")

    def show_login_error(self, error_message):
        """
        Method to showing error to the user
        :param error_message: error message what will be shown to the user
        """
        self.login_scene.error_label.setVisible(True)
        self.login_scene.error_label.setText(error_message)

    def ticks_to_time_string(self, waiting_time):
        """
        Method compute and sets time in the queue
        :param waiting_time: number of ping messages, what have been sent
        """
        # Calculate minutes and seconds
        minutes = waiting_time // 4 // 60
        seconds = (waiting_time // 4) % 60

        # Format the time string
        time_string = f"{minutes}:{seconds:02d}"  # Use :02d to ensure two digits for seconds

        self.waiting_scene.time_label.setText(f"Time in queue: {time_string}")

    def parse_game_message(self, params):
        """
        Method for parsing message of game state in format:
        GAME|<side_of_player>|<opponent_name>|<number_of_turn>|<play_field1>|...|<play_fieldN>
        :param params: received message with game status
        """
        opponent = params[2]
        x = ""
        o = ""
        if params[1] == "1":
            self.game_scene.player_1.setText(f"X - {self.user.user_name} (YOU)")
            self.game_scene.player_2.setText(f"O - {opponent}")
            x = self.user.user_name
            o = opponent
        else:
            self.game_scene.player_1.setText(f"X - {opponent}")
            self.game_scene.player_2.setText(f"O - {self.user.user_name} (YOU)")
            x = opponent
            o = self.user.user_name
        user_connected = convert_string_to_integer(params[3])
        turn = convert_string_to_integer(params[4])
        if user_connected > 0:
            logger.info(f"Opponent is not connected")
            self.game_scene.turn.setText(f"Waiting for opponent: {30 - user_connected}")
            self.game_scene.set_turn_color("Error")
        elif turn % 2 == 0:
            self.game_scene.turn.setText(f"O is on the turn {o}")
            self.game_scene.set_turn_color("O")
        else:
            self.game_scene.turn.setText(f"X is on the turn {x}")
            self.game_scene.set_turn_color("X")
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
        RESULT|<game_result>|<response_state>|<index1>|...|indexN>|<winIndex1>|...|<winIndex3>
        :param message: message with result
        """
        game_result = convert_string_to_integer(message[1])
        self.result_scene.result.setText(results[game_result])

        self.result_scene.change_widget_scene(convert_string_to_integer(message[2]), convert_string_to_integer(message[3]))
        if game_result == 4:
            self.result_scene.play_again_button.setEnabled(False)
        # Filling game play board
        for i in range(len(self.result_scene.fields)):
            index = i + 4
            play_field = convert_string_to_integer(message[index])
            if play_field == 0:
                self.result_scene.clean_field(i)
            elif play_field % 2 == 1:
                self.result_scene.draw_X(i)
            else:
                self.result_scene.draw_O(i)
        if message[1] == "1" or message[1] == "2":
            for i in range(3):
                index = i + 13
                self.result_scene.color_winner_field(convert_string_to_integer(message[index]))
