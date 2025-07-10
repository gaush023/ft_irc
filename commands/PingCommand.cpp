#include "../headers/Server.hpp"

std::string Server::_ping(Request request, int sender_fd)
{
    if (!this->_clients[sender_fd]->getRegistered())
        return _printMessage("451", this->_clients[sender_fd]->getNickName(), ":You have not registered");
    if (request.args.size() < 1)
        return _printMessage("461", this->_clients[sender_fd]->getNickName(), ":Not enough parameters");
    
    std::string response = "PONG " + this->_name + " :" + request.args[0] + "\r\n";
    _sendall(sender_fd, response);
    return "";
}

