
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "Protocol.hpp"
#include "misc.h"

Protocol::Protocol()
{
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

void Protocol::readLoop()
{
  std::map<std::string, t_command_input> input_map = {
    {"START", &Protocol::inputStart}
  };
  while (true)
  {
    std::string cmd = this->rawRecv();
    std::vector<std::string> elems = split(cmd, ' ');
    //std::cout << "MESSAGE x" << vector.at(0) << "x"<< std::endl;
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
