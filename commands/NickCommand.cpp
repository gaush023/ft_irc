#include "./include/NickCommand.hpp"

NickCommand::NickCommand(Server& server) : _server(server) {}

std::string NickCommand::execute(Request& request, int fd)
{
    Client* cli = _server.getClientByFd(fd);
    const std::string& curNick = cli->getNickName();

    if (!cli->getAuth())
        return _server._printMessage("988", curNick,
                                    ":You must be authenticated to set a nickname");

    if (request.args.empty())
        return _server._printMessage("431", curNick, ":Not enough parameters");

    const std::string& newNick = request.args[0];

    for (size_t i = 0; i < newNick.size(); ++i)
        if (!std::isalnum(newNick[i]) && newNick[i] != '-')
            return _server._printMessage("432", curNick, ":Erroneous nickname");

    if (_server.nickExists(newNick))
        return _server._printMessage("433", curNick,
                                    newNick + " :Nickname is already in use");

    _server.unregisterNick(curNick);
    cli->setNickName(newNick);
    _server.registerNick(newNick);

    if (!cli->getUserName().empty()) {
        cli->setID(cli->getUserName() + "!" +
                   cli->getNickName() + "@" + cli->getHost());
        cli->setRegistered(true);
        return _server._printMessage("001", cli->getNickName(),
                                    ":Welcome to the Internet Relay Network " +
                                    cli->getID());
    }
    return "";      
}
