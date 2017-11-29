
#pragma once

#include <thread>
#include <string>
#include <vector>

class IGame;

class Protocol
{
private:
  IGame* game;
public:
  Protocol(IGame*);
  virtual ~Protocol();
  void log(std::string const & cmd);
  void readLoop();
  void inputStart(std::string const &);
  std::string rawRecv();
  void rawSend(std::string const & cmd);
};

typedef void(Protocol::*t_command_input)(std::string const &);
