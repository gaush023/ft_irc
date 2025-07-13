#include "./include/SetCommand.hpp"

std::string Server::_setPassword(Request request, int sender_fd)
{
  if (request.args.size() < 1)
    return _printMessage("461", this->_clients[sender_fd]->getNickName(), ":Not enough parameters");
  if(this->_clients[sender_fd]->getRegistered())
    return _printMessage("462", this->_clients[sender_fd]->getNickName(), ":Unauthorized command (already registered)");
  if(request.args[0] != this->_password)
    return _printMessage("464", this->_clients[sender_fd]->getNickName(), ":Password incorrect");
  else {
      this->_clients[sender_fd]->setAuth(true);
  }
  return ("");
}

std::string Server::_setNickName(Request request, int sender_fd)
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


std::string Server::_setUserName(Request request, int sender_fd)
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

std::string Server::_setOper(Request request, int sender_fd)
{
  if(!this->_clients[sender_fd]->getAuth())
    return _printMessage("451", this->_clients[sender_fd]->getNickName(), ":You have not registered");
  if (request.args.size() < 2)
    return _printMessage("461", this->_clients[sender_fd]->getNickName(), "PASS :Not enough parameters");
  if (request.args[0] != "ADMIN")
    return _printMessage("461", this->_clients[sender_fd]->getNickName(), ":Username or password incorrect");
  if (request.args[1] != "DEEZNUTS")
    return _printMessage("464", this->_clients[sender_fd]->getNickName(), ":Username or password incorrect");
  this->_clients[sender_fd]->setIsOperator(true);
  return _printMessage("381", this->_clients[sender_fd]->getNickName(), ":You are now an operator");
}
