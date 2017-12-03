#include "Game.hpp"

Game::Game()
{

}

Game::~Game() {}

void Game::start(Protocol* protocol)
{
  this->protocol = protocol;
}

void Game::end()
{

}

Point Game::play()
{
  Point pt;
  pt.x = 1;
  pt.y = 3;
  return pt;
}
