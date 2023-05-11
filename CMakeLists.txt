cmake_minimum_required(VERSION 3.24)
project(Solitaire C)

set(CMAKE_C_STANDARD 11)

add_executable(Solitaire src/Deck.c src/LD.c src/LD.h src/main.h)
