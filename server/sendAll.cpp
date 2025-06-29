#include "../headers/Server.hpp"

std::string Server::_sendToAllUsers(Channel *channel, int senderFd, std::string message)
{
  std::map<int, Client *> allusers = channel->getAllUsers();
  std::map<int, Client *>::iterator it = allusers.begin();
  std::string reply = this->_clients[senderFd]->getUserPerfix();
  reply.append(message);
  while (it != allusers.end())
  {
    if (senderFd != it->first)
    {
      if (_sendall(it->first, reply) == -1)
      {
        std::cerr << "_sendall() error: " << strerror(errno) << std::endl;
        return "";
      }
    }
    it++;
  }
  return "";
}
