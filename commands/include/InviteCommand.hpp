#pragma once

#include "../../include/ICommand.hpp"

class InviteCommand : public ICommand
{
  private:
    Server &_server;
    std::string _inviteToChannel(const std::string& channelName, const std::string& userName, int sender_fd);
    std::string _inviteToPrvChannel(const std::string& channelName, const std::string& userName, int sender_fd);


  public:
    std::string execute(Request &request, int sender_fd);
    InviteCommand(Server &server);
};
