#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "Server.hpp"

class Request {
  public:
    std::vector<std::string> args;
    std::string command;
    bool invalidMessage;

  public:
    Request();
    ~Request();
    Request(const Request &other);
    Request &operator=(const Request &other);
};

#endif // REQUEST_HPP
