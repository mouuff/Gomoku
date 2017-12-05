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

char Game::evaluateDir(Point origin, Dir dir, Tile origin_tile)
{
  Point ptdir = this->directionToPoint(dir);
  Point pos = origin;
  Point size = protocol->mapSize();
  char score = 0;
  if (origin_tile == EMPTY)
    throw std::runtime_error("Evaluate empty pos");
  for (int x = 0; x < 4; x += 1) {
    pos.x += ptdir.x;
    pos.y += ptdir.y;
    if (pos.x < 0 || pos.x >= size.x || pos.y < 0 || pos.y >= size.y)
      break;
    if (protocol->mapGet(pos) == origin_tile) {
      score += 1;
    }
    else {
      break;
    }
  }
  return score;
}

char Game::evaluate(Point pt, Tile origin_tile)
{
  char max = 0;
  char buff;
  buff = this->evaluateDir(pt, NORTH, origin_tile)
        + this->evaluateDir(pt, SOUTH, origin_tile);
  if (buff > max)
    max = buff;
  buff = this->evaluateDir(pt, NORTH_EAST, origin_tile)
        + this->evaluateDir(pt, SOUTH_WEST, origin_tile);
  if (buff > max)
    max = buff;
  buff = this->evaluateDir(pt, SOUTH_EAST, origin_tile)
          + this->evaluateDir(pt, NORTH_WEST, origin_tile);
  if (buff > max)
    max = buff;
  return max;
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
    if (true)
    pt.x = my_randint(0, protocol->mapSize().x - 1);
    pt.y = my_randint(0, protocol->mapSize().y - 1);
  } while (protocol->mapGet(pt) != Tile::EMPTY);
  return pt;
}
