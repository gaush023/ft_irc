#pragma once

#include "../../../ICommand.hpp"

class InviteCommand : public ICommand
{
  private:
    std::string _inviteToChannel(std::string ChannelName, std::string UserName, int sender_fd);
    std::string _inviteToPrvChannel(std::string ChannelName, std::string UserName, int sender_fd);
    Server &_server;
  
  public:
    std::string execute(Request &request, int sender_fd) = 0;
    InviteCommand(Server &server);
};
