#include "GetFileCommand.hpp"

std::string Server::_getFile(Request request, int i)
{
  if (!this->_clients[i]->getRegistered())
    return (_printMessage("451", this->_clients[i]->getNickName(), ":You have not registered"));
  if (request.args.size() < 2)
    return (_printMessage("461", this->_clients[i]->getNickName(), ":Not enough parameters"));
  if (this->_files.find(request.args[0]) == this->_files.end())
    return (_printMessage("995", this->_clients[i]->getNickName(), ":No such file on the server"));

  File file(this->_files.find(request.args[0])->second);

  if (file.receiver != this->_clients[i]->getNickName())
    return (_printMessage("994", this->_clients[i]->getNickName(), ":Permission Denied"));

  std::string outPath = request.args[1] + "/" + request.args[0];
  std::fstream ofs(outPath.c_str(), std::fstream::out);
  std::fstream ifs2(file.Path.c_str(), std::fstream::in);

  if (ofs.is_open())
    ofs << ifs2.rdbuf();

  this->_files.erase(file.Name);
  return ("");
}

GetFileCommand::GetFileCommand(Server& server) : _server(server) {}

std::string GetFileCommand::execute(const Request& request, int sender_fd) {
    return _server._getFile(request, sender_fd);
}
