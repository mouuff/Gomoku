#pragma once

#include <stdexcept>
#include <vector>
#include "Protocol.hpp"
#include "IGame.hpp"

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
  int evaluateDir(Point pt, Dir dir, Tile origin_tile);
  int evaluate(Point pt, Tile origin_tile);
  Point directionToPoint(Dir dir) const;
};
