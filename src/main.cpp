
#include <iostream>
#include <fstream>
#include <string>
#include "gomoku.h"
#include "Game.hpp"

int main()
{
  std::string tmp;
  Game* game = new Game();
  Protocol* protocol = new Protocol((IGame*)game);
  protocol->readLoop();
  return 0;
}
