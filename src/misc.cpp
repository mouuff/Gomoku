
#include <sstream>
#include "misc.h"

std::vector<std::string> split(const std::string & s, char delim) {
  std::vector<std::string> vector;
  std::stringstream ss(s);
  std::string item;
  while (std::getline(ss, item, delim))
  {
    vector.push_back(item);
  }
  return vector;
}
