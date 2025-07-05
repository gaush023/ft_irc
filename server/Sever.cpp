#include "../headers/Server.hpp"

Server::Server() : _name(), _password(), _socketfd(0), _clients(), _pfds(NULL), _online_c(0), _prefix(":"), _allChannels(), _unavailableUserName(), _clientNicknames() {}

static void rtrim(std::string &s)
{
    const std::string whitespace = " \t\n\r\f\v\u3000";
    std::size_t end = s.find_last_not_of(whitespace);
    if (end != std::string::npos)
        s.erase(end + 1);
    else
        s.clear();
}


Server::Server(std::string Name, int max_online, std::string Port, std::string Password): _clients()
{
    this->_name = Name;
    this->_max_online_c = max_online + 1;
    std::string trimePassword = Password;
    rtrim(trimePassword);
    this->_password = Password;
    this->_online_c = 0;
    this->_pfds = new pollfd[max_online + 1];
    _getSocket(Port); 
    this->_pfds[0].fd = this->_socketfd;
    this->_pfds[0].events = POLLIN;
    this->_online_c++;
}

Server::~Server()
{
  if(this->_pfds)
    delete [] this->_pfds;
  std::map<int, Client *>::iterator it = this->_clients.begin(); 
  while(it != this->_clients.end())
  {
    delete it->second;
    it++;
  }  
  this->_clients.clear();
  std::map<std::string, Channel *>::iterator it2 = this->_allChannels.begin();
  while(it2 != this->_allChannels.end())
  {
    delete it2->second;
    it2++;
  }
  this->_allChannels.clear();
}

std::string Server::_printMessage(std::string num, std::string nickname, std::string msg)
{
  if(nickname.empty())
    nickname = "unknown";
  return ":" + this->_name + " " + num + " " + nickname + " " + msg + "\n";
}

void Server::_newClient(void)
{
  struct sockaddr_storage remotaddr;
  socklen_t addrlen;
  int newfd;
  
  addrlen = sizeof remotaddr;
  newfd = accept(this->_socketfd, (struct sockaddr *)&remotaddr, &addrlen);
  if (newfd < -1)
  {
    std::cerr << "accept: " << strerror(errno) << std::endl;
    return;
  }
  else {
    _addToPoll(newfd);    
    std::string welcomeMsg = _welcomeMsg();
    if (send(newfd, welcomeMsg.c_str(), welcomeMsg.length(), 0) == -1)
    {
      std::cerr << "Client on socket " << newfd << " disconnected or error sending message: " << strerror(errno) << std::endl;
      _removeFromPoll(newfd);
    }
    char *ip = inet_ntoa(((struct sockaddr_in *)&remotaddr)->sin_addr);
    std::cout << "[" << currentDateTime() << "]: new connection from " << ip << " on socket " << newfd << std::endl;
  }
}

void Server::startServer(void)
{
  while(77)
  {
    int pollCount = poll(this->_pfds, this->_online_c, -1);
    if (pollCount == -1)
    {
      std::cerr << "poll error: " << strerror(errno) << std::endl;
      exit(-1); 
    }
    for (int i = 0; i < this->_online_c; i++)
    {
      if(this->_pfds[i].revents & POLLIN)
      {
        if(this->_pfds[i].fd == this->_socketfd)
          _newClient();
        else 
          _ClientRequest(i);
      }
    }
  }
}

std::string Server::_getPassword() const
{
  return this->_password;
};
