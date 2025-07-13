#pragma once

#include "../../../ICommand.hpp"


class KickCommand : public ICommand
{
  public:
    std::string execute(Request &request, int sender_fd) = 0;
    std::string _kickedFromChannel(std::string ChannelName, std::string message, std::vector<std::string> users, int i);

}
