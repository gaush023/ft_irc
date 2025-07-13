#include "./include/OperCommand.hpp"

std::string OperCommand::execute(Request request, int sender_fd)
{
  if(!this->_clients[sender_fd]->getAuth())
    return _printMessage("451", this->_clients[sender_fd]->getNickName(), ":You have not registered");
  if (request.args.size() < 2)
    return _printMessage("461", this->_clients[sender_fd]->getNickName(), "PASS :Not enough parameters");
  if (request.args[0] != "ADMIN")
    return _printMessage("461", this->_clients[sender_fd]->getNickName(), ":Username or password incorrect");
  if (request.args[1] != "DEEZNUTS")
    return _printMessage("464", this->_clients[sender_fd]->getNickName(), ":Username or password incorrect");
  this->_clients[sender_fd]->setIsOperator(true);
  return _printMessage("381", this->_clients[sender_fd]->getNickName(), ":You are now an operator");
}
