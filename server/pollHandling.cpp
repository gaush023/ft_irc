#include "../headers/Server.hpp"

void _addToPoll(int newfd)
{
  if(this->_online_c == this->_max_online_c)
  {
    this->_max_online_c *= 2;
    this->_pfds = (struct pollfd *)realloc(this->_pfds, sizeof(struct pollfd) * this->_max_online_c);
    if (this->_pfds == nullptr)
    {
      std::cerr << "Error reallocating memory for pollfd array" << std::endl;
      //error handling
    }
  }
  this->_pfds[this->_online_c].fd = newfd;
  this->_pfds[this->_online_c].events = POLLIN;
  this->_clients.insert(std::pai<int, Clinet *>(newfd, newClient));
  this->_online_c++;
}

void _removeFromPoll(int i)
{
  close(this->_pfds[i].fd);
  this->_pfds[i] = this->_pfds[this->_online_c - 1];
  this->_clients.erase(this->_pfds[i].fd);
  this->_online_c--;
}

