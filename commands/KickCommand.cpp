#include "./include/KickCommand.hpp"

KickCommand::KickCommand(Server& server) : _server(server) {}

std::vector<std::string> KickCommand::_commaSeparator(const std::string& s) const {
    std::vector<std::string> out;
    std::string tmp;
    std::istringstream iss(s);
    while (std::getline(iss, tmp, ',')) out.push_back(tmp);
    return out;
}

std::string KickCommand::_kickFromChannel(const std::string& channelName, const std::string& message, const std::vector<std::string>& users, int sender_fd)
{
    Channel* ch = _server.getChannel(channelName);
    Client* kicker = _server.getClientByFd(sender_fd);
    std::string nick = kicker->getNickName();

    if (!ch)
        return _server._printMessage("403", nick, channelName + " :No such channel");
    if (!ch->hasUser(sender_fd))
        return _server._printMessage("442", nick, channelName + " :You're not on that channel");
    if (!ch->isOperator(sender_fd))
        return _server._printMessage("482", nick, channelName + " :You're not channel operator");

    for (std::vector<std::string>::const_iterator it = users.begin(); it != users.end(); ++it) {
        Client* target = _server.getClientByNickName(*it);
        if (!target || !ch->hasUser(target->getClientfd()))
            return _server._printMessage("441", nick, *it + " " + channelName + " :They aren't on that channel");

        std::string line = kicker->getUserPerfix() + "KICK " + channelName + " " + *it +
                           (message.empty() ? "\r\n" : " :" + message + "\r\n");
        _server.broadcastToChannel(ch, line);
        ch->removeUser(target->getClientfd());
        target->leaveChannel(channelName);
    }
    return "";
}

std::string KickCommand::execute(Request& request, int fd)
{
    Client* cli = _server.getClientByFd(fd);
    std::string nick = cli->getNickName();

    if (!cli->getRegistered())
        return _server._printMessage("451", nick, ":You have not registered");
    if (request.args.size() < 2)
        return _server._printMessage("461", nick, ":Not enough parameters");

    std::vector<std::string> channels = _commaSeparator(request.args[0]);
    std::vector<std::string> users = _commaSeparator(request.args[1]);
    std::string msg = request.args.size() > 2 ? request.args[2] : "";

    for (size_t i = 0; i < channels.size(); ++i) {
        std::string err = _kickFromChannel(channels[i], msg, users, fd);
        if (!err.empty())
            return err;
    }
    return "";
}
