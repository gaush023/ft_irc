#pragma once

#include "../../../ICommand.hpp"


class PrivmsgCommand : public ICommand
{
  public:
    std::string execute(Request request, int sender_fd) override
    std::string _privToUser(std::string User, std::string msg, std::string cmd, int fd)
    std::string _privToChannel(std::string ChannelName, std::string masseage, int fd)
};
