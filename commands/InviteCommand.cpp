#include "./include/InviteCommand.hpp"

InviteCommand::InviteCommand(Server &server) : _server(server)
{}

InviteCommand::~InviteCommand()
{}

std::string InviteCommand::_inviteToChannel(Channel *channel, Client *user, const std::string &nickname)
{
  Channel *channel = _server.getChannelByName(channelName);  
  Client *invitee = _server.getClientByNickName(userName);
  Client *inviter = _server.getClientByFd(sender_fd);
  const std::string &nickname = inviter->getNickName();

  std::string message = _server._printMessage("341", nickname, userName + " " + channelName);
  _server._sendall(invitee->getClientfd(), message);
  std::string inviteMessage = inviter->getUserPerfix() + " INVITE " + userName + " :" + channelName + "\r\n";
  _server._sendall(inviter->getClientfd(), inviteMessage);
  return "";
}

std::string InviteCommand::_inviteToPrvChannel(const std::stirn &channelName, const std::string &userName, int sender_fd)
{
  Channel *channel = _server.getChannel(channelName);
  Client *inviter = _server.getClientByFd(sender_fd);
  Client *invitee = _server.getClientByNickName(userName);
  const std::string &nickname = inviter->getNickName();

  channel->addUser(userName);
  
  std::string message = _server._printMessage("341", nickname, userName + " " + channelName);
  _server._sendall(invitee->getClientfd(), message);
  
  std::string inviteMessage = inviter->getUserPerfix() + " INVITE " + userName + " :" + channelName + "\r\n";
  _server._sendall(inviter->getClientfd(), inviteMessage);
  return "";
}

std::string InviteCommand::execute(Request request, int sender_fd)
{
  Client *client = _server.getClientByFd(sender_fd);
  const std::string nickname = client->getNickName();
  
  if (!client->getRegistered())
    return _server._printMessage("451", nickname, ":You have not registered");
  
  if (request.args.size() < 2)
    return _server._printMessage("461", nickname, ":Not enough parameters");

  std::string channelName = request.args[0];
  std::string userName = request.args[1];

  if(_server.getAllChannels().find(channelName) == _server.getAllChannels().end())
    return _server._printMessage("403", nickname, channelName + " :No such channel");
  
  Channel *channel = _server.getChannels(channelName);
  
  if (!channel->hasUser(sender_fd))
    return _server._printMessage("442", nickname, channelName + " :You're not on that channel");

  if (channel->isOperator(sender_fd))
    return _server.printMessage("482", nickname, channelName + " :You're not channel operator");

  if (channel->hasUSer(userName))
    return _serveer.printMessage("443", nickname, userName + " :is already on that channel");

  if (channel->isBanned(userName))
    return _server._printMessage("474", nickname, userName + " :Cannot invite a banned user");
  
  Client *user = _server.getClientByNickName(userName);
  if (!user)
    return _server._printMessage("401", nickname, userName + " :No such nick");

  if (channel->isInviteOnly())
    return _inviteToPrvChannel(channel, user, nickname);
  else {
    return _inviteToChannel(channel, user, nickname);
  }
}
