#include "../include/Server.hpp"

std::string Server::_parsing(const std::string &message, int sender_fd)
{
  Request request(_splitRequest(message));
  
  if(request.invalidMessage)
    return _printMessage("421", this->_clients[sender_fd]->getNickName(), request.command + " :Unknown command");
  
  std::map<std::string, ICommand *>::iterator it = _commands.find(request.command);
  if (it != _commands.end())
    return it->second->execute(request, sender_fd);
  else
    return _printMessage("421", this->_clients[sender_fd]->getNickName(), request.command + " :Unknown command");
}
