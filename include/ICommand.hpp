#pragma once

#incule "../headers/Server.hpp"

class Server;
class Request;

class ICommand
{
  private:
    // No private members
  public:
    virtual ~ICommand() {}
    virtual std::string execute(Request request, int sender_fd, Server &server) = 0;
    
    std::string _setUserName(Request request, int sender_fd);
    std::string _setNickName(Request request, int sender_fd);
    std::string _setFullName(Request request, int sender_fd);
    std::string _setPassword(Request request, int sender_fd);
    std::string _setOper(Request request, int sender_fd);
    std::string _setMode(Request &request, int sender_fd);
    std::string _joinChannel(Request request, int sender_fd);
    bool _validMode(Request request);
    std::string _quit(Request request, int sender_fd);
    std::string _part(Request request, int sender_fd);
    std::string _topic(Request request, int sender_fd);
    std::string _kick(Request request, int sender_fd);
    std::string _ping(Request request, int sender_fd);
};


