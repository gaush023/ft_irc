#pragma once

#include "../../include/ICommand.hpp"

class JoinCommand : public ICommand {
public:
    JoinCommand(Server& server);

    std::string execute(Request& request, int sender_fd);

private:
    std::vector<std::string> _commaSeparator(const std::string& s) const;

    int _createChannel(const std::string& channelName, int creatorFd);
    int _createPrvChannel(const std::string& channelName, const std::string& key, int creatorFd);

    Server& _server;  
};
