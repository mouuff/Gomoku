
#include <iostream>
#include <fstream>
#include <string>
#include "gomoku.h"
#include "Game.hpp"

int main()
{
  std::string tmp;
  IGame* game = new Game();
  //std::cout << std::to_string(game->play().x) << "," << std::to_string(game->play().y) << std::endl;
  Protocol* protocol = new Protocol(game);
  protocol->readLoop();
  return 0;
}
