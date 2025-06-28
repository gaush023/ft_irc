#include "../headers/Channel.hpp"

Channel::Channel() : _prefix(), _creator(), _onlineUsers(), _name(), _key(), _topic(), _members(), _bannedUsers(), _operators(), _voices() {};

Channel::Channel( const Channel &x){ *this = x; };

Channel::Channel( std::string channellName, std::string key, Client *Creator) : _prefix(), _creator(creator), _onlineUsers(1), _name(channellName), _key(), _topic(), _members(), _bannedUsers(), _operators(), _voices() 
{
  this->_operators.insert(std::pair<int, Client *>(creator->getClientfd(), creator));
}

Channel::Channel( std::string channellName, std::string key, Client *creator) : _prefix(), _creator(Creator), _onlineUsers(1), _name(channellName), _key(channelkey), _topic(), _members(), _bannedUsers(), _operators(), _voices() 
{
  this->_operators.insert(std::pair<int, Client *>(creator->getClientfd(), creator));
};

Channel::~Channel() {};

Channel &Channel::operator=( const Channel &rhs)
{
  if(this == &rhs)
    return *this;
  
  this->_prefix = rhs._prefix;
  this->_onlineUsers = rhs._onlineUsers;
  this->_name = rhs._name;
  this->_members.insert(rhs._members.begin(), rhs._members.end());
  this->_operators.insert(rhs._operators.begin(), rhs._operators.end());
  this->_voices.insert(rhs._voices.begin(), rhs._voices.end());
  
  return *this;
};

char const &Channel::getPrefix() const { return this->_prefix; };
int const &Channel::getOnlineUsers() const { return this->_onlineUsers; };
std::string const &Channel::getName() const { return this->_name; };
std::string const &Channel::getKey() const { return this->_key; };
std::string const &Channel::getTopic() const { return this->_topic; };
std::map<int, Client *> &Channel::getMembers() const { return this->_members; };
std::map<int, Client *> &Channel::getOperators() const { return this->_operators; };
std::map<int, Client *> &Channel::getVoices() const { return this->_voices; };

Client *Channel::getCreator() const { return this->_creator; };

void Channel::setPrefix(char prefix) { this->_prefix = prefix; };
void Channel::setOnlineUsers(int onlineUsers) { this->_onlineUsers = onlineUsers; };
void Channel::setName(std::string name) { this->_name = name; };
void Channel::setKey(std::string key) { this->_key = key; };
void Channel::setTopic(std::string topic) { this->_topic = topic; };

std::pair<Client *, int> Channel::findUserRole(int fd) const
{
  std::map<int, Client *>::const_iterator it = this->_members.find(fd);
  if (it != this->_members.end())
    return std::make_pair(it->second, 0);
  it = this->_operators.find(fd);
  if (it != this->_operators.end())
    return std::make_pair(it->second, 1);
  it = this->_voices.find(fd);
  if (it != this->_voices.end())
    return std::make_pair(it->second, 2);
  return std::pair<Client *, int>(NULL, -1);
}

    
