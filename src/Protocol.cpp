
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "Protocol.hpp"
#include "Point.hpp"
#include "misc.h"

//rm:
#include "Game.hpp"

Protocol::Protocol(IGame* _game)
{
  this->game = _game;
  if (this->game == nullptr)
  {
    throw std::invalid_argument("Error: null IGame");
  }
}

Protocol::~Protocol()
{
}

void Protocol::rawSend(std::string const & cmd)
{
  std::cout << cmd << std::endl;
}

void Protocol::log(std::string const & cmd)
{
  std::cout << "MESSAGE " << cmd << std::endl;
}

std::string Protocol::rawRecv()
{
  std::string line;
  std::getline(std::cin, line);
  return line;
}

Point Protocol::mapSize() const
{
  Point ret;
  ret.x = size_x;
  ret.y = size_y;
  return ret;
}

Tile& Protocol::mapGet(int x, int y) {
  return map.at(x).at(y);
}

Tile& Protocol::mapGet(Point const & pt) {
  return this->mapGet(pt.x, pt.y);
}

void Protocol::logMap() {
  for (int y = 0; y < size_y; y += 1) {
    std::string line;
    for (int x = 0; x < size_x; x += 1) {
      line += std::to_string((int)mapGet(x, y)) + ", ";
    }
    this->log(line);
  }
}

bool Protocol::init(int size_x, int size_y)
{
  //this->log("start size: " + size_x + ", " + size_y);
  if (size_x < MIN_SIZE || size_y < MIN_SIZE)
    return false;
  this->size_x = size_x;
  this->size_y = size_y;
  std::vector<Tile> buff;
  map.clear();
  for (int y = 0; y < size_y; y += 1) {
    buff.clear();
    for (int x = 0; x < size_x; x += 1) {
      buff.push_back(Tile::EMPTY);
    }
    map.push_back(buff);
  }
  game->start(this);
  return true;
}

void Protocol::inputStart(std::string const & str)
{
  long size = my_stol(str);
  if (this->init(size, size))
    this->rawSend("OK");
}

void Protocol::inputRectstart(std::string const & str)
{
  std::vector<std::string> elems = split(str, ',');
  if (elems.size() != 2) {
    this->log("Bad rectstart: " + str);
  }
  int width = my_stol(elems.at(0));
  int height = my_stol(elems.at(1));
  if (this->init(width, height))
    this->rawSend("OK");
}

void Protocol::inputRestart(std::string const & str)
{
  if (this->init(size_x, size_y))
    this->rawSend("OK");
}

void Protocol::inputTurn(std::string const & str)
{
  std::vector<std::string> elems = split(str, ',');
  if (elems.size() != 2) {
    this->log("Bad turn: " + str);
    return;
  }
  Point pt;
  pt.x = (char)my_stol(elems.at(0));
  pt.y = (char)my_stol(elems.at(1));
  this->mapGet(pt) = OPPONENT;

  //DEBUG:
  //this->logMap();
  //this->log("evaluate: " + std::to_string((int)((Game*)game)->evaluate(pt, ATTACK)));
  //DEBUG
  this->play();
}

void Protocol::play()
{
  Point move = game->play();
  this->mapGet(move) = OWN;
  //this->logMap();
  this->rawSend(std::to_string(move.x) + "," + std::to_string(move.y));
}

void Protocol::inputBegin(std::string const & str)
{
  play();
}

void Protocol::inputAbout(std::string const & str)
{
  this->rawSend("name=\"pd35-Brain\", version=\"1.0\", author=\"pd35 team\", country=\"France\"");
}

void Protocol::inputInfo(std::string const & str)
{
  std::vector<std::string> elems = split(str, ' ');
  if (elems.size() != 2) {
    this->log("Bad info: " + str);
  }
  std::string name = elems.at(0);
  std::string str_info = elems.at(1);
  long n_info;
  n_info = my_stol(str_info);
  if (name == "timeout_turn")
    this->timeout_turn = n_info;
  else if (name == "timeout_match")
    this->timeout_match = n_info;
  else if (name == "max_memory")
    this->max_memory = n_info;
  else if (name == "time_left")
    this->time_left = n_info;
  else if (name == "game_type")
    this->game_type = n_info;
  else if (name == "rule")
    this->rule = n_info;
  else if (name == "evaluate")
    this->evaluate = n_info;
  else if (name == "folder")
    this->folder = str_info;
  else
    this->log("Unknown info: " + name);
}

void Protocol::inputBoard(std::string const & str)
{
  std::string last;
  std::vector<std::string> elems;
  while (true)
  {
    last = this->rawRecv();
    if (last == "DONE")
      break;
    elems = split(last, ',');
    if (elems.size() != 3) {
      this->log("Bad board: " + last);
      return;
    }
    Point pt;
    pt.x = (char)my_stol(elems.at(0));
    pt.y = (char)my_stol(elems.at(1));
    int stone = my_stol(elems.at(2));
    if (stone == 1) {
      mapGet(pt) = Tile::OWN;
    }
    else if (stone == 2) {
      mapGet(pt) = Tile::OPPONENT;
    }
  }
  this->play();
}

void Protocol::inputTakeback(std::string const & str)
{
  std::vector<std::string> elems = split(str, ',');
  if (elems.size() != 2) {
    this->log("Bad takeback: " + str);
    return;
  }
  Point pt;
  pt.x = (char)my_stol(elems.at(0));
  pt.y = (char)my_stol(elems.at(1));
  this->mapGet(pt) = Tile::EMPTY;
  this->rawSend("OK");
}

void Protocol::readLoop()
{
  std::map<std::string, t_command_input> input_map = {
    {"START", &Protocol::inputStart},
    {"RESTART", &Protocol::inputRestart},
    {"RECTSTART", &Protocol::inputRectstart},
    {"BEGIN", &Protocol::inputBegin},
    {"ABOUT", &Protocol::inputAbout},
    {"TURN", &Protocol::inputTurn},
    {"BOARD", &Protocol::inputBoard},
    {"TAKEBACK", &Protocol::inputTakeback},
    {"INFO", &Protocol::inputInfo}
  };
  std::string cmd, key;
  std::vector<std::string> elems;
  while (true)
  {
    cmd = this->rawRecv();
    if (cmd == "")
      continue;
    elems = split(cmd, ' ');
    key = elems.at(0);
    if (input_map.find(key) == input_map.end())
    {
      this->log("Not found key: " + key);
    }
    else
    {
      std::string res = cmd.substr(key.length());
      (this->*(input_map[key]))(res);
    }
  }
}
