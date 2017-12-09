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


struct PtEval
{
  Point pt;
  Eval eval;
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
  //Game Brain:
protected:
  VPoint mapIterator(int);
  //Game Eval:
protected:
  Point directionToPoint(Dir dir) const;
  Eval evaluateDir(Point pos, Dir dir, TEval type, Tile team);
  Eval evaluateSum(Point pt, Dir dir1, Dir dir2, TEval type, Tile team);
public:
  Eval evaluate(Point pt, TEval type, Tile team = EMPTY);
  std::vector<PtEval> evaluateMap(TEval type, Tile team = EMPTY);
  Point whoIsTheBest(std::vector<PtEval> bests);
};
