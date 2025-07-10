#include "../headers/Server.hpp"

bool Server::_validMode(Request request)
{
  char c = request.args[1][1];
  if (request.args[1].length() != 2 || (request.args[1][0] != '+' && request.args[1][0] != '-'))
    return false;
  if (c != 'i' && c != 'w' && c != 'r' && c != 'o' && c != 'O' && c != 's')
    return false;
  return true;
}

std::string Server::_printUserModes(std::string ret, int sender_fd)
{
  ret.append("Current modes for " + toString(this->_clients[sender_fd]->getMode('i')));
  ret.append("\ni: " + toString(this->_clients[sender_fd]->getMode('i')));
  ret.append("\nw: " + toString(this->_clients[sender_fd]->getMode('w')));
  ret.append("\nr: " + toString(this->_clients[sender_fd]->getMode('r')));
  ret.append("\no: " + toString(this->_clients[sender_fd]->getMode('o')));
  ret.append("\nO: " + toString(this->_clients[sender_fd]->getMode('O')));
  ret.append("\ns: " + toString(this->_clients[sender_fd]->getMode('s')));
  ret.append("\n");
  return ret;
}

std::string Server::_setMode(Request& request, int sender_fd) {
    const std::string& nick = this->_clients[sender_fd]->getNickName();

    if (!this->_clients[sender_fd]->getRegistered())
        return _printMessage("451", nick, ":You have not registered");

    if (request.args.size() == 1 && request.args[0] == nick)
        return _printUserModes("", sender_fd);

    if (request.args.size() < 2)
        return _printMessage("461", nick, "MODE :Not enough parameters");

    if (request.args[0] != nick)
        return _printMessage("502", nick, ":Cannot change modes for other users");

    if (!_validMode(request))
        return _printMessage("501", nick, ":Unknown mode flag");

    char flag = request.args[1][1];
    bool add  = (request.args[1][0] == '+');
    this->_clients[sender_fd]->setMode(add, flag);

    return _printMessage("221", nick, request.args[1]);
}
