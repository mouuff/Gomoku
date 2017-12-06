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

Eval Game::evaluateDir(Point pos, Dir dir, TEval type, Tile team)
{
  Point ptdir = this->directionToPoint(dir);
  Point size = protocol->mapSize();
  Tile buff;
  Eval eval;
  eval.score = 0;
  //protocol->log("Debug: " + std::to_string(pos.x) + ", " + std::to_string(pos.y));
  for (eval.x = 0; eval.x < EVAL_RANGE; eval.x += 1) {
    pos.x += ptdir.x;
    pos.y += ptdir.y;
    if (pos.x < 0 || pos.x >= size.x || pos.y < 0 || pos.y >= size.y)
      break;
    buff = protocol->mapGet(pos);
    if (type == ATTACK)
    {
      if (buff == team) {
        eval.score += EVAL_RANGE;
      }
      else if (buff == EMPTY) {
        eval.score += 1;
      }
      else {
        break;
      }
    }
    else if (type == DEFENSE){
      if (buff == team) {
        eval.score += EVAL_RANGE;
      }
      else {
        break;
      }
    }
  }
  return eval;
}

Eval Game::evaluateSum(Point pt, Dir dir1, Dir dir2, TEval type, Tile tl)
{
  return (this->evaluateDir(pt, dir1, type, tl)
          + this->evaluateDir(pt, dir2, type, tl));
}

int Game::evaluate(Point pt, TEval type, Tile tl)
{
  if (tl == EMPTY) {
    tl = (type == ATTACK ? OWN : OPPONENT);
  }
  Eval evals[] = {
    this->evaluateSum(pt, NORTH, SOUTH, type, tl),
    this->evaluateSum(pt, NORTH_EAST, SOUTH_WEST, type, tl),
    this->evaluateSum(pt, SOUTH_EAST, NORTH_WEST, type, tl),
    this->evaluateSum(pt, WEST, EAST, type, tl)
  };
  Eval max;
  Eval* buff;
  max.x = 0;
  max.score = 0;
  if (type == ATTACK) {
    for (int x = 0; x < 4; x += 1) {
      buff = &(evals[x]);
      if (buff->x >= EVAL_RANGE && buff->score > max.score)
        max = *buff;
    }
  }
  else if (type == DEFENSE){
    for (int x = 0; x < 4; x += 1) {
      buff = &(evals[x]);
      if (buff->score > max.score)
        max = *buff;
    }
  }
  return max.score;
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
        int buff = evaluate(curr, DEFENSE);
        if (buff > max) {
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
