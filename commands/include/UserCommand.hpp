#pragma once

#include "../../../ICommand.hpp"

class UserCommand : public ICommand
{
  public:
    std::string execute(Request &request, int sender_fd) = 0;
};
