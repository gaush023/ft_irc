#include "../headers/Server.hpp"

void Server::_ClientRequest(int i)
{
  char buf[6000];
  int sender_fd = this->_pfds[i].fd;
  int nbytes = recv(sender_fd, buf, sizeof(buf), 0);

  if (nbytes <= 0)
  {
    if (nbytes == 0)
      std::cout << "[" << currentDateTime() << "]: socket " << sender_fd << " hung up" << std::endl;
    else
      std::cout << "recv() error: " << strerror(errno) << std::endl;
    
    close(sender_fd);
    _removeFromPoll(i);
  }
  else {
        _recvBuf[sender_fd] += std::string(buf, nbytes);
        size_t pos;
        while((pos = _recvBuf[sender_fd].find("\n")) != std::string::npos)
        {
              std::string message = _recvBuf[sender_fd].substr(0, pos);
              _recvBuf[sender_fd].erase(0, pos + 2);
              std::string ret = _parsing(message, this->_pfds[i].fd);
              if ( message != "QUIT"  && send(sender_fd, ret.c_str(), ret.length(), 0) < 0)
              { 
                  std::cerr << "send() error: " << strerror(errno) << std::endl;
                  std::cout << message << std::endl;
              }
        }
        std::cout << "Buffer size after: " << _recvBuf[sender_fd].size() << std::endl;
    }
  memset(&buf, 0, 6000);
};

Request Server::_splitRequest(std::string req)
{
  Request request;
  size_t i = 0;
  size_t j = 0;
  size_t len = req.length();

  if (req[i] == ' ' || !req[i]) {
    request.invalidMessage = true;
    return (request);
  }
  j = i;
  while(i < len && req[i])
  {
    if(req[i] == ' ')
    {
      if (i + 1 >= len || req[i + 1] == ' ') {
        request.invalidMessage = true;
        return (request);
      }
      request.args.push_back(req.substr(j, i - j)); 
      while (req[i] == ' ')
        i++;
      j = i;
    }
    if (req[i] == ':')
    {
      if (i == 0 || req[i - 1] != ' ') {
        request.invalidMessage = true;
        return (request);
      }
      request.args.push_back(req.substr(i + 1, req.length() - i));
      request.command = request.args[0];
      request.args.erase(request.args.begin());
      return (request);
    }
    i++;
  }
  if(i > j)
    request.args.push_back(req.substr(j, i - j));
  request.command = request.args[0];
  request.args.erase(request.args.begin());
  return (request);
};

