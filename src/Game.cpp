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

VPoint Game::mapIterator(int deepth) {
  register Point pt = Point();
  register VPoint win = VPoint();
  for (pt.x = 0; pt.x < protocol->mapSize().x ; pt.x++) {
    for (pt.y = 0; pt.y < protocol->mapSize().y ; pt.y++) {
      if (deepth % 2 == 0)
        protocol->mapGet(pt) = Tile::OWN;
      else
        protocol->mapGet(pt) = Tile::OPPONENT;
      if (deepth != 0)
      {
        //evaluate
        mapIterator(deepth--);
        //evaluate - ret
      }
      else
        (void)pt;//evaluate();
      protocol->mapGet(pt) = Tile::EMPTY;
    }
  }
  return win;
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
