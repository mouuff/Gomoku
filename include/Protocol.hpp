
#pragma once

#include <thread>
#include <string>
#include <vector>

class Protocol
{
public:
  Protocol();
  virtual ~Protocol();
  void rawSend(std::string const & cmd);
  void log(std::string const & cmd);
  std::string rawRecv();
  void readLoop();
  void inputStart(std::string const &);
  void inputAbout(std::string const &);
  void inputInfo(std::string const &);
};

typedef void(Protocol::*t_command_input)(std::string const &);
typedef void(Protocol::*t_command_info)();
