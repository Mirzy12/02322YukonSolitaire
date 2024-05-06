cmake_minimum_required(VERSION 3.24)
project(Solitaire C)

set(CMAKE_C_STANDARD 11)

add_executable(Solitaire src/Display.c src/LoadGame.c src/Display.h src/gamePlay.c src/gamePlay.h src/LoadGame.h src/main.c src/main.h src/Setup.c src/Setup.h)
