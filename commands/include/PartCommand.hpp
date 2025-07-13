#pragma once

#include "../../../ICommand.hpp"

class PartCommand : public ICommand
{
  public:
    std::string execute(Request &request, int sender_fd) = 0;
    int _partChannel(std::string ChannelName, int i, std::string message, int isPart);
};
