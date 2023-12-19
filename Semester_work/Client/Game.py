
class Game:
    def __init__(self, game_scene):
        self.game_scene = game_scene
        self.play_board = 9 * 0
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
        else:
            self.player1 == params[2]
            self.player2 == f"{username} (YOU)"
        self.turn = params[3]
        self.game_state = params[4]
        # Filling game play board
        for i in range(len(self.play_board)):
            self.play_board[i] = params[5 + i]