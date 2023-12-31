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

# Types of result
results = {
    1: "WIN",
    2: "LOSE",
    3: "DRAW",
    4: "CONNECTION ERROR"
}
