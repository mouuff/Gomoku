#pragma once

#include <vector>
#include "Protocol.hpp"
#include "IGame.hpp"

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
};
