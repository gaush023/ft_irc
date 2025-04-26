#include "heardes/Server.hpp"

Server::Server() : _name(), _password(), _socketfd(0), _clients(), _pfds(nullptr), _online_c(0), _prefix(":"), _allChannels(), _unavailableUserName(), _clinetsNicknames{}
{
}

Server::Server(std::string Name, int max_online, std::string Port, std::string Password): _clients()
{
  this->_name = Name;
  this->_max_online_c = max_online + 1;
  this->_password = Password;
  this->_online_c = 0;
  this->_pdfs = new pollfd[max_online];
  _getSocket(Port); 
  this->_pdfs[0].fd = this->socketfd;
  this->_pdfs[0].events = POLLIN;
  this->_online_c++;
}

server::~Server()
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

