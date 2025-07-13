#include "./include/NickCommand.hpp"

std::string NickCommand::execute(Request request, int sender_fd)
{
  if (!this->_clients[sender_fd]->getAuth())
    return _printMessage("988", this->_clients[sender_fd]->getNickName(), ":You must be authenticated to set a nickname");
  if (request.args.size() < 1)
    return _printMessage("431", this->_clients[sender_fd]->getNickName(), ":Not enough parameters");
  
  int pos = 0;
  while(request.args[0][pos])
  {
    if (!isalnum(request.args[0][pos]) && request.args[0][pos] != '-' && request.args[0][pos] != '\r')
      return _printMessage("432", this->_clients[sender_fd]->getNickName(), ":Erroneous nickname");
    pos++;
  }
  if (std::find(this->_clientNicknames.begin(), this->_clientNicknames.end(), request.args[0]) != this->_clientNicknames.end())
    return _printMessage("433", this->_clients[sender_fd]->getNickName(), request.args[0] + " :Nickname is already in use");
  
  this->_clients[sender_fd]->setNickName(request.args[0]);
  this->_clientNicknames.push_back(this->_clients[sender_fd]->getNickName());
  if(this->_clients[sender_fd]->getUserName() != "")
  {
    this->_clients[sender_fd]->setID(this->_clients[sender_fd]->getUserName() + "!" + this->_clients[sender_fd]->getNickName() + "@" + this->_clients[sender_fd]->getHost());
    this->_clients[sender_fd]->setRegistered(true);
    return _printMessage("001", this->_clients[sender_fd]->getNickName(), ":Welcome to the Internet Relay Network " + this->_clients[sender_fd]->getID());
  }
  return ("");
}

