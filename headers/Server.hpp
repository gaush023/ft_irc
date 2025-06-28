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
    
  private:
    int _sendall(int destfd, std::string message);

  public:
    Server();
    void _getSocket(std::string port);
    std::pair<int, Client *> findRole(int i);
    void _addToPoll(int newfd);
    void _removeFromPoll(int fd);
    std::string _printMessage(std::string num, std::string nickname, std::string msg)
    std::string _welcomeMsg(void);
    int _sendAll(int destfd, std::string message);
    void _newClient(void);
    void startServer(void);
    std::string _parsing(std::string message, int sender_fd);
    std::string Server::_joinedChannel( Request request, int fd)

}
