#pragma once

#include "../../../ICommand.hpp"

class NoticeCommand : public ICommand
{
  public:
    std::string execute(Request request, int sender_fd) override;
};
