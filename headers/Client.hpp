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
    const std::string _hostname;
    std::string _ID;
  
  public:
    Client();
    Clinet(int fd);
    Client(const Client & x);
    ~Client();
    Client & operator=(const Client & rhs);


}
