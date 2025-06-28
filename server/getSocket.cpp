#include "../headers/Server.hpp"

void Server::_getSocket(std::string Port)
{
  int yes = 1;
  int status;
  struct addrinfo hints;
  struct addrinfo *servinfo;
  struct addrinfo *p;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.protocol = getprotobyname("TCP")->p_proto;
  
  status = getaddrinfo("0.0.0.0", Port.c_str(), &hints, &servinfo);
  
  if (status != 0)
  {
    std::cerr << "getaddrinfo: " << gai_strerror(status) << std::endl;
    exit(-1);
  }

  for(tmp = servinfo; tmp != NULL; tmp = tmp->ai_next)
  {
    this->_socketfd = socket(tmp->ai_family, tmp->ai_socktype, tmp->ai_protocol);
    if (this->_socketfd < 0)
      continue;
    setsockopt(this->_socketfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
    if (bind(this->_socketfd, tmp->ai_addr, tmp->ai_addrlen) < 0)
    {
      close(this->_socketfd);
      continue;
    }
    break;
  }
  freeaddrinfo(servinfo);
  
  if(tmp == NULL)
  {
    std::cerr << "Failed to bind socket" << std::endl;
    exit(-1);
  }
  if (listen(this->_socketfd, this->_max_online_c) == -1)
  {
    std::cerr << "Failed to listen on socket" << std::endl;
    exit(-1);
  }
};


