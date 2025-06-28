#pragma once

#include "Server.hpp"

#define USERALREADYJOINED 0
#define USERJOINED 1
#define NOTINCHANNEL 2
#define BANDFROMECHANNEL 3
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
    std::map<Client *> _members;
    std::vector<Client *> _bannedUsers;
    std::map<Client *> _operators;
    std::map<Client *> _voices;

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
    std::map<std::string, Client *> const &getBannedUsers() const;
  
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
  
  public:
    void setPrefix(char prefix);
    void setOnlineUsers(int onlineUsers);
    void setName(std::string name);
    void setKey(std::string key);
    void setTopic(std::string topic);
};

