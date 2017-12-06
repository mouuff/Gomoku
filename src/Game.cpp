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
  int val;

  win.v = 0;
  if (deepth % 2 != 0)
    player = Tile::OWN;
  else
    player = Tile::OPPONENT;
  for (pt.x = 0; pt.x < protocol->mapSize().x ; pt.x += 1) {
    for (pt.y = 0; pt.y < protocol->mapSize().y ; pt.y += 1) {
      if (protocol->mapGet(pt) != Tile::EMPTY)
        continue;
      protocol->mapGet(pt) = player;
      if (deepth != 0)
      {
        tmp = mapIterator(--deepth);
        //attackEvaluate(pt, player);
      }
        val = evaluate(pt, ATTACK);
        if (win.v < val)
        {
          win.v = val;
          win.x = pt.x;
          win.y = pt.y;
        }
      protocol->mapGet(pt) = Tile::EMPTY;
    }
  }
  protocol->log("Debug " + std::to_string(win.x) + ", " + std::to_string(win.y) + ", " + std::to_string(win.v));
  //protocol->log("Debug " + std::to_string(pt.x) + ", " + std::to_string(pt.y));
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

Attack Game::evaluateDir(Point pos, Dir dir, Eval eval, Tile team)
{
  Point ptdir = this->directionToPoint(dir);
  Point size = protocol->mapSize();
  Tile buff;
  Attack attack;
  attack.score = 0;
  //protocol->log("Debug: " + std::to_string(pos.x) + ", " + std::to_string(pos.y));
  for (attack.x = 0; attack.x < EVAL_RANGE; attack.x += 1) {
    pos.x += ptdir.x;
    pos.y += ptdir.y;
    if (pos.x < 0 || pos.x >= size.x || pos.y < 0 || pos.y >= size.y)
      break;
    buff = protocol->mapGet(pos);
    if (eval == ATTACK)
    {
      if (buff == team) {
        attack.score += EVAL_RANGE;
      }
      else if (buff == EMPTY) {
        attack.score += 1;
      }
      else {
        break;
      }
    }
    else if (eval == DEFENSE){
      if (buff == team) {
        attack.score += EVAL_RANGE;
      }
      else {
        break;
      }
    }
  }
  return attack;
}

int Game::evaluate(Point pt, Eval eval, Tile tl)
{
  if (tl == EMPTY) {
    if (eval == ATTACK)
      tl = OWN;
    else
      tl = OPPONENT;
  }
  Attack max;
  Attack buff;
  max.x = 0;
  max.score = 0;
  if (eval == ATTACK) {
    buff = this->evaluateDir(pt, NORTH, eval, tl) + this->evaluateDir(pt, SOUTH, eval, tl);
    if ((buff.x >= EVAL_RANGE && buff.score > max.score)
      max = buff;
    buff = this->evaluateDir(pt, NORTH_EAST, eval, tl) + this->evaluateDir(pt, SOUTH_WEST, eval, tl);
    if ((buff.x >= EVAL_RANGE && buff.score > max.score)
      max = buff;
    buff = this->evaluateDir(pt, SOUTH_EAST, eval, tl) + this->evaluateDir(pt, NORTH_WEST, eval, tl);
    if ((buff.x >= EVAL_RANGE && buff.score > max.score)
      max = buff;
    buff = this->evaluateDir(pt, WEST, eval, tl) + this->evaluateDir(pt, EAST, eval, tl);
    if ((buff.x >= EVAL_RANGE && buff.score > max.score)
      max = buff;
  } else if (eval == DEFENSE){
    buff = this->evaluateDir(pt, NORTH, eval, tl) + this->evaluateDir(pt, SOUTH, eval, tl);
    if (buff.score > max.score)
      max = buff;
    buff = this->evaluateDir(pt, NORTH_EAST, eval, tl) + this->evaluateDir(pt, SOUTH_WEST, eval, tl);
    if (buff.score > max.score)
      max = buff;
    buff = this->evaluateDir(pt, SOUTH_EAST, eval, tl) + this->evaluateDir(pt, NORTH_WEST, eval, tl);
    if (buff.score > max.score)
      max = buff;
    buff = this->evaluateDir(pt, WEST, eval, tl) + this->evaluateDir(pt, EAST, eval, tl);
    if (buff.score > max.score)
      max = buff;
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
  //Point size = protocol->mapSize();
  //Point curr;
  //char max = 0;
  // for (curr.y = 0; curr.y < size.y; curr.y += 1) {
  //   for (curr.x = 0; curr.x < size.x; curr.x += 1) {
  //     if (protocol->mapGet(curr) == EMPTY) {
  //       int buff = attackEvaluate(curr, OWN);
  //       if (buff > max)
  //       {
  //         pt = curr;
  //         max = buff;
  //       }
  //     }
  //   }
  // }
  win = mapIterator(1);
  if (win.v == 0) {
    return randomEmptyPoint();
  }
  protocol->log("Debug " + std::to_string(win.x) + ", " + std::to_string(win.y));
  pt.x = win.x;
  pt.y = win.y;
  return pt;
}
