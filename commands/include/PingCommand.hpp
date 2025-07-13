#pragma once

#include "../../../ICommand.hpp"

class PingCommand : public ICommand
{
  public:
    std::string execute(Request request, int sender_fd) override;
};
