#pragma once

#include "../../../ICommand.hpp"

class HelpDesk : public ICommand
{
  public:
    std::string execute(Request request, int sender_fd) override;
    std::string _listAllChannels() const;
    std::string _channelInfo(std::string ChannelName, int fd) const;
    std::string _serverInfo(int fd) const;
    std::string _getBotMessage() const;
};
