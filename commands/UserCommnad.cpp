#include "./include/UserCommand.hpp"

std::string UserCommand::execute(Request request, int sender_fd)
{

    if (!this->_clients[sender_fd]->getAuth())
        return _printMessage("988", this->_clients[sender_fd]->getNickName(), ":You must be authenticated to set a username");
    if (this->_clients[sender_fd]->getRegistered())
        return _printMessage("462", this->_clients[sender_fd]->getNickName(), ":Unauthorized command (already registered)");
    if (request.args.size() < 4)
        return _printMessage("461", this->_clients[sender_fd]->getNickName(), ":Not enough parameters");
    this->_clients[sender_fd]->setUserName(request.args[0]);
    this->_clients[sender_fd]->setFullName(request.args[3]);
    if(this->_clients[sender_fd]->getNickName() != "")
    {
        this->_clients[sender_fd]->setID(this->_clients[sender_fd]->getUserName() + "!" + this->_clients[sender_fd]->getNickName() + "@" + this->_clients[sender_fd]->getHost());
        this->_clients[sender_fd]->setRegistered(true);
        return _printMessage("001", this->_clients[sender_fd]->getNickName(), ":Welcome to the Internet Relay Network " + this->_clients[sender_fd]->getID());
    }
    return ("");
}

