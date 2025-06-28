#include "../headers/Server.hpp"

std::string Server::_privmsg(Request request, int fd)
{
  if (!this->_clients[fd]->getRegistered())
    return (_printMessage("451", this->_clients[fd]->getNickName(), ":You have not registered"));
  if (request.args.size() < 2)
    return (_printMessage("461", this->_clients[fd]->getNickName(), ":Not enough parameters"));
  if (request.args.size() == 2)
  {
    if(request.args[0].find(",") != std::string::npos)
      return (_printMessage("401", this->_clients[fd]->getNickName(), request.args[0].append(" :Too many recipients.")));
    if (request.args[0][0] != '&' && request.args[0][0] != '#' && request.args[0][0] != '+' && request.args[0][0] != '!')
      return (_privToUser(request.args[0], request.args[1], "PRIVMSG", fd));
    return (_privToChannel(request.args[0], request.args[1], fd));  
  }
  return "";
}

std::string Server::_privToUser(std::string User, std::string msg, std::string cmd, int fd)
{
  int userFd = _findFdByNickName(User);
  if (userFd == USERNOTFOUND)
    return (_printMessage("401", this->_clients[fd]->getNickName(), User.append(" :No such nick/channel")));
  std::string reply = this->_clients[fd]->getUserPerfix();
  reply.append(cmd + " " + User + " :" + msg + "\n");
  if (_sendall(userFd, reply) == -1)
    std::cout << "_sendall() error: " << strerror(errno) << std::endl;  
  return "";
}

std::string Server::_privToChannel(std::string ChannelName, std::string masseage, int fd)
{
  std::map<std::string, Channel *>::iterator it = this->_allChannels.find(ChannelName);
  if (it != this->_allChannels.end())
  {
    std::pair<Client *, int> user = it->second->findUserRole(fd);
    if (user.second == -1)
      return (_printMessage("404", this->_clients[fd]->getNickName(), ChannelName.append(" :You're not on that channel")));
    std::string msg("PRIVMSG " + ChannelName + " :" + masseage + "\n");
    _sendAll(it->second-, msg);
  }
  else
    return (_printMessage("401", this->_clients[fd]->getNickName(), ChannelName.append(" :No such nick/channel")));    
  return "";
}

