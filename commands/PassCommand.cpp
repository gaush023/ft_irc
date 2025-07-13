#include "./include/PassCommand.hpp"

std::string PassCommand::execute(Request request, int sender_fd)
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

