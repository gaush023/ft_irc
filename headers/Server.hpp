#pragma once
#include "Client.hpp"

Class Client;

class Server
{
  private:
    std::string _name;
    std::string _password;
    int _socketfd;
    std::map<int, Client *> _clients;
    struct pollfd *_pfds;    
    int _online_c;
    int _max_online_c;
    std::string _prefix;
     

  public:
    Server();
    std::string _welcomeMessage(void);
    void _getSocket(std::string port);

}
