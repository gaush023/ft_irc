#include "../headers/Server.hpp"

std::string Server::_parsing(std::string message, int sender_fd)
{
   Request request(_splitRequest(message)); 


  if (request.invalidMessage)
    return ("Invalid message format\n");
  else if (request.command == "PASS")
    return (_setPassword(request, sender_fd));
  else if (request.command == "NICK")
    return (_setNickName(request, sender_fd));
  else if (request.command == "USER")
    return (_setUserName(request, sender_fd));
  else if (request.command == "OPER")
    return (_setOper(request, sender_fd));
  else if (request.command == "MODE")
  {
    if (request.args.size() > 0 && (request.args[0][0] == '#' || request.args[0][0] == '&'))
      return (_channelMode(request, sender_fd));
    else
      return (_setMode(request, sender_fd));
  }
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
  else if (request.command == "INVITE")
    return (_invite(request, sender_fd));
  else
    return ("Invalid command\n");
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
    if (user.second == -1) 
      return _printMessage("442", this->_clients[fd]->getNickName(), request.args[0] + " :You're not on that channel");
    
    if (it->second->getTopicRestricted() && user.second != 1)
      return _printMessage("482", this->_clients[fd]->getNickName(), request.args[0] + " :You're not channel operator");
    
    if (user.second == 1 || !it->second->getTopicRestricted())
    {
      it->second->setTopic(request.args[1]);
      std::string reply = "TOPIC " + it->second->getName() + " :" + request.args[1] + "\n";
      _sendToAllUsers(it->second, fd, reply);
    }
  }
  return ("");
};

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

bool Server::_validMode(Request request)
{
  char c = request.args[1][1];
  if (request.args[1].length() != 2 || (request.args[1][0] != '+' && request.args[1][0] != '-'))
    return false;
  if (c != 'i' && c != 'w' && c != 'r' && c != 'o' && c != 'O' && c != 's')
    return false;
  return true;
}

std::string Server::_printUserModes(std::string ret, int sender_fd)
{
  ret.append("Current modes for " + toString(this->_clients[sender_fd]->getMode('i')));
  ret.append("\ni: " + toString(this->_clients[sender_fd]->getMode('i')));
  ret.append("\nw: " + toString(this->_clients[sender_fd]->getMode('w')));
  ret.append("\nr: " + toString(this->_clients[sender_fd]->getMode('r')));
  ret.append("\no: " + toString(this->_clients[sender_fd]->getMode('o')));
  ret.append("\nO: " + toString(this->_clients[sender_fd]->getMode('O')));
  ret.append("\ns: " + toString(this->_clients[sender_fd]->getMode('s')));
  ret.append("\n");
  return ret;
}

