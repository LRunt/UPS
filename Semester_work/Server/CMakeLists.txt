cmake_minimum_required(VERSION 3.16)

project(UPS_Server)

# Setting source files
set(SOURCE_FILES
    Logger.cpp
    User.cpp
    Game.cpp
    Server.cpp
)

# Setting header files
set(HEADER_FILES
    Logger.h
    User.h
    Game.h
)

# Create the executable
add_executable(Server ${SOURCE_FILES} ${HEADER_FILES})