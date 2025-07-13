#include "include/PassCommand.hpp"

PassCommand::PassCommand(Server& server, const std::string& password)
    : _server(server), _password(password) {}

std::string PassCommand::execute(Request& req, int fd)
{
    Client* cli = _server.getClientByFd(fd);
    const std::string& nick = cli->getNickName();

    if (req.args.empty())
        return _server._printMessage("461", nick,
                                    "PASS :Not enough parameters");
   if (cli->getRegistered())
        return _server._printMessage("462", nick,
                                    ":Unauthorized command (already registered)");
    if (req.args[0] != _password)
        return _server._printMessage("464", nick, ":Password incorrect");

    cli->setAuth(true);
    return "";                               
}
