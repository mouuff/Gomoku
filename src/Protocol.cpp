
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

void Protocol::inputStart(std::string const & str)
{
  this->rawSend("OK");
}

void Protocol::inputAbout(std::string const & str)
{
  this->rawSend("name=\"pd35-Brain\", version=\"1.0\", author=\"pd35 team\", country=\"France\"");
}

void Protocol::inputInfo(std::string const & str)
{
  this->rawSend(str);
  std::map<std::string, t_command_info> info_map = {
    {"timeout_turn", NULL},
    {"timeout_match", NULL},
    {"max_memory", NULL},
    {"time_left", NULL},
    {"game_type", NULL},
    {"rule", NULL},
    {"evaluate", NULL},
    {"folder", NULL}
  };
  std::vector<std::string> elems = split(str, ' ');
}

void Protocol::readLoop()
{
  std::map<std::string, t_command_input> input_map = {
    {"START", &Protocol::inputStart},
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
      (this->*(input_map[key]))(cmd);
    }
  }
}
