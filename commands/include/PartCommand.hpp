#pragma once

#include "../../include/ICommand.hpp"

class PartCommand : public ICommand
{
  public:
    PartCommand(Server &server);
    virtual std::string execute(Request &request, int sender_fd);

  private:
    int _partChannel(const std::string& channelName, int arget_fd, const std::string& message, int isPart);
    Server &_server;
};
