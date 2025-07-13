#include "../headers/Server.hpp"

void Server::_addToPoll(int newfd)
{
  if(this->_online_c >= this->_max_online_c)
  {
    std::cerr << "Maximum client limit reached" << std::endl;
    close(newfd);
    return;
  }
  this->_pfds[this->_online_c].fd = newfd;
  this->_pfds[this->_online_c].events = POLLIN;
  try {
    this->_clients.insert(std::pair<int, Client *>(newfd, new Client(newfd)));
  } catch (const std::bad_alloc& e) {
    std::cerr << "Memory allocation failed for new client" << std::endl;
    close(newfd);
    return;
  }
  this->_online_c++;
}

void Server::_removeFromPoll(int i)
{
    if (i < 0 || i >= this->_online_c || this->_pfds[i].fd < 0)
        return;

    int fd = this->_pfds[i].fd;

    close(fd); 
    this->_pfds[i].fd = -1;

    std::map<int, Client*>::iterator it = this->_clients.find(fd);
    if (it != this->_clients.end()) {
        delete it->second;
        this->_clients.erase(it);
    }

    if (i != this->_online_c - 1)
        this->_pfds[i] = this->_pfds[this->_online_c - 1];

    this->_pfds[this->_online_c - 1].fd = -1;
    this->_pfds[this->_online_c - 1].events = 0;
    this->_pfds[this->_online_c - 1].revents = 0;
    this->_online_c--;
}

