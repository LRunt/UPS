class Game:
    def __init__(self, game_scene):
        self.game_scene = game_scene
        self.play_board = 9 * [0]
        self.side = None
        self.player1 = None
        self.player2 = None
        self.turn = None
        self.game_state = None

    def parse_game_status_message(self, params, username):
        # Setting side
        self.side = params[1]
        # Players
        if self.side == 1:
            self.player1 == f"{username} (YOU)"
            self.player2 == params[2]
            self.game_scene.player_1.setText(f"X - {username} (YOU)")
            self.game_scene.player_2.setText(f"O - {params[2]}")
        else:
            self.player1 == params[2]
            self.player2 == f"{username} (YOU)"
            self.game_scene.player_1.setText(f"X - {params[2]}")
            self.game_scene.player_2.setText(f"O - {username} (YOU)")
        self.turn = params[3]
        if self.turn % 2 == 0:
            self.game_scene.turn.setText("Na tahu je hráč: O")
        else:
            self.game_scene.turn.setText("Na tahu je hráč: X")
        self.game_state = params[4]
        # Filling game play board
        for i in range(len(self.play_board)):
            index = i + 5
            if params[index] == 0:
                self.game_scene.clean_field(index)
            elif params[index] % 2 == 1:
                self.game_scene.draw_X(index)
            else:
                self.game_scene.draw_O(index)
            self.play_board[i] = params[index]
