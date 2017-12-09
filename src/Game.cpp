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
  register VPoint tmp = VPoint();
  Tile player;
  TEval type;
  int val;

  win.v = 0;
  tmp.v = 0;
  if (deepth % 2 != 0) {
    player = Tile::OWN;
    type = ATTACK;
  } else {
    player = Tile::OPPONENT;
    type = DEFENSE;
  }
  for (pt.x = 0; pt.x < protocol->mapSize().x ; pt.x += 1) {
    for (pt.y = 0; pt.y < protocol->mapSize().y ; pt.y += 1) {
      if (protocol->mapGet(pt) == Tile::EMPTY) {
        protocol->mapGet(pt) = player;
        val = evaluate(pt, ATTACK) + (evaluate(pt, DEFENSE) * 10);
        if (deepth != 0) {
          tmp = mapIterator((deepth - 1));
        }
        if (win.v < val - (tmp.v / 2)) {
          win.v = val;
          win.pt = pt;
        }
        protocol->mapGet(pt) = Tile::EMPTY;
      }
    }
  }
  // protocol->log("Debug " + std::to_string(win.pt.x) + ", " + std::to_string(win.pt.y) + ", " + std::to_string(win.v) + ", deepth =" + std::to_string(deepth));
  // protocol->log("Debug " + std::to_string(pt.x) + ", " + std::to_string(pt.y));
  return win;
}

Point Game::randomEmptyPoint()
{
  Point pt;
  do {
    pt.x = my_randint(0, protocol->mapSize().x - 1);
    pt.y = my_randint(0, protocol->mapSize().y - 1);
  } while (protocol->mapGet(pt) != Tile::EMPTY);
  return pt;
}

Point Game::play()
{
  VPoint win = VPoint();
  Point pt = Point();
  if (protocol == nullptr) {
    std::cout << "MESSAGE protocol not set in game" << std::endl;
    return pt;
  }
  Point size = protocol->mapSize();
  Point curr;
  char max = 0;
  for (curr.y = 0; curr.y < size.y; curr.y += 1) {
    for (curr.x = 0; curr.x < size.x; curr.x += 1) {
      if (protocol->mapGet(curr) == EMPTY) {
        int buff = evaluate(curr, ATTACK);
        if (buff > max) {
          pt = curr;
          max = buff;
        }
        buff = evaluate(curr, DEFENSE);
        if (buff >= max) {
          pt = curr;
          max = buff;
        }
      }
    }
  }
  return pt;
  /*
  win = mapIterator(1);
  if (win.v == 0) {
    return randomEmptyPoint();
  }
  protocol->log("Debug " + std::to_string(win.pt.x) + ", " + std::to_string(win.pt.y));
  return win.pt;
  */
}
