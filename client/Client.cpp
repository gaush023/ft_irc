#include "../headers/Client.hpp"

Client::Client(): _clientfd(0), _Auth(false), _Registered(false), _isOperator(false), _NickName(), _UserName(), _RealName(), _Host("deez.nuts"), _ID(), _remoteAddr(), _addrlen(),_modes(), _joinedChannels{};
Client::Client(int fd): _clientfd(fd), _Auth(false), _Registered(false), _isOperator(false), _NickName(), _UserName(), _RealName(), _Host("deez.nuts"), _ID(), _remoteAddr(), _addrlen(),_modes(), _joinedChannels{};
Client::Client(const Client& x): _Host(x._Host) { *this = x; };

Client &Client::operator=( const Client& rhs)
{
  if(this == &rhs)
    return *this;
  
  this->_clientfd = rhs._clientfd;
  this->_Registered = rhs._Registered;
  this->_isOperator = rhs._isOperator;
  this->_NickName = rhs._NickName;
  this->_UserName = rhs._UserName;
  this->_FullName = rhs._FullName;
  this->_Auth = rhs._Auth;
  this->_remoteaddr = rhs._remoteAddr;
  this->_addrlen = rhs._addrlen;
  this->_joinedChannels.insert(rhs._joinedChannels.begin(), rhs._joinedChannels.end());
  return *this;
}

Client::~Client() {};

std::string Client::getUserName() const {return this->_UserName; };
std::string Client::getNickName() const {return this->_NickName; };
std::string Client::getFullName() const {return this->_FullName; };
std::string Client::getHost() const { return this->_Host; };
std::string Client::getID() const { return this->_ID; };
bool Client::getAuth() const { return this->_Auth; };
int Client::getRegistered() const { return this->_Registered; };
int Client::getisOperator() const { return this->_isOperator; };

int Client::getMode(char mode) const
{
  if (mode == 'a')
    return this->_modes.away;
  else if (mode == 'i')
    return this->_modes.invisible;
  else if (mode == 'w')
    return this->_modes.wallops;
  else if (mode == 'r')
    return this->_modes.restricted;
  else if (mode == 'o')
    return this->_modes.op;
  else if (mode == 'O')
    return this->_modes.localOp;
  else if (mode == 's')
    return this->_modes.server;
  return 0;
}

void Client::setUserName(std::string UserName) { this->_UserName = UserName; };
void Client::setNickName(std::string NickName) { this->_NickName = NickName; };
void Client::setFullName(std::string FullName) { this->_FullName = FullName; };
void Client::setID(std::string ID) { this->_ID = ID; };
void Client::setClientfd(int fd) { this->_clientfd = fd; };
void Client::setRegistered(int Registered) { this->_Registered = Registered; };
void Client::setAuth(int Auth) { this->_Auth = Auth; };

void Client::setIsOperator(int isOperator) 
{ 
  this->_isOperator = isOperator;
  this->_modes.op = isOperator;
  this->_modes.localOp = isOperator;
};

void Client::setMode(int value, char mode)
{
  if (mode == 'i')
    this->_modes.invisible = value;
  else if (mode == 'w')
    this->_modes.wallops = value;
  else if (mode == 'r' && value == 1)
    this->_modes.restricted = value;
  else if (mode == 'o' && value == 0)
    this->modes.op = value;
  else if (mode == 'O' && value == 0)
    this->_modes.localOp = value;
else if (mode == 's')
    this->_modes.server = value;
}

int Client::isjoined(std::string ChannelName) const
{
  if (this->_joinedChannels.find(ChannelName) != this->_joinedChannels.end())
    return 1;
  return 0;
};

void Client::joinChannel(std::string ChannelName, Channel *channel)
{
  if (!isjoined(ChannelName))
    this->_joinedChannels.insert(std::pair<std::string, Channel *>(ChannelName, channel));
};

void Client::leaveChannel(std::string ChannelName)
{
  this->_joinedChannels.erase(ChannelName);
}

std::string Client::leaveAllChannels()
{ 
std::map<std::string, Channel *>::iterator it = this->_joinedChannels.begin();
  while ( it != this->joinedChannels.end())
  {
    std::pair<Client *, int> user(it->second->findUserRole(this->_clientfd)); 
  }
    if(user.second == 0)
    {
      it->second->removeMember(this->_clientfd);
    } 
    else if (user.second == 1)
    {
      it->second->removeOperator(this->_clientfd);
    }
    else
    {
      it->second->removeVoice(this->_clientfd);
    }
    return ("");
}

std::string	Client::getAllChannels() const
{
	std::string channels(YELLOW "███████████████████████████████████████████████████████████████████████████████████████\n");
	channels.append("█              █              █                    █                                  █\n");
	channels.append("█" RESET " Channel Name " YELLOW "█ " RESET "Online Users " YELLOW "█ " RESET "Creator Of Channel " YELLOW "█ " RESET "          Channel Topic          " YELLOW "█\n");
	channels.append("█              █              █                    █                                  █\n");
	channels.append("███████████████████████████████████████████████████████████████████████████████████████\n");
	std::map<std::string, Channel *>::const_iterator it = this->_joinedChannels.begin();
	while (it != this->_joinedChannels.end())
	{
		channels.append("█              █              █                    █                                  █\n");
		channels.append("█ " RESET + fillIt(it->first, 12));
		channels.append(YELLOW " █      " RESET + fillIt(std::to_string(it->second->getOnlineUsers()), 7));
		channels.append(YELLOW " █ " RESET + fillIt(it->second->getCreator()->getFullName(), 18));
		channels.append(YELLOW " █ " RESET + fillIt(it->second->getTopic(), 32));
		channels.append(YELLOW " █\n");
		channels.append(YELLOW "█              █              █                    █                                  █\n");
		channels.append("███████████████████████████████████████████████████████████████████████████████████████\n");
		it++;
	};
	if (this->_joinedChannels.size() == 0)
	{
		channels.append("█                                                                                     █\n");
		channels.append("█                                " RESET "YOU JOINED NO CHANNEL" YELLOW "                                █\n");
		channels.append("█                                                                                     █\n");
		channels.append("███████████████████████████████████████████████████████████████████████████████████████\n");
	}
	channels.append(RESET "\n\n");
	return (channels);
};

std::string Client::getUserPrefix() const
{
  return ":" + this->_NickName + "!" + this->_UserName + "@" + this->_Host + " ";
}

std::map<std::string, Channel *> Client::getJoinedChannels() const
{
  return this->_joinedChannels;
}
