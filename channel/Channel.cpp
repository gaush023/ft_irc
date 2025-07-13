#include "../headers/Channel.hpp"

Channel::Channel() : _prefix(), _creator(), _onlineUsers(), _name(), _key(), _topic(), _members(), _bannedUsers(), _operators(), _voices(), _invitedUsers(), _inviteOnly(false), _topicRestricted(false), _userLimit(0) {};

Channel::Channel( const Channel &x){ *this = x; };

Channel::Channel( std::string channelName, Client *Creator) : _prefix(), _creator(Creator), _onlineUsers(1), _name(channelName), _key(), _topic(), _members(), _bannedUsers(), _operators(), _voices(), _invitedUsers(), _inviteOnly(false), _topicRestricted(false), _userLimit(0) 
{
  this->_operators.insert(std::pair<int, Client *>(Creator->getClientfd(), Creator));
}

Channel::Channel( std::string channelName, std::string key, Client *Creator) : _prefix(), _creator(Creator), _onlineUsers(1), _name(channelName), _key(key), _topic(), _members(), _bannedUsers(), _operators(), _voices(), _invitedUsers(), _inviteOnly(false), _topicRestricted(false), _userLimit(0) 
{
  this->_operators.insert(std::pair<int, Client *>(Creator->getClientfd(), Creator));
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
std::map<int, Client *> const &Channel::getMembers() const { return this->_members; };
std::map<int, Client *> const &Channel::getOperators() const { return this->_operators; };
std::map<int, Client *> const &Channel::getVoice() const { return this->_voices; };

Client *Channel::getCreator() const { return this->_creator; };

void Channel::setPrefix(char prefix) { this->_prefix = prefix; };
void Channel::setOnlineUsers(int onlineUsers) { this->_onlineUsers = onlineUsers; };
void Channel::setName(std::string name) { this->_name = name; };
void Channel::setKey(std::string key) { this->_key = key; };
void Channel::setTopic(std::string topic) { this->_topic = topic; };
void Channel::setInviteOnly(bool inviteOnly) { this->_inviteOnly = inviteOnly; };
void Channel::setTopicRestricted(bool topicRestricted) { this->_topicRestricted = topicRestricted; };
void Channel::setUserLimit(int userLimit) { this->_userLimit = userLimit; };

bool Channel::getInviteOnly() const { return this->_inviteOnly; };
bool Channel::getTopicRestricted() const { return this->_topicRestricted; };
std::vector<std::string> const &Channel::getInvitedUsers() const { return this->_invitedUsers; };
std::vector<std::string> const &Channel::getBannedUsers() const { return this->_bannedUsers; };
int Channel::getUserLimit() const { return this->_userLimit; };

void Channel::addInvitedUser(std::string nickname) {
  if (std::find(this->_invitedUsers.begin(), this->_invitedUsers.end(), nickname) == this->_invitedUsers.end()) {
    this->_invitedUsers.push_back(nickname);
  }
};

void Channel::removeInvitedUser(std::string nickname) {
  std::vector<std::string>::iterator it = std::find(this->_invitedUsers.begin(), this->_invitedUsers.end(), nickname);
  if (it != this->_invitedUsers.end()) {
    this->_invitedUsers.erase(it);
  }
};

bool Channel::isInvited(std::string nickname) const {
  return std::find(this->_invitedUsers.begin(), this->_invitedUsers.end(), nickname) != this->_invitedUsers.end();
};

int Channel::addUser(Client *member)
{
  if (std::find(this->_bannedUsers.begin(), this->_bannedUsers.end(), member->getNickName()) != this->_bannedUsers.end())
    return BANNEDFROMCHAN;
  if (this->_inviteOnly && !this->isInvited(member->getNickName()))
    return USERNOTFOUND;
  if (this->_userLimit > 0 && this->_onlineUsers >= this->_userLimit)
    return CHANNELISFULL;
  if (this->_members.find(member->getClientfd()) == this->_members.end())
  {
    this->_members.insert(std::pair<int, Client *>(member->getClientfd(), member));
    this->_onlineUsers++;
    if (this->_inviteOnly)
      this->removeInvitedUser(member->getNickName());
    return USERJOINED;
  }
  return USERALREADYJOINED;
}


int Channel::addOperator(Client *member)
{
  if (std::find(this->_bannedUsers.begin(), this->_bannedUsers.end(), member->getNickName()) != this->_bannedUsers.end())
    return BANNEDFROMCHAN;
  if (this->_inviteOnly && !this->isInvited(member->getNickName()))
    return USERNOTFOUND;
  if (this->_userLimit > 0 && this->_onlineUsers >= this->_userLimit)
    return CHANNELISFULL;
  if (this->_operators.find(member->getClientfd()) == this->_operators.end())
  {
    this->_operators.insert(std::pair<int, Client *>(member->getClientfd(), member));
    this->_onlineUsers++;
    if (this->_inviteOnly)
      this->removeInvitedUser(member->getNickName());
    return USERJOINED;
  }
  return USERALREADYJOINED;
}

int Channel::banUser(Client *member)
{
  if (std::find(this->_bannedUsers.begin(), this->_bannedUsers.end(), member->getNickName()) != this->_bannedUsers.end())
    return USERALREADYBANNED;
  this->_bannedUsers.push_back(member->getNickName());
  return USERISBANNED;
}

void Channel::removeOperator(int i)
{
  this->_operators.erase(i);
  this->_onlineUsers--;
}

void Channel::removeVoice(int i)
{
  this->_voices.erase(i);
  this->_onlineUsers--;
}

void Channel::removeBannedUser(std::string nickname)
{
  std::vector<std::string>::iterator it = std::find(this->_bannedUsers.begin(), this->_bannedUsers.end(), nickname);
  if (it != this->_bannedUsers.end())
    this->_bannedUsers.erase(it);
}

void Channel::removeUser(int i)
{
  this->_members.erase(i);
  this->_onlineUsers--;
}


std::map<int, Client *> Channel::getAllUsers() const
{
  std::map<int, Client *> allUsers = this->_members;
  allUsers.insert(this->_operators.begin(), this->_operators.end());
  allUsers.insert(this->_voices.begin(), this->_voices.end());
  return allUsers;
};


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
};

    
std::string Channel::listUsers() const
{
  std::string AllUsers(":");
  std::map<int, Client *>::const_iterator it = this->_operators.begin();
  while (it != this->_operators.end())
  {
    AllUsers.append("@" + it->second->getNickName() + " ");
    ++it;
  }
  it = this->_members.begin();
  while (it != this->_members.end())
  {
    AllUsers.append(it->second->getNickName() + " ");
    ++it;
  }
  it = this->_voices.begin();
  while (it != this->_voices.end())
  {
    AllUsers.append("+" + it->second->getNickName() + " ");
    ++it;
  }
  return AllUsers;
};
