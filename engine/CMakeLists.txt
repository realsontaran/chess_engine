cmake_minimum_required(VERSION 3.16)

project (
  chess_engine
  VERSION 0.1.0
  LANGUAGES CXX)

add_executable(${PROJECT_NAME} src/main.cpp src/bitboard.cpp src/attack_table.cpp)
