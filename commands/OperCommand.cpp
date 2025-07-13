#include "include/OperCommand.hpp"

OperCommand::OperCommand(Server& server) : _server(server) {}

std::string OperCommand::execute(Request& req, int fd)
{
    Client* cli = _server.getClientByFd(fd);
    const std::string& nick = cli->getNickName();

    if (!cli->getAuth())
        return _server._printMessage("451", nick, ":You have not registered");

    if (req.args.size() < 2)
        return _server._printMessage("461", nick, "OPER :Not enough parameters");

    if (req.args[0] != "ADMIN" || req.args[1] != "DEEZNUTS")
        return _server._printMessage("464", nick, ":Username or password incorrect");

    cli->setIsOperator(true);
    return _server._printMessage("381", nick, ":You are now an operator");
}
