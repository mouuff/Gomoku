
#pragma once

#include <thread>
#include <string>

class Protocol
{
public:
  Protocol();
  virtual ~Protocol();
  void rawSend(std::string const & cmd);
  std::string rawRecv();
};
