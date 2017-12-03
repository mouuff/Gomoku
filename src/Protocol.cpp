
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "Protocol.hpp"
#include "misc.h"

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

bool Protocol::initMap(int size_x, int size_y)
{
  //this->log("start size: " + size_x + ", " + size_y);
  if (size_x < MIN_SIZE || size_y < MIN_SIZE)
    return false;
  this->size_x = size_x;
  this->size_y = size_y;
  std::vector<Tile> buff;
  map.clear();
  for (int x = 0; x < size_x; x += 1) {
    buff.clear();
    for (int y = 0; y < size_y; y += 1) {
      buff.push_back(Tile::EMPTY);
    }
    map.push_back(buff);
  }
  return true;
}

Tile Protocol::mapGet(int x, int y)
{
  return map.at(x).at(y);
}

void Protocol::inputStart(std::string const & str)
{
  long size = my_stol(str);
  if (this->initMap(size, size))
    this->rawSend("OK");

  for (int x = 0; x < size_x; x += 1) {
    std::string line;
    for (int y = 0; y < size_y; y += 1) {
      line += std::to_string((int)mapGet(x, y)) + ", ";
    }
    this->log(line);
  }
}


void Protocol::inputBegin(std::string const & str)
{
  
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
  else if (name == "rule")
    this->rule = n_info;
  else if (name == "evaluate")
    this->evaluate = n_info;
  else if (name == "folder")
    this->folder = str_info;
  else
    this->log("Unknown info: " + name);
}

void Protocol::readLoop()
{
  std::map<std::string, t_command_input> input_map = {
    {"START", &Protocol::inputStart},
    {"BEGIN", &Protocol::inputBegin},
    {"ABOUT", &Protocol::inputAbout},
    {"INFO", &Protocol::inputInfo}
  };
  while (true)
  {
    std::string cmd = this->rawRecv();
    std::vector<std::string> elems = split(cmd, ' ');
    std::string key = elems.at(0);
    if (input_map.find(key) == input_map.end())
    {
      this->log("Not found key: " + key);
    }
    else
    {
      std::string sub = cmd.substr(key.length() + 1);
      (this->*(input_map[key]))(sub);
    }
  }
}
