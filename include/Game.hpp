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

enum Eval : int
{
  ATTACK = 0,
  DEFENSE
};

struct Attack {
  int x;//0 to 4
  int score;
  Attack& operator+(Attack const & attack) {
    this->x += attack.x;
    this->score += attack.score;
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
  //GameEval:
protected:
  Point directionToPoint(Dir dir) const;
  Attack evaluateDir(Point pos, Dir dir, Eval eval, Tile origin_tile);
  Attack evaluateSum(Point pt, Dir dir1, Dir dir2, Eval eval, Tile tl);
public:
  int evaluate(Point pt, Eval eval, Tile origin_tile = EMPTY);
};
