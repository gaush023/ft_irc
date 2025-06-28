#include "../headers/Server.hpp"

std::string Server::_joinChannel( Request request, int fd)
{ 
  int status = 1;
  if (!this->_clients[fd]->getRegistered())
    return (_printMessage("451", this->_clients[fd]->getNickName(), ":You have not registered"));
  if (request.args.size() == 0)
    return (_printMessage("461", this->_clients[fd]->getNickName(), ":Not enough parameters"));
  if (request.args[0] == "0")
    return (this->_clients[fd]->leaveAllChannels());
  std::vector<std::string> parsChannels(_commaSeparator(request.args[0]));
  std::vector<std::string> parsKeys;
  if (request.args.size() == 2)
    parsKeys = _commaSeparator(request.args[1]);
  std::vector<std::string> parsChannels(_commaSeparator(request.args[0]));
  std::vector<std::string> parsKeys;
  if (request.args.size() == 2)
    parsKeys = _commaSeparator(request.args[1]);
  std::vector<std::string>::iterator itChannels = parsChannels.begin();
  std::vector<std::string>::iterator itKeys = parsKeys.begin();
  while ( itChannels != parsChannels.end() && status == 1)
  {
    if (itKeys != parsKeys.end())
      status = _createPrvChannel(*itChannels, *itKeys, fd);
    else
      status = _createChannel(*itChannels, fd);
  }
 

int Server::_createChannel( std::string ChannelName, int CreatorFd)
{
  std::map<std::string, Channel *>::iterator it = this->_allChannels.find(ChannelName);
  if ( it == this->_allChannel.end())
  {
    if (ChannelName[0] != '&' && ChannelName[0] != '#' && ChannelName[0] != '+' && ChannelName[0] != '!')
      return (BADCHANMASK);
    this->_allChannels.insert(std::pair<std::string, Channel *>(ChannelName, channel));
    this->_clients[CreatorFd]->joinChannel(ChannelName, channel);
  }
  else {
      if (it->second->getKey().empty())
      {
        status = 0;
        if (this->_clients[CreatorFd]->getisOperator() == true)
          status = it->second->addOperator(this->_clients[CreatorFd]);
        else
          status = it->second->addMember(this->_clients[CreatorFd]);
        if (status == USERISJOINED)
          this->_clients[CreatorFd]->joinChannel(it->first, it->second);
        else if (status == USERALREADYJOINED)
          return (USERALREADYJOINED);
        else if (status == BANNEDFROMCHAN)
          return (BANNEDFROMCHAN);
        _sendall(CreatorFd, this->_clients[CreatorFd]->getUserPerfix() + "JOIN " + ChannelName + "\n");
        _sendall(CreatorFd, _printMessage("332", this->_clients[CreatorFd]->getNickName(), ChannelName + " :" + it->second->getTopic()));
        _sendall(CreatorFd, _printMessage("353", this->_clients[CreatorFd]->getNickName() + " = " + ChannelName, it->second->listAllUsers()));
        _sendall(CreatorFd, _printMessage("353", this->_clients[CreatorFd]->getNickName() + " " + ChannelName, ":End of NAMES list"));
        std::string reply = "JOIN " + ChannelName + "\n";
        _sendToAllUsers(it->second, CreatorFd, reply);
        return (USERISJOINED);
      }
    }
    return (USERISJOINED);
}

int Server::_createPrvChannel(std::string ChannelName, std::string ChannelKey, int CreatorFd)
{
  std::map<std::string, Channel *>::iterator it = this->_allChannels.find(ChannelName);
  if ( it == this->_allChannels.end())
  {
    if (ChannelName[0] != '&' && ChannelName[0] != '#' && ChannelName[0] != '+' && ChannelName[0] != '!')
      return (BADCHANMASK);
    Channel *channel = new Channel(ChannelName, ChannelKey, this->_clients[CreatorFd]);
    this->_allChannels.insert(std::pair<std::string, Channel *>(ChannelName, channel));
    this->_clients[CreatorFd]->joinChannel(ChannelName, channel);
  }
  else {
      if (it->second->getKey() == ChannelKey)
      {
        status = 0;
        if (this->_clients[CreatorFd]->getisOperator() == true)
          status = it->second->addOperator(this->_clients[CreatorFd]);
        else
          status = it->second->addMember(this->_clients[CreatorFd]);
        if (status == USERISJOINED)
          this->_clients[CreatorFd]->joinChannel(it->first, it->second);
        else if (status == USERALREADYJOINED)
          return (USERALREADYJOINED);
        else if (status == BANNEDFROMCHAN)
          return (BANNEDFROMCHAN);
        _sendall(CreatorFd, this->_clients[CreatorFd]->getUserPerfix() + "JOIN " + ChannelName + "\n");
        _sendall(CreatorFd, _printMessage("332", this->_clients[CreatorFd]->getNickName(), ChannelName + " :" + it->second->getTopic()));
        _sendall(CreatorFd, _printMessage("353", this->_clients[CreatorFd]->getNickName() + " = " + ChannelName, it->second->listAllUsers()));
        _sendall(CreatorFd, _printMessage("353", this->_clients[CreatorFd]->getNickName() + " " + ChannelName, ":End of NAMES list"));
        _sendToAllUsers(it->second, CreatorFd, "JOIN " + ChannelName + "\n");
        return (USERISJOINED);
      }
      else {
        return (BADCHANKEY);
      }
    }
    return (USERISJOINED);
  };

std::vector<std::string> Server::_commaSeparator(std::string arg)
{
  std::vector<std::string> ret;
  size_t pos = 0;
  while ((pos = arg.find(",")) != std::string::npos)
  {
    ret.push_back(arg.substr(0, pos));
    arg.erase(0, pos+ 1);
  }
  ret.push_back(arg.substr(0, pos));
  return ret;
}
