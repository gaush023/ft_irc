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
  return <static_cast<int>(sent);
}

