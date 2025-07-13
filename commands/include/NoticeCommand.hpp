#pragma once

#include "../../include/ICommand.hpp"

class NoticeCommand : public ICommand
{
  public:
    NoticeCommand(Server &server);
    std::string execute(Request &request, int sender_fd);
  private:
    Server &_server;
};
