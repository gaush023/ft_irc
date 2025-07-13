#include "SendFileCommand.hpp"

std::string Server::_sendFile(Request request, int i)
{
  if (!this->_clients[i]->getRegistered())
    return (_printMessage("451", this->_clients[i]->getNickName(), ":You have not registered"));
  if (request.args.size() < 2)
    return (_printMessage("461", this->_clients[i]->getNickName(), ":Not enough parameters"));
  if (_findFdByNickName(request.args[0]) == USERNOTFOUND)
    return (_printMessage("401", this->_clients[i]->getNickName(), request.args[0] + " :No such nick/channel"));

  std::fstream ifs(request.args[1].c_str(), std::fstream::in);
  if (ifs.fail())
    return (_printMessage("999", this->_clients[i]->getNickName(), ":Invalid file path"));

  size_t pos = request.args[1].find_last_of('/');
  std::string filename = request.args[1].substr(pos + 1);
  File file(filename, request.args[1], this->_clients[i]->getNickName(), request.args[0]);
  if (this->_files.find(filename) != this->_files.end())
    return (_printMessage("996", this->_clients[i]->getNickName(), ":File by this name already exists"));

  this->_files.insert(std::pair<std::string, File>(filename, file));
  _privToUser(request.args[0], this->_clients[i]->getNickName() + " wants to send you a file called " + filename + ".", "NOTICE", i);
  return ("");
}

SendFileCommand::SendFileCommand(Server& server) : _server(server) {}

std::string SendFileCommand::execute(const Request& request, int sender_fd) {
    return _server._sendFile(request, sender_fd);
}
