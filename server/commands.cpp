#include "../headers/Server.hpp" 

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

bool Server::nickExists(const std::string& nick) const
{
    return _nickRegistry.find(nick) != _nickRegistry.end();
}

void Server::registerNick(const std::string& nick)
{
    _nickRegistry.insert(nick);
}

void Server::unregisterNick(const std::string& nick)
{
    std::set<std::string>::iterator it = _nickRegistry.find(nick);
    if (it != _nickRegistry.end())
        _nickRegistry.erase(it);
}
