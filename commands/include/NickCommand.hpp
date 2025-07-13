#pragma once

#include "../../include/ICommand.hpp"

class NickCommand : public ICommand
{
  public:
    NickCommand(Server &server);
    std::string execute(Request &request, int sender_fd);
  private:
    Server &_server;
};
