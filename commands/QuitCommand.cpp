#include "./includes/QuitCommand.hpp"

std::string QuitCommand::execute(Request request, int sender_fd)
{
    std::string ret = this->_clients[sender_fd]->getUserPerfix() + "QUIT ";
    if (request.args.size())
        ret.append(":" + request.args[0] + "\n");
    else
        ret.append("\n");
    std::map<std::string, Channel *> channels = this->_clients[sender_fd]->getJoinedChannels();
    std::map<std::string, Channel *>::iterator it = channels.begin();
    while (it != channels.end())
    {
        _sendToAllUsers(it->second, sender_fd, ret);
        it++;
    }
    this->_clients[sender_fd]->leaveAllChannels();
    close(this->_clients[sender_fd]->getClientfd());
    return ("QUIT");
};
