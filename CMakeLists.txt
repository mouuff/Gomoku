
include_directories(include)

project (pbrain-pd35 C CXX)

add_executable (pbrain-pd35
  src/main.cpp
  src/misc.cpp
  src/Game.cpp
  src/GameEval.cpp
  src/Protocol.cpp
)

install (TARGETS pbrain-pd35 DESTINATION .)
