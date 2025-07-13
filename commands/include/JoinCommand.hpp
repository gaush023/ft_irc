#pragma once

#include "../../../ICommand.hpp"

class JoinCommand : public ICommand
{
  public:
    std::string execute(Request &request, int sender_fd) = 0;
    int _createChannel( std::string ChannelName, int CreatorFd)
    int _createPrvChannel( std::string ChannelName, std::string Key, int CreatorFd)
    std::vector<std::string> _commaSEparator(std::string arg);
};
