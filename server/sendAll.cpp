#include "../headers/Server.hpp"

int Server::_sendAll(int destfd, std::string message)
{
  const char *msg = message.c_str();  
  size_t total = message.size();
  size_t sent = 0;

  while(sent < total)
  {
    ssize_t bytes = send(destfd, msg + sent, total - sent, 0);
    if (bytes == -1)
    {
      if(errno == EINTR)
        continue;
      std::cerr << "send error: " << strerror(errno) << std::endl;
      return -1;
    }
    if (bytes == 0)
    { 
      std::cerr << "Connection closed by peer ??" << std::endl;
      return -1;
    }
    sent += static_cast<size_t>(bytes);
  }  
  return static_cast<int>(sent);
}

std::string Server::_sendToAllUsers(Channel *channel, int senderFd, std::string message)
{
  std::map<int, Client *> allusers = channel->getAllUsers();
  std::map<int, Client *>::iterator it = allusers.begin();
  std::string reply = this->_clients[senderFd]->getUserPrefix();
  reply.append(message);
  while (it != allusers.end())
  {
    if (senderFd != it->first)
    {
      if (_sendAll(it->first, reply) == -1)
      {
        std::cerr << "_sendAll() error: " << strerror(errno) << std::endl;
        return "";
      }
    }
    it++;
  }
  return "";
}
