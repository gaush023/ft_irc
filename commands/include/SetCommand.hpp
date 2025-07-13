#pragma once

#include "../../../ICommand.hpp"

class SetCommand : public ICommand
{
  public:
    std::string execute(Request &request, int sender_fd) = 0;
};
