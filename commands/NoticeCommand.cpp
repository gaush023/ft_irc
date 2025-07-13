#include "./includes/NoticeCommand.hpp"

std::string Server::_notice(Request request, int sender_fd)
{
  if(!this->_clients[sender_fd]->getRegistered())
    return _printMessage("451", this->_clients[sender_fd]->getNickName(), ":You have not registered");
  if(request.args.size() < 2)
    return _printMessage("461", this->_clients[sender_fd]->getNickName(), ":Not enough parameters");
  if(request.args.size() == 2)
    _privToUser(request.args[0], request.args[1], "NOTICE", sender_fd);
  return ("");
}
