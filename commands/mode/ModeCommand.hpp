#pragma once


#include "../../include/server.hpp"

class ModeCommand : public ICommand {
  public:
      explicit ModeCommand(Server& server);

      std::string execute(Request& request, int sender_fd);

  private:
      std::string _handleUserMode   (Request& req, int sender_fd);
      std::string _handleChannelMode(Request& req, int sender_fd);
      std::string _err(const std::string& code, const std::string& nick, const std::string& txt) const { return _server.printMessage(code, nick, txt); }

      Server& _server;
};
