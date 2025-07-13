#pragma once

#include "../../include/Server.hpp"

class Mode
{
  public:
      explicit Mode(const std::string& validFlags);
      bool set(char flag, bool on);
      bool get(char flag) const;
      std::string toString() const;

  private:
      std::map<char, bool> _flags;
};