std::string Server::_setMode(Request& request, int sender_fd) {
    const std::string& nick = this->_clients[sender_fd]->getNickName();

    if (!this->_clients[sender_fd]->getRegistered())
        return _printMessage("451", nick, ":You have not registered");

    if (request.args.size() == 1 && request.args[0] == nick)
        return _printUserModes("", sender_fd);

    if (request.args.size() < 2)
        return _printMessage("461", nick, "MODE :Not enough parameters");

    if (request.args[0] != nick)
        return _printMessage("502", nick, ":Cannot change modes for other users");

    if (!_validMode(request))
        return _printMessage("501", nick, ":Unknown mode flag");

    char flag = request.args[1][1];
    bool add  = (request.args[1][0] == '+');
    this->_clients[sender_fd]->setMode(add, flag);

    return _printMessage("221", nick, request.args[1]);
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

std::string Server::_invite(Request request, int sender_fd)
{
  if (!this->_clients[sender_fd]->getRegistered())
    return _printMessage("451", this->_clients[sender_fd]->getNickName(), ":You have not registered");
  if (request.args.size() < 2)
    return _printMessage("461", this->_clients[sender_fd]->getNickName(), ":Not enough parameters");

  std::string nickname = request.args[0];
  std::string channelName = request.args[1];

  int targetFd = _findFdByNickName(nickname);
  if (targetFd == USERNOTFOUND)
    return _printMessage("401", this->_clients[sender_fd]->getNickName(), nickname + " :No such nick/channel");

  std::map<std::string, Channel *>::iterator it = this->_allChannels.find(channelName);
  if (it == this->_allChannels.end())
    return _printMessage("403", this->_clients[sender_fd]->getNickName(), channelName + " :No such channel");

  std::pair<Client *, int> user = it->second->findUserRole(sender_fd);
  if (user.second == -1)
    return _printMessage("442", this->_clients[sender_fd]->getNickName(), channelName + " :You're not on that channel");

  if (user.second != 1)
    return _printMessage("482", this->_clients[sender_fd]->getNickName(), channelName + " :You're not channel operator");

  std::pair<Client *, int> targetUser = it->second->findUserRole(targetFd);
  if (targetUser.second != -1)
    return _printMessage("443", this->_clients[sender_fd]->getNickName(), nickname + " " + channelName + " :is already on channel");

  it->second->addInvitedUser(nickname);

  std::string inviteMsg = this->_clients[sender_fd]->getUserPerfix() + "INVITE " + nickname + " " + channelName + "\n";
  if (_sendall(targetFd, inviteMsg) == -1)
    std::cout << "_sendall() error: " << strerror(errno) << std::endl;

  return _printMessage("341", this->_clients[sender_fd]->getNickName(), nickname + " " + channelName);
}

std::string Server::_channelMode(Request request, int sender_fd)
{
  if (!this->_clients[sender_fd]->getRegistered())
    return _printMessage("451", this->_clients[sender_fd]->getNickName(), ":You have not registered");
  if (request.args.size() < 1)
    return _printMessage("461", this->_clients[sender_fd]->getNickName(), ":Not enough parameters");

  std::string channelName = request.args[0];
  std::map<std::string, Channel *>::iterator it = this->_allChannels.find(channelName);
  if (it == this->_allChannels.end())
    return _printMessage("403", this->_clients[sender_fd]->getNickName(), channelName + " :No such channel");

  std::pair<Client *, int> user = it->second->findUserRole(sender_fd);
  if (user.second == -1)
    return _printMessage("442", this->_clients[sender_fd]->getNickName(), channelName + " :You're not on that channel");

  if (request.args.size() == 1)
  {
    std::string modes = "+";
    if (it->second->getInviteOnly()) modes += "i";
    if (it->second->getTopicRestricted()) modes += "t";
    if (!it->second->getKey().empty()) modes += "k";
    if (it->second->getUserLimit() > 0) modes += "l";
    return _printMessage("324", this->_clients[sender_fd]->getNickName(), channelName + " " + modes);
  }

  if (user.second != 1)
    return _printMessage("482", this->_clients[sender_fd]->getNickName(), channelName + " :You're not channel operator");

  std::string modeString = request.args[1];
  bool add = true;
  std::string response = "";

  for (size_t i = 0; i < modeString.length(); i++)
  {
    char mode = modeString[i];
    if (mode == '+')
      add = true;
    else if (mode == '-')
      add = false;
    else if (mode == 'i')
    {
      it->second->setInviteOnly(add);
      response += (add ? "+" : "-") + std::string(1, mode);
    }
    else if (mode == 't')
    {
      it->second->setTopicRestricted(add);
      response += (add ? "+" : "-") + std::string(1, mode);
    }
    else if (mode == 'k')
    {
      if (add && request.args.size() > 2)
      {
        it->second->setKey(request.args[2]);
        response += "+" + std::string(1, mode) + " " + request.args[2];
      }
      else if (!add)
      {
        it->second->setKey("");
        response += "-" + std::string(1, mode);
      }
    }
    else if (mode == 'l')
    {
      if (add && request.args.size() > 2)
      {
        int limit = atoi(request.args[2].c_str());
        if (limit > 0)
        {
          it->second->setUserLimit(limit);
          response += "+" + std::string(1, mode) + " " + request.args[2];
        }
      }
      else if (!add)
      {
        it->second->setUserLimit(0);
        response += "-" + std::string(1, mode);
      }
    }
    else if (mode == 'o')
    {
      if (request.args.size() > 2)
      {
        int targetFd = _findFdByNickName(request.args[2]);
        if (targetFd != USERNOTFOUND)
        {
          std::pair<Client *, int> targetUser = it->second->findUserRole(targetFd);
          if (targetUser.second != -1)
          {
            if (add && targetUser.second != 1)
            {
              it->second->removeUser(targetFd);
              it->second->addOperator(targetUser.first);
              response += "+" + std::string(1, mode) + " " + request.args[2];
            }
            else if (!add && targetUser.second == 1)
            {
              it->second->removeOperator(targetFd);
              it->second->addUser(targetUser.first);
              response += "-" + std::string(1, mode) + " " + request.args[2];
            }
          }
        }
      }
    }
  }

  if (!response.empty())
  {
    std::string modeMsg = this->_clients[sender_fd]->getUserPerfix() + "MODE " + channelName + " " + response + "\n";
    std::map<int, Client *> allUsers = it->second->getAllUsers();
    for (std::map<int, Client *>::iterator userIt = allUsers.begin(); userIt != allUsers.end(); ++userIt)
    {
      if (_sendall(userIt->first, modeMsg) == -1)
        std::cout << "_sendall() error: " << strerror(errno) << std::endl;
    }
  }

  return "";
}

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
