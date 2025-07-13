#include "./headers/TopicCommand.hpp"

std::string TopicCommand::execute(Request request, int fd)
{
  if (!this->_clients[fd]->getRegistered())
    return _printMessage("451", this->_clients[fd]->getNickName(), ":You have not registered");
  if (request.args.size() == 0)
    return _printMessage("461", this->_clients[fd]->getNickName(), ":Not enough parameters");
  if (request.args.size() == 1)
  {
    if (this->_allChannels.find(request.args[0]) == this->_allChannels.end())
      return _printMessage("331", this->_clients[fd]->getNickName(), request.args[0] + " :No topic is set");
    else
      return _printMessage("332", this->_clients[fd]->getNickName(), request.args[0] + " :" + this->_allChannels.find(request.args[0])->second->getTopic());
  }
  std::map<std::string, Channel *>::iterator it = this->_allChannels.find(request.args[0]);
  if (it != this->_allChannels.end())
  {
    std::pair<Client *, int> user = it->second->findUserRole(fd);
    if (user.second == 1)
    {
      it->second->setTopic(request.args[1]);
      std::string reply = "TOPIC " + it->second->getName() + " :" + request.args[1] + "\n";
      _sendToAllUsers(it->second, fd, reply);
    }
    else if (user.second == -1) 
      return _printMessage("442", this->_clients[fd]->getNickName(), request.args[0] + " :You're not on that channel");
    else
      return _printMessage("482", this->_clients[fd]->getNickName(), request.args[0] + " :You're not channel operator");
  }
  return ("");
};

