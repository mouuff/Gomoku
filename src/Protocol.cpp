
#include <iostream>
#include <fstream>
#include <string>
#include "Protocol.hpp"

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

std::string Protocol::rawRecv()
{
  std::string line;
  std::getline(std::cin, line);
  return line;
}
