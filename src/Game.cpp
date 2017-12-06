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

Point Game::directionToPoint(Dir dir) const
{
  Point pt;
  pt.x = 0;
  pt.y = 0;
  switch (dir)
  {
    case NORTH:
      pt.y -= 1;
      break;
    case NORTH_EAST:
      pt.y -= 1;
      pt.x += 1;
      break;
    case EAST:
      pt.x += 1;
      break;
    case SOUTH_EAST:
      pt.y += 1;
      pt.x += 1;
      break;
    case SOUTH:
      pt.y += 1;
      break;
    case SOUTH_WEST:
      pt.y += 1;
      pt.x -= 1;
      break;
    case WEST:
      pt.x -= 1;
      break;
    case NORTH_WEST:
      pt.x -= 1;
      pt.y -= 1;
      break;
    default:
      throw std::runtime_error("Invalid direction");
  }
  return pt;
}

int Game::attackEvaluateDir(Point origin, Dir dir, Tile origin_tile)
{
  Point ptdir = this->directionToPoint(dir);
  Point pos = origin;
  Point size = protocol->mapSize();
  Tile buff;
  int x;
  int score = 0;
  if (origin_tile == EMPTY)
    throw std::runtime_error("attackEvaluate empty pos");
  //protocol->log("Debug: " + std::to_string(pos.x) + ", " + std::to_string(pos.y));
  for (x = 0; x < LINE_SIZE - 1; x += 1) {
    pos.x += ptdir.x;
    pos.y += ptdir.y;
    if (pos.x < 0 || pos.x >= size.x || pos.y < 0 || pos.y >= size.y)
      break;
    buff = protocol->mapGet(pos);
    if (buff == origin_tile) {
      score += 2;
    }
    else if (buff == EMPTY) {
      score += 1;
    }
    else {
      break;
    }
  }
  if (x < LINE_SIZE - 1) {
    return 0;
  }
  return score;
}

int Game::attackEvaluate(Point pt, Tile tl)
{
  int max = 0;
  int buff;
  buff = this->attackEvaluateDir(pt, NORTH, tl) + this->attackEvaluateDir(pt, SOUTH, tl);
  if (buff > max)
    max = buff;
  buff = this->attackEvaluateDir(pt, NORTH_EAST, tl) + this->attackEvaluateDir(pt, SOUTH_WEST, tl);
  if (buff > max)
    max = buff;
  buff = this->attackEvaluateDir(pt, SOUTH_EAST, tl) + this->attackEvaluateDir(pt, NORTH_WEST, tl);
  if (buff > max)
    max = buff;
  buff = this->attackEvaluateDir(pt, WEST, tl) + this->attackEvaluateDir(pt, EAST, tl);
  if (buff > max)
    max = buff;
  return max;
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
        int buff = attackEvaluate(curr, OWN);
        if (buff > max)
        {
          pt = curr;
          max = buff;
        }
      }
    }
  }
  if (max == 0) {
    return randomEmptyPoint();
  }
  return pt;
}
