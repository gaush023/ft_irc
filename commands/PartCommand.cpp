#include "../headers/Server.hpp"

int Server::_partChannel(std::string ChannelName, int i, std::string message, int isPart)
{
  std::map<std::string, Channel *>::iterator itCh = this->_allChannels.find(ChannelName);
  if (itCh == this->_allChannels.end())
    return (NOSUCHCHANNEL);
  else {
    std::pair<Client *, int> user(itCh->second->findUserRole(i));
    if (user.second == -1)
      return (NOTINCHANNEL);
    else {
      if (user.second == 0)
        itCh->second->removeUser(i);
      else if (user.second == 1)
        itCh->second->removeOperator(i);
      else
        itCh->second->removeVoice(i);
      user.first->leaveChannel(itCh->second->getName());
      if (isPart == 1) {
        std::string reply = "PART " + ChannelName;
        if (message.empty())
          reply.append("\n");
        else
          reply.append(" " + message + "\n");
        _sendToAllUsers(itCh->second, i, reply);
      }
    }
  }
  return (0);
}

std::string Server::_part( Request request, int fd)
{
  if (!this->_clients[fd]->getRegistered())
    return _printMessage("451", this->_clients[fd]->getNickName(), ":You have not registered");
  if (request.args.size() == 0)
    return _printMessage("461", this->_clients[fd]->getNickName(), ":Not enough parameters");
  std::vector<std::string> parsChannels(_commaSeparator(request.args[0]));
  std::vector<std::string>::iterator it = parsChannels.begin();
  while (it != parsChannels.end())
  {
    int status = 0;
    if (request.args.size() == 2)
      status = _partChannel(*it, fd, request.args[1], 1);
    else
      status = _partChannel(*it, fd, "", 1);
    if (status == NOSUCHCHANNEL) 
      return _printMessage("403", this->_clients[fd]->getNickName(), *it + " :No such channel");
    if (status == NOTINCHANNEL)
      return _printMessage("442", this->_clients[fd]->getNickName(), *it + " :You're not on that channel");
    it++;
  }
  return ("");
};
