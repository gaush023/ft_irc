#include "ModeCommand.hpp"

ModeCommand::ModeCommand(Server& server) : _server(server) {}

std::string ModeCommand::execute(Request& req, int fd)
{
    if (req.args.empty())
        return _err("461", _server.getClientByFd(fd)->getNickName(),
                    "MODE :Not enough parameters");

    if (req.args[0][0] == '#' || req.args[0][0] == '&')
        return _handleChannelMode(req, fd);
    else
        return _handleUserMode(req, fd);
}

std::string ModeCommand::_handleUserMode(Request& req, int fd)
{
    Client* self = _server.getClientByFd(fd);
    const std::string& nick = self->getNickName();

    if (!self->getRegistered())
        return _err("451", nick, ":You have not registered");

    if (req.args[0] != nick)
        return _err("502", nick, ":Cannot change modes for other users");

    if (req.args.size() == 1)
        return _err("221", nick, self->currentUserModeString());

    const std::string& modes = req.args[1];
    bool add = true;
    for (size_t i = 0; i < modes.size(); ++i)
    {
        char c = modes[i];
        if (c == '+') { add = true;  continue; }
        if (c == '-') { add = false; continue; }

        if (c == 'o' && add)         // 一般ユーザーが自分へ +o は不可
            return _err("481", nick, ":Permission denied - you cannot set +o");

        if (!self->setMode(add, c))  // 未知フラグ
            return _err("501", nick, ":Unknown MODE flag");
    }
    return _err("221", nick, self->currentUserModeString());
}

std::string ModeCommand::_handleChannelMode(Request& req, int fd)
{
    const std::string& chanName = req.args[0];
    Channel* ch   = _server.getChannel(chanName);
    Client*  cli  = _server.getClientByFd(fd);
    const std::string& nick = cli->getNickName();

    if (!ch)
        return _err("403", nick, chanName+" :No such channel");
    if (!ch->hasUser(fd))
        return _err("442", nick, chanName+" :You're not on that channel");
    if (!ch->isOperator(fd))
        return _err("482", nick, chanName+" :You're not channel operator");

    /* 照会のみ → 324 */
    if (req.args.size() == 1)
        return _err("324", nick, chanName+" "+ch->currentModeString());

    const std::string& modes = req.args[1];
    size_t   argIdx = 2; bool add = true;
    std::string broadcast = cli->getUserPerfix()+"MODE "+chanName+" ";

    for (size_t i = 0; i < modes.size(); ++i)
    {
        char flag = modes[i];
        if (flag == '+') { add = true;  broadcast += "+"; continue; }
        if (flag == '-') { add = false; broadcast += "-"; continue; }

        switch (flag)
        {
        case 'i':
        case 't':
            ch->setMode(add, flag);
            broadcast += flag;
            break;

        case 'k':                              
            if (add) {
                if (argIdx >= req.args.size())
                    return _err("461", nick, "MODE :Not enough parameters");
                ch->setKey(req.args[argIdx++]);
                broadcast += "k";
            } else {
                ch->removeKey();  broadcast += "k";
            }
            break;

        case 'l':                              
            if (add) {
                if (argIdx >= req.args.size())
                    return _err("461", nick, "MODE :Not enough parameters");
                ch->setUserLimit(atoi(req.args[argIdx++].c_str()));
                broadcast += "l";
            } else {
                ch->disableUserLimit(); broadcast += "l";
            }
            break;

        case 'o':                              
        {
            if (argIdx >= req.args.size())
                return _err("461", nick, "MODE :Not enough parameters");
            const std::string& tn = req.args[argIdx++];
            Client* target = _server.getClientByNick(tn);
            if (!target || !ch->hasUser(target->getClientfd()))
                return _err("441", nick, tn+" "+chanName+" :They aren't on that channel");

            if (add) ch->addOperator(target);
            else     ch->removeOperator(target->getClientfd());
            broadcast += "o "+tn;
            break;
        }

        default:
            return _err("472", nick, std::string(1,flag)+" :unknown mode");
        }
        broadcast += " ";
    }

    broadcast += "\r\n";
    _server.broadcastToChannel(ch, broadcast);
    return "";
}
