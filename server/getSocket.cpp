#include "../headers/Server.hpp"

void Server::_getSocket(std::string Port)
{
  int yes = 1;
  int status;
  struct addrinfo hint;
  struct addrinfo *servinfo;
  struct addrinfo *tmp;

  memset(&hint, 0, sizeof hint);
  hint.ai_family = AF_INET;
  hint.ai_socktype = SOCK_STREAM;
  hint.ai_protocol = getprotobyname("TCP")->p_proto;
  
  status = getaddrinfo("0.0.0.0", Port.c_str(), &hint, &servinfo);
  
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
  
  if (fcntl(this->_socketfd, F_SETFL, O_NONBLOCK) == -1)
  {
    std::cerr << "Failed to set non-blocking mode on server socket" << std::endl;
    exit(-1);
  }
};


