#include "Mode.hpp"

Mode::Mode(const std::string& validFlags)
{
    for (std::string::const_iterator it = validFlags.begin();
         it != validFlags.end(); ++it)
        _flags[*it] = false;                 
}

bool Mode::set(char flag, bool on)
{
    std::map<char,bool>::iterator it = _flags.find(flag);
    if (it == _flags.end())
        return false;                        
    it->second = on;
    return true;
}

bool Mode::get(char flag) const
{
    std::map<char,bool>::const_iterator it = _flags.find(flag);
    return (it != _flags.end()) ? it->second : false;
}

std::string Mode::toString() const
{
    std::string out("+");
    for (std::map<char,bool>::const_iterator it = _flags.begin();
         it != _flags.end(); ++it)
        if (it->second) out += it->first;
    if (out == "+")           
        out.clear();
    return out;
}
