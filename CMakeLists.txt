project (gomoku-cmake C CXX)

cmake_minimum_required(VERSION 3.9)

add_executable (gomoku src/main.cpp)

install (TARGETS gomoku DESTINATION bin)

