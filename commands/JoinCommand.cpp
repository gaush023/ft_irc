#include "./include/JoinCommand.hpp"

JoinCommand::JoinCommand(Server& server) : _server(server) {}

std::vector<std::string>
JoinCommand::_commaSeparator(const std::string& s) const
{
    std::vector<std::string> out;
    std::string tmp;
    std::istringstream iss(s);
    while (std::getline(iss, tmp, ',')) out.push_back(tmp);
    return out;
}

int JoinCommand::_createChannel(const std::string& name, int fd)
{
    if (_server.channelExists(name))
        return 0;                       

    if (name.empty() || std::string("#&+!").find(name[0]) == std::string::npos)
        return BADCHANMASK;             

    Channel* ch = new Channel(name, _server.getClientByFd(fd));
    _server.registerChannel(name, ch);  
    _server.getClientByFd(fd)->joinChannel(name, ch);

    _server.getClientByFd(fd)->sendRaw(
        _server.getClientByFd(fd)->getUserPerfix() + "JOIN " + name + "\r\n");

    return USERJOINED;
}

int JoinCommand::_createPrvChannel(const std::string& name,
                                   const std::string& key,
                                   int fd)
{
    if (_server.channelExists(name))
        return BADCHANNELKEY;           
    if (name.empty() || std::string("#&+!").find(name[0]) == std::string::npos)
        return BADCHANMASK;

    Channel* ch = new Channel(name, key, _server.getClientByFd(fd));
    _server.registerChannel(name, ch);
    _server.getClientByFd(fd)->joinChannel(name, ch);

    _server.getClientByFd(fd)->sendRaw(
        _server.getClientByFd(fd)->getUserPerfix() + "JOIN " + name + "\r\n");

    return USERJOINED;
}

std::string JoinCommand::execute(Request& request, int fd)
{
    Client* cli = _server.getClientByFd(fd);
    std::string nick = cli->getNickName();

    if (!cli->getRegistered())
        return _server._printMessage("451", nick, ":You have not registered");

    if (request.args.empty())
        return _server._printMessage("461", nick, ":Not enough parameters");

    if (request.args[0] == "0")
        return cli->leaveAllChannels();         

    std::vector<std::string> chans = _commaSeparator(request.args[0]);
    std::vector<std::string> keys;
    if (request.args.size() > 1)
        keys = _commaSeparator(request.args[1]);

    std::vector<std::string>::iterator itC = chans.begin();
    std::vector<std::string>::iterator itK = keys.begin();

    for (; itC != chans.end(); ++itC) {
        if (itK != keys.end())
            _createPrvChannel(*itC, *itK++, fd);
        else
            _createChannel(*itC, fd);
    }
    return "";
}
