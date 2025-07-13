#pragma once

#include "../../../include/ICommand.hpp"
#include <string>

class GetFileCommand : public ICommand {
public:
    GetFileCommand(Server& server);
    std::string execute(const Request& request, int sender_fd);
private:
    Server& _server;
};
