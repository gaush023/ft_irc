#pragma once

#include "Server.hpp"

class Server;
class Request;

class ICommand
{
  private:

  public:
    virtual ~ICommand() {}
    virtual std::string execute(Request &request, int sender_fd) = 0;
};


