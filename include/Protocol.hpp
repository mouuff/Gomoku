
#pragma once

#include <thread>
#include <string>
#include <vector>
#include "IGame.hpp"

#define MIN_SIZE 5

enum Tile : int {
  EMPTY = 0,
  OWN,
  OPPONENT
};

class Protocol
{
protected:
  IGame* game;
  // Vars:
  int size_x;
  int size_y;
  std::vector<std::vector<Tile>> map;
  //Infos:
  long timeout_turn;
  long timeout_match;
  long max_memory;
  long time_left;
  long game_type;
  long rule;
  long evaluate;
  std::string folder;
protected:
  bool init(int size_x, int size_y);
  std::string rawRecv();
  void rawSend(std::string const & cmd);
  void play();
public:
  Protocol(IGame*);
  virtual ~Protocol();
  Tile mapGet(int x, int y);
  void log(std::string const & cmd);
  void readLoop();
  void inputStart(std::string const &);
  void inputAbout(std::string const &);
  void inputInfo(std::string const &);
  void inputBegin(std::string const &);
};

typedef void(Protocol::*t_command_input)(std::string const &);
