
#include <sstream>
#include <string>
#include "misc.h"

std::vector<std::string> split(std::string const & s, char delim) {
  std::vector<std::string> vector;
  std::stringstream ss(s);
  std::string item;
  while (std::getline(ss, item, delim))
  {
    vector.push_back(item);
  }
  return vector;
}

long my_stol(std::string const & s)
{
  try {
    return std::stol(s);
  } catch (const std::exception& ex) {
    return (0);
  }
}
