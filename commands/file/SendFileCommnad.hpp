#pragma once

#include "../../../include/ICommand.hpp"
#include <string>

class SendFileCommand : public ICommand {
public:
    SendFileCommand(Server& server);
    std::string execute(const Request& request, int sender_fd);
private:
    Server& _server;
};
