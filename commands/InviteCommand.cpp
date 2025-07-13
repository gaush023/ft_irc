#include "include/InviteCommand.hpp"


InviteCommand::InviteCommand(Server& server)
  : _server(server)
{}

std::string InviteCommand::_inviteToChannel(const std::string& channelName,
                                            const std::string& userName,
                                            int sender_fd)
{
    Client* inviter = _server.getClientByFd(sender_fd);
    Client* invitee = _server.getClientByNickName(userName);
    std::string nick = inviter->getNickName();

    std::string reply341 = _server._printMessage(
        "341", nick, userName + " " + channelName
    );
    inviter->sendRaw(reply341);

    std::string inviteMsg =
        inviter->getUserPerfix()
      + "INVITE " + userName + " " + channelName
      + "\r\n";
    invitee->sendRaw(inviteMsg);

    return "";
}

std::string InviteCommand::_inviteToPrvChannel(const std::string& channelName,
                                               const std::string& userName,
                                               int sender_fd)
{
    Channel* chan   = _server.getChannel(channelName);
    Client* inviter = _server.getClientByFd(sender_fd);
    Client* invitee = _server.getClientByNickName(userName);
    std::string nick = inviter->getNickName();

    chan->addUser(invitee);

    std::string reply341 = _server._printMessage(
        "341", nick, userName + " " + channelName
    );
    inviter->sendRaw(reply341);

    std::string inviteMsg =
        inviter->getUserPerfix()
      + "INVITE " + userName + " " + channelName
      + "\r\n";
    invitee->sendRaw(inviteMsg);

    return "";
}

std::string InviteCommand::execute(Request& request, int sender_fd)
{
    Client* client = _server.getClientByFd(sender_fd);
    std::string nick = client->getNickName();

    if (!client->getRegistered())
        return _server._printMessage("451", nick, ":You have not registered");

    if (request.args.size() < 2)
        return _server._printMessage("461", nick, ":Not enough parameters");

    const std::string& channelName = request.args[0];
    const std::string& userName    = request.args[1];

    if (!_server.channelExists(channelName))
        return _server._printMessage("403", nick, channelName + " :No such channel");
    Channel* chan = _server.getChannel(channelName);

    if (!chan->hasUser(sender_fd))
        return _server._printMessage("442", nick, channelName + " :You’re not on that channel");

    if (!chan->isOperator(sender_fd))
        return _server._printMessage("482", nick, channelName + " :You’re not channel operator");

    Client* target = _server.getClientByNickName(userName);
    if (!target)
        return _server._printMessage("401", nick, userName + " :No such nick");

    if (chan->hasUser(target->getClientfd()))
        return _server._printMessage("443", nick, userName + " :is already on that channel");

    if (chan->isBanned(userName))
        return _server._printMessage("474", nick, userName + " :Cannot invite a banned user");

    if (chan->isInviteOnly())
        return _inviteToPrvChannel(channelName, userName, sender_fd);
    else
        return _inviteToChannel(channelName, userName, sender_fd);
}
