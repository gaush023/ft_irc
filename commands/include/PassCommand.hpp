#pragma once

#include "../../include/ICommand.hpp"

class PassCommand : public ICommand
{
  public:
    std::string execute(Request &request, int sender_fd);
    PassCommand(Server& server, const std::string& password);
  private:
    Server &_server;
};
