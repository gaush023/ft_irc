#include "../headers/Server.hpp"

std::string Server::_parsing(std::string message, int sender_fd)
{
  Request request(_splitRequest(message)); 

  std::cout << request.command << std::endl;
  if (request.invalidMessage)
    return _printMessage("421", "", ":Unknown command");
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

std::string Server::_setPassword(Request request, int sender_fd)
{
  std::cout << "#"+this->_password+"#" << std::endl;
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

std::string Server::_setMode(Request request, int sender_fd)
{
  if (!this->_clients[sender_fd]->getRegistered())
    return _printMessage("451", this->_clients[sender_fd]->getNickName(), ":You have not registered");
  if (request.args.size() < 2)
  {
    std::string ret;
    if (request.args.size() == 1 && request.args[0] == this->_clients[sender_fd]->getNickName())
      ret = _printUserModes(ret, sender_fd);
    ret.append(toString(461) + "ERR_NEEDMOREPARAMS\n\tPASS :Not enough parameters");
    return ret;
  }
  if (request.args[0] != this->_clients[sender_fd]->getNickName())
    return _printMessage("502", this->_clients[sender_fd]->getNickName(), ":Cannot change modes for other users");
  if (!_validMode(request))
    return _printMessage("501", this->_clients[sender_fd]->getNickName(), ":Unknown mode flag");
  if (request.args[1][0] == '+')
    this->_clients[sender_fd]->setMode(true, request.args[1][1]);
  else {
    this->_clients[sender_fd]->setMode(false, request.args[2][1]);
  }
  return _printMessage("221", this->_clients[sender_fd]->getNickName(), request.args[1] + " :Mode set successfully");
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

std::string Server::_quit(Request request, int sender_fd)
{
    std::string ret = this->_clients[sender_fd]->getUserPerfix() + "QUIT ";
    if (request.args.size())
        ret.append(":" + request.args[0] + "\n");
    else
        ret.append("\n");
    std::cout << ret << std::endl;
    std::map<std::string, Channel *> channels = this->_clients[sender_fd]->getJoinedChannels();
    std::cout << "Channels size: " << channels.size() << std::endl;
    std::map<std::string, Channel *>::iterator it = channels.begin();
    std::cout << "Channels iterator: " << it->first << std::endl;
    while (it != channels.end())
    {
        _sendToAllUsers(it->second, sender_fd, ret);
        it++;
    }
    std::cout << "Leaving all channels for client: " << this->_clients[sender_fd]->getNickName() << std::endl;
    this->_clients[sender_fd]->leaveAllChannels();
    std::cout << "Removing client: " << this->_clients[sender_fd]->getNickName() << std::endl;
    close(this->_clients[sender_fd]->getClientfd());
    std::cout << "Client removed: " << this->_clients[sender_fd]->getNickName() << std::endl;
    _removeFromPoll(sender_fd);
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
