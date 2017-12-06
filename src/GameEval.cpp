#include <iostream>
#include "Protocol.hpp"
#include "Game.hpp"
#include "misc.h"

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

Eval Game::evaluateSum(Point pt, Dir dir1, Dir dir2, TEval type, Tile team)
{
  return (this->evaluateDir(pt, dir1, type, team)
          + this->evaluateDir(pt, dir2, type, team));
}

int Game::evaluate(Point pt, TEval type, Tile team)
{
  if (team == EMPTY) {
    team = (type == ATTACK ? OWN : OPPONENT);
  }
  Eval evals[] = {
    this->evaluateSum(pt, NORTH, SOUTH, type, team),
    this->evaluateSum(pt, NORTH_EAST, SOUTH_WEST, type, team),
    this->evaluateSum(pt, SOUTH_EAST, NORTH_WEST, type, team),
    this->evaluateSum(pt, WEST, EAST, type, team)
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
