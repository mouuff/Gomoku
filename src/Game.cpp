#include <iostream>
#include "Protocol.hpp"
#include "Game.hpp"
#include "misc.h"

Game::Game()
{
  this->protocol = nullptr;
}

Game::~Game() {}

void Game::start(Protocol* protocol)
{
  this->protocol = protocol;
}

void Game::end()
{

}
/*
VPoint Game::mapIterator(int deepth) {
  register Point pt = Point();
  register VPoint win = VPoint();
  register VPoint tmp = VPoint();
  Tile player;
  TEval type;
  int val;
  int val2;

  win.v = 0;
  tmp.v = 0;
  if (deepth % 2 != 0) {
    player = Tile::OWN;
    type = ATTACK;
  } else {
    player = Tile::OPPONENT;
    type = DEFENSE;
  }
  for (pt.x = 0; pt.x < protocol->mapSize().x ; pt.x += 1) {
    for (pt.y = 0; pt.y < protocol->mapSize().y ; pt.y += 1) {
      if (protocol->mapGet(pt) == Tile::EMPTY) {
        protocol->mapGet(pt) = player;
        val = evaluate(pt, ATTACK);
        val2 = evaluate(pt, DEFENSE);
        if (val < val2)
          val = val2;
        if (deepth != 0) {
          tmp = mapIterator((deepth - 1));
        }
        if (win.v < (val - (tmp.v / 2))) {
          win.v = val;
          win.pt = pt;
        }
        protocol->mapGet(pt) = Tile::EMPTY;
      }
    }
  }
  // protocol->log("Debug " + std::to_string(win.pt.x) + ", " + std::to_string(win.pt.y) + ", " + std::to_string(win.v) + ", deepth =" + std::to_string(deepth));
  // protocol->log("Debug " + std::to_string(pt.x) + ", " + std::to_string(pt.y));
  return win;
}
*/

Point Game::findMapMiddle()
{
  Point best;
  best.x = protocol->mapSize().x / 2;
  best.y = protocol->mapSize().y / 2;
  for (; best.y < protocol->mapSize().y; best.y += 1) {
    for (; best.x < protocol->mapSize().x; best.x += 1) {
      if (protocol->mapGet(best) == EMPTY)
        return best;
    }
  }
  return best;
}

bool Game::isFirst()
{
  Point curr;
  for (curr.y = 0; curr.y < protocol->mapSize().y; curr.y += 1) {
    for (curr.x = 0; curr.x < protocol->mapSize().x; curr.x += 1) {
      if (protocol->mapGet(curr) != EMPTY) {
        return false;
      }
    }
  }
  return true;
}

Point Game::play()
{
  Point best = Point();
  if (protocol == nullptr) {
    std::cout << "MESSAGE protocol not set in game" << std::endl;
    return best;
  }
  if (this->isFirst()) {
    return findMapMiddle();
  }
  std::vector<PtEval> best_defenses = evaluateMap(DEFENSE, OPPONENT);
  std::vector<PtEval> best_attacks = evaluateMap(DEFENSE, OWN);
  if (best_attacks.at(0).eval.score >= best_defenses.at(0).eval.score) {
    best = whoIsTheBest(evaluateMap(ATTACK));
    protocol->log("ATTACK");
  }
  else {
    best = whoIsTheBest(best_defenses);
    protocol->log("DEFENSE");
  }
  return best;
  /*
  win = mapIterator(1);
  if (win.v == 0) {
    return randomEmptyPoint();
  }
  protocol->log("Debug " + std::to_string(win.pt.x) + ", " + std::to_string(win.pt.y));
  return win.pt;
  */
}
