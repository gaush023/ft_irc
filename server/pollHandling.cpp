#include "../headers/Server.hpp"

void Server::_addToPoll(int newfd)
{
  if(this->_online_c == this->_max_online_c)
  {
    this->_max_online_c *= 2;
    this->_pfds = (struct pollfd *)realloc(this->_pfds, sizeof(struct pollfd) * this->_max_online_c);
    if (this->_pfds == NULL)
    {
      std::cerr << "Error reallocating memory for pollfd array" << std::endl;
    }
  }
  this->_pfds[this->_online_c].fd = newfd;
  this->_pfds[this->_online_c].events = POLLIN;
  this->_clients.insert(std::pair<int, Client *>(newfd, new Client(newfd)));
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
        // Remove nickname from the list
        std::string nickName = it->second->getNickName();
        if (!nickName.empty()) {
            std::vector<std::string>::iterator nick_it = std::find(this->_clientNicknames.begin(), this->_clientNicknames.end(), nickName);
            if (nick_it != this->_clientNicknames.end())
                this->_clientNicknames.erase(nick_it);
        }
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

