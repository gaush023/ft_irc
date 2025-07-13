#pragma once

#include "../../include/ICommand.hpp"

class KickCommand : public ICommand {
  public:
      KickCommand(Server& server);
      std::string execute(Request& request, int sender_fd);

  private:
      std::vector<std::string> _commaSeparator(const std::string& s) const;
      std::string _kickFromChannel(const std::string& channelName,
                                  const std::string& message,
                                  const std::vector<std::string>& users,
                                  int sender_fd);
      Server& _server;
};
