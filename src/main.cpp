
#include <iostream>
#include <fstream>
#include <string>
#include "gomoku.h"

int main()
{
  std::string tmp;
  Protocol* protocol = new Protocol();
  protocol->readLoop();
  /*
  tmp = protocol->rawRecv();
  protocol->rawSend("MESSAGE got " + tmp);
  protocol->rawSend("OK");
  */
  return 0;
}
