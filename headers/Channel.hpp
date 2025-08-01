#pragma once

#include "Server.hpp"

#define USERALREADYJOINED 0
#define USERJOINED 1
#define NOTINCHANNEL 2
#define BANNEDFROMCHAN 3
#define TOOMANYCHANNELS 4
#define BADCHANNELKEY 5
#define CHANNELISFULL 6
#define NOSUCHCHANNEL 7
#define USERISBANNED 8
#define BADCHANMASK 9
#define USERNOTINCHANNEL -1
#define USERNOTFOUND -1
#define USERALREADYBANNED -1
#define USERISALREADYJOINED -1

class Channel
{
  private:
    char _prefix;
    Client *_creator;
    int _onlineUsers;
    std::string _name;
    std::string _key;
    std::string _topic;
    std::map<int, Client *> _members;
    std::vector<std::string> _bannedUsers;
    std::map<int, Client *> _operators;
    std::map<int, Client *> _voices;
    std::vector<std::string> _invitedUsers;
    bool _inviteOnly;
    bool _topicRestricted;
    int _userLimit;

  public:
    Channel();

  public:
    Channel(std::string channelName, Client *Creator);
    Channel(std::string channelName, std::string channelKey, Client *Creator);
    Channel(const Channel &x);
    Channel &operator=(const Channel &rhs);
    ~Channel();
  
  public:
    char const &getPrefix() const;
    int const &getOnlineUsers() const;
    std::string const &getName() const;
    std::string const &getKey() const;
    std::string const &getTopic() const;
    std::map<int, Client *> const &getMembers() const;
    std::map<int, Client *> const &getOperators() const;
    std::map<int, Client *> const &getVoice() const;
    std::vector<std::string> const &getBannedUsers() const;
    bool getInviteOnly() const;
    bool getTopicRestricted() const;
    std::vector<std::string> const &getInvitedUsers() const;
    int getUserLimit() const;
  
    Client *getCreator() const;
    std::map<int, Client *> getAllUsers() const;
    std::pair<Client *, int> findUserRole(int fd) const;
    std::string listUsers() const;

  public:
    int addUser(Client *member);
    int addOperator(Client *member);
    void addVoice(Client *member);
    int banUser(Client *member);
    void removeOperator(int i);
    void removeVoice(int i);
    void removeUser(int i);
    void removeBannedUser(std::string nickname);
    void addInvitedUser(std::string nickname);
    void removeInvitedUser(std::string nickname);
    bool isInvited(std::string nickname) const;
  
  public:
    void setPrefix(char prefix);
    void setOnlineUsers(int onlineUsers);
    void setName(std::string name);
    void setKey(std::string key);
    void setTopic(std::string topic);
    void setInviteOnly(bool inviteOnly);
    void setTopicRestricted(bool topicRestricted);
    void setUserLimit(int userLimit);
};

