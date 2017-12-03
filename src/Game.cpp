#include <iostream>
#include "Protocol.hpp"
#include "Game.hpp"
#include "misc.h"

Game::Game()
{
  this->protocol = nullptr;
}

Game::~Game() {}

void Game::start(Protocol* protocol)
{
  this->protocol = protocol;
}

void Game::end()
{

}

Point Game::play()
{
  Point pt = Point();
  if (protocol == nullptr) {
    std::cout << "MESSAGE protocol not set in game" << std::endl;
    return pt;
  }
  protocol->log("start find");
  do {
    pt.x = my_randint(0, protocol->mapSize().x - 1);
    pt.y = my_randint(0, protocol->mapSize().y - 1);
    protocol->log("top");
  } while (protocol->mapGet(pt) != Tile::EMPTY);
  return pt;
}
