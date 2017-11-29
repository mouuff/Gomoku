#pragma once

#include "Protocol.hpp"
#include "Point.hpp"

class IGame
{
public:
  virtual void start(Protocol*) = 0;
  virtual void end() = 0;
  virtual Point play() = 0;
  virtual ~IGame() {}
};
