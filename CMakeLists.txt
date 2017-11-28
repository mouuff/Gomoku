project (gomoku-cmake C CXX)

cmake_minimum_required(VERSION 3.9)

include_directories(include)

add_executable (pbrain-pd35
  src/main.cpp
  src/Protocol.cpp
)

install (TARGETS gomoku DESTINATION bin)
