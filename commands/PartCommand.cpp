#include "include/PartCommand.hpp"

PartCommand::PartCommand(Server& server) : _server(server) {}

static std::vector<std::string> splitComma(const std::string& s)
{
    std::vector<std::string> out;
    size_t pos = 0, start = 0;
    while ((pos = s.find(',', start)) != std::string::npos) {
        out.push_back(s.substr(start, pos - start));
        start = pos + 1;
    }
    out.push_back(s.substr(start));
    return out;
}

std::string PartCommand::execute(Request& req, int fd)
{
    Client* cli = _server.getClientByFd(fd);
    const std::string& nick = cli->getNickName();

    if (!cli->getRegistered())
        return _server._printMessage("451", nick, ":You have not registered");

    if (req.args.empty())
        return _server._printMessage("461", nick, "PART :Not enough parameters");

    std::vector<std::string> channels = splitComma(req.args[0]);
    const std::string msg = (req.args.size() > 1) ? req.args[1] : "";

    for (size_t i = 0; i < channels.size(); ++i) {
        int ret = _partChannel(channels[i], fd, msg, 1);
        if (ret != 0)   
            return ret == -1 ? "" : _server._printMessage("403", nick,
                                                         channels[i] + " :No such channel");
    }
    return "";  
}

int PartCommand::_partChannel(const std::string& chanName,
                              int                target_fd,
                              const std::string& msg,
                              int                isPart)
{
    Channel* ch = _server.getChannel(chanName);
    Client*  who = _server.getClientByFd(target_fd);
    if (!ch) return -1;

    if (!ch->hasUser(target_fd)) {
        if (isPart)
        { 
          Client* target = _server.getClientByFd(target_fd);
          target->sendRaw(
              _server._printMessage("442", target->getNickName(),
                                    chanName + " :You're not on that channel"));
          return 0;
        }
    }

    std::string line = who->getUserPerfix() +
                       "PART " + chanName +
                       (msg.empty() ? "" : " :"+msg) + "\r\n";
    _server.broadcastToChannel(ch, line);

    ch->removeUser(target_fd);
    who->leaveChannel(chanName);

    if (ch->getOnlineUsers() == 0)
        _server.destroyChannel(chanName);

    return 0;
}
