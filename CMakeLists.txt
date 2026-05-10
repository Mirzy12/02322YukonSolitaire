cmake_minimum_required(VERSION 3.24)
project(Solitaire C)

set(CMAKE_C_STANDARD 11)

add_executable(Solitaire source/src/Game.c source/src/LD.c source/src/LD.h source/src/main.h)
