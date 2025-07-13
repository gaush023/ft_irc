#pragma once

#include "../../include/ICommand.hpp"
#include <sstream>

class HelpDesk : public ICommand
{

  public:
      std::string execute(Request& request, int sender_fd);
      HelpDesk(Server& server);
  private:
      std::string _getBotMessage() const;
      std::string _serverInfo() const;
      std::string _channelInfo(const std::string& channelName) const;
      std::string _listAllChannels() const;
      Server& _server; 
};
