#pragma once

#include "Protocol.hpp"
#include "IGame.hpp"

class Game : public IGame
{
public:
  Game();
  virtual ~Game();
  void start(Protocol*);
  void end();
  Point play();
};
