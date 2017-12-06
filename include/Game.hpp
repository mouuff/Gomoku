#pragma once

#include <stdexcept>
#include <vector>
#include <tuple>
#include "Protocol.hpp"
#include "IGame.hpp"

#define EVAL_RANGE ((LINE_SIZE - 1))

enum Dir : int
{
  NORTH = 0,
  NORTH_EAST,
  EAST,
  SOUTH_EAST,
  SOUTH,
  SOUTH_WEST,
  WEST,
  NORTH_WEST
};

enum TEval : int
{
  ATTACK = 0,
  DEFENSE
};

struct Eval {
  int x;//0 to 4
  int score;
  Eval& operator+(Eval const & Eval) {
    this->x += Eval.x;
    this->score += Eval.score;
    return *this;
  }
};

class Game : public IGame
{
protected:
  Protocol* protocol;
public:
  Game();
  virtual ~Game();
  void start(Protocol*);
  void end();
  Point play();
  VPoint mapIterator(int);
  Point randomEmptyPoint();
  //Game Eval:
protected:
  Point directionToPoint(Dir dir) const;
  Eval evaluateDir(Point pos, Dir dir, TEval type, Tile origin_tile);
  Eval evaluateSum(Point pt, Dir dir1, Dir dir2, TEval type, Tile tl);
public:
  int evaluate(Point pt, TEval eval, Tile origin_tile = EMPTY);
};
