#pragma once  

#include "Server.hpp"

struct Modes
{
  bool away;
  bool invisible;
  bool wallops;
  bool restricted;
  bool op;
  bool local0p;
  bool server;
};  

class Client 
{
  private:
    int _clientfd;
    bool _Auth;
    bool _Registered;
    bool _isOperator;
    std::string _Nickname;
    std::string _Username;
    std::string _Fullname;
    const std::string _Host;
    std::string _ID;
    struct  sockaddr_storage _remoteAddr;
    socklen_t _addrlen;
    struct Modes _modes;
    std::map<std::string, Channel *> _joinedChannels;
  
  public:
    Client();
    Clinet(int fd);
    Client(const Client & x);
    ~Client();
    Client & operator=(const Client & rhs);
    std::sting getUserPrefix() const;
    std::string getUserInfo() const;
    std::string getAllChannels() const;
    std::map<std::string, Channel *> getJoinedChannels() const;

  public:
    void setUserName(std::string UserName);
    void setNickName(std::string NickName);
    void setFullName(std::string FullName);
    void setID(std::string ID);
    void setClientfd(int fd);
    void setAuth(int Auth);
    void setRegistered(int Registered);
    void setIsOperator(int isOperator);
    void setMode(int value, char mode);
    void joinChannel(std::string ChannelName, Channel *channel);
    void leaveChannel(std::string ChannelName);
    std::string leaveAllChannels();
    
  public:
    int isjoined(std::string ChannelName) const;
    int joinedChannelsCount() const;
};
    
