
#include <sstream>
#include <string>
#include <random>
#include "misc.h"

std::vector<std::string> split(std::string const & s,
                              char delim,
                              bool strip) {
  std::vector<std::string> vector;
  std::stringstream ss(s);
  std::string item;
  while (std::getline(ss, item, delim))
  {
    if (strip && item == "")
      continue;
    vector.push_back(item);
  }
  return vector;
}

long my_stol(std::string const & s)
{
  try {
    return std::stol(s);
  } catch (const std::exception& ex) {
    (void)ex;
    return (0);
  }
}

int my_randint(int min, int max)
{
  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_int_distribution<int> uni(min, max);
  return uni(rng);
}
