#pragma once

#include "../includes/Server.hpp"
#include "../includes/ICommand.hpp"


class PrivmsgCommand : public ICommand
{
  public:
    std::string execute(Request request, int sender_fd, Server &server) override
};
