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
  Point randomEmptyPoint();
  Attack Game::attackEvaluateDir(Point pos, Dir dir, Tile origin_tile);
  int attackEvaluate(Point pt, Tile origin_tile = OWN);
  Point directionToPoint(Dir dir) const;
};
