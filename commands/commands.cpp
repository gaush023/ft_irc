#include "../headers/Server.hpp"

std::string Server::_parsing(std::string message, int sender_fd)
{
   Request request(_splitRequest(message)); 


  if (request.invalidMessage)
    return (_printMessage("421", this->_clients[sender_fd]->getNickName(), request.command + " :Unknown command"));
  else if (request.command == "PASS")
    return (_setPassword(request, sender_fd));
  else if (request.command == "NICK")
    return (_setNickName(request, sender_fd));
  else if (request.command == "USER")
    return (_setUserName(request, sender_fd));
  else if (request.command == "OPER")
    return (_setOper(request, sender_fd));
  else if (request.command == "MODE")
    return (_setMode(request, sender_fd));
  else if (request.command == "PRIVMSG")
    return (_privmsg(request, sender_fd));
  else if (request.command == "NOTICE")
    return (_notice(request, sender_fd));
  else if (request.command == "HELP")
    return (_printHelpInfo());
  else if (request.command == "JOIN")
    return (_joinChannel(request, sender_fd));
  else if (request.command == "TOPIC")
    return (_topic(request, sender_fd));
  else if (request.command == "KICK")
    return (_kick(request, sender_fd));
  else if (request.command == "PART")
    return (_part(request, sender_fd));
  else if (request.command == "QUIT")
    return (_quit(request, sender_fd));
  else if (request.command == "SENDFILE")
    return (_sendFile(request, sender_fd));
  else if (request.command == "GETFILE")
    return (_getFile(request, sender_fd));
  else if (request.command == "HELPDESK")
    return (_helpDesk(request, sender_fd));
  else if (request.command == "PING")
    return (_ping(request, sender_fd));
  else if (request.command == "INVITE")
    return (_invite(request, sender_fd));
  else 
    return (_printMessage("421", this->_clients[sender_fd]->getNickName(), request.command + " :Unknown command"));
};

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
 

int Server::_findFdByNickName(std::string nickName)
{
  std::map<int, Client *>::iterator it = this->_clients.begin();
  while (it != this->_clients.end())
  {
    if (it->second->getNickName() == nickName)
      return it->second->getClientfd();
    it++;
  }
  return USERNOTFOUND;
}


std::string Server::_topic(Request request, int fd)
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




std::string Server::_quit(Request request, int sender_fd)
{
    std::string ret = this->_clients[sender_fd]->getUserPerfix() + "QUIT ";
    if (request.args.size())
        ret.append(":" + request.args[0] + "\n");
    else
        ret.append("\n");
    std::map<std::string, Channel *> channels = this->_clients[sender_fd]->getJoinedChannels();
    std::map<std::string, Channel *>::iterator it = channels.begin();
    while (it != channels.end())
    {
        _sendToAllUsers(it->second, sender_fd, ret);
        it++;
    }
    this->_clients[sender_fd]->leaveAllChannels();
    close(this->_clients[sender_fd]->getClientfd());
    return ("QUIT");
};

std::string Server::_printHelpInfo()
{
  std::string helpInfo;
  
  helpInfo.append(GREEN);
  helpInfo.append("STEP 1: PASS\n");
  helpInfo.append(RESET);
  helpInfo.append("\tUse PASS command to set a password. e.g: PASS [Server Password]\n\n");
  helpInfo.append(GREEN);
  helpInfo.append("STEP 2: NICK\n");
  helpInfo.append(RESET);
  helpInfo.append("\tUse NICK command to set a nickname. e.g: NICK deezNuts69\n\n");
  helpInfo.append(GREEN);
  helpInfo.append("STEP 3: USER\n");
  helpInfo.append(RESET);
  helpInfo.append("\tUse USER command to set a username. e.g: USER [Username] 0 * :[Full Name]\n\n");
  return (helpInfo);
}
