#include "include/NoticeCommand.hpp"

NoticeCommand::NoticeCommand(Server& server) : _server(server) {}

std::string NoticeCommand::execute(Request& req, int fd)
{
    Client* cli = _server.getClientByFd(fd);

    if (!cli->getRegistered())
        return _server._printMessage("451", cli->getNickName(),
                                    ":You have not registered");

    if (req.args.size() < 2)
        return _server._printMessage("461", cli->getNickName(),
                                    "NOTICE :Not enough parameters");

    const std::string& target = req.args[0];
    const std::string& text   = req.args[1];

    if (!target.empty() && (target[0] == '#' || target[0] == '&')) {
        Channel* ch = _server.getChannel(target);
        if (!ch) return "";             
        std::string line = cli->getUserPerfix() +
                           "NOTICE " + target + " :" + text + "\r\n";
        _server.broadcastToChannel(ch, line);        
    }
    else {
        Client* dst = _server.getClientByNickName(target);
        if (!dst) return "";                        
        std::string line = cli->getUserPerfix() +
                           "NOTICE " + target + " :" + text + "\r\n";
        dst->sendRaw(line);
    }
    return "";
}
