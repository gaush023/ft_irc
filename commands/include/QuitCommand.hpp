#pragma once

#include "../../../ICommand.hpp"

class QuitCommand : public ICommand
{
  public:
    std::string execute(Request request, int sender_fd) override;
    
};
