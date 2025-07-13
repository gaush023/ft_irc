#pragma once

#include "../../include/ICommand.hpp"

class OperCommand : public ICommand
{
  public:
    OperCommand(Server &server);
    virtual std::string execute(Request &request, int sender_fd);
  private:
    Server &_server;
};
