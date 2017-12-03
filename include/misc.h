#pragma once

#include <vector>
#include <iostream>
#include <string>

std::vector<std::string> split(std::string const & s,
                              char delim,
                              bool strip = true);
long my_stol(std::string const & s);
int my_randint(int min, int max);
