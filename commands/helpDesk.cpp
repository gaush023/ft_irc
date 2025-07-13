#include "../headers/Server.hpp"

std::string HelpDesk::_getBotMessage() const
{
    std::string Greeting = BLUE;
    Greeting.append("\n\n\n\n\n\n\n██████████████████████████████████████████████████████████████████████████████████\n");
    Greeting.append("█                                                                                █\n");
    Greeting.append("█\t ██╗  ██╗███████╗██╗     ██╗██████╗ ███████╗██████╗ ██╗  ██╗██╗  ██╗████████╗\t █\n");
    Greeting.append("█\t ██║  ██║██╔════╝██║     ██║██╔══██╗██╔════╝██╔══██╗██║ ██╔╝██║ ██╔╝╚══██╔══╝\t █\n");
    Greeting.append("█\t ███████║█████╗  ██║     ██║██████╔╝█████╗  ██████╔╝█████╔╝ █████╔╝   ██║   \t █\n");
    Greeting.append("█\t ██╔══██║██╔══╝  ██║     ██║██╔═══╝ ██╔══╝  ██╔══██╗██╔═██╗ ██╔═██╗   ██║   \t █\n");
    Greeting.append("█\t ██║  ██║███████╗███████╗██║██║     ███████╗██║  ██║██║  ██╗██║  ██╗   ██║   \t █\n");
    Greeting.append("█\t ╚═╝  ╚═╝╚══════╝╚══════╝╚═╝╚═╝     ╚══════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝   \t █\n");
    Greeting.append("█                                                                                █\n");
    Greeting.append("██████████████████████████████████████████████████████████████████████████████████\n");
    Greeting.append(BLUE "█                                                                                █\n");
    Greeting.append(BLUE "█\t" RED " Usage: HELPDESK [COMMAND_NUMBER] [MORE_OPTIONS]" BLUE "\r\t\t\t\t\t\t\t\t\t\t\t\t █\n");
    Greeting.append(BLUE "█\t How Can I Help You: (You can use The following list of commands)" BLUE "\r\t\t\t\t\t\t\t\t\t\t\t █\n");
    Greeting.append(BLUE "█\t " CYAN "[0]" RESET " : to List all Your stats" BLUE "\r\t\t\t\t\t\t\t\t\t\t\t █\n");
    Greeting.append(BLUE "█\t " CYAN "[1]" RESET " : to List all Your Joined Channels" BLUE "\r\t\t\t\t\t\t\t\t\t\t\t █\n");
    Greeting.append(BLUE "█\t " CYAN "[2]" RESET " : to see How many user online" BLUE "\r\t\t\t\t\t\t\t\t\t\t\t █\n");
    Greeting.append(BLUE "█\t " CYAN "[3]" RESET " : to List all Channels in Server" BLUE "\r\t\t\t\t\t\t\t\t\t\t\t █\n");
    Greeting.append(BLUE "█\t " CYAN "[4]" RESET " : to List stats of specific Channel" BLUE "\r\t\t\t\t\t\t\t\t\t\t\t █\n");
    Greeting.append(BLUE "█\t " CYAN "[5]" RESET " : to List Infos about the Server" BLUE "\r\t\t\t\t\t\t\t\t\t\t\t █\n");
    Greeting.append(BLUE "█                                                                                █\n");
    Greeting.append(BLUE "██████████████████████████████████████████████████████████████████████████████████\n\n\n\n");
    Greeting.append(RESET);
    return (Greeting);
}

static size_t visibleLength(const std::string& s) {
    size_t len = 0;
    bool inEsc = false;
    for (std::string::size_type i = 0; i < s.size(); ++i) {
        char c = s[i];
        if (!inEsc && c == '\x1b') {
            inEsc = true;
        }
        else if (inEsc && c == 'm') {
            inEsc = false;
        }
        else if (!inEsc) {
            ++len;
        }
    }
    return len;
}

static std::string frameLine(const std::string& content, size_t width) {
    size_t vis = visibleLength(content);
    size_t pad = (vis < width ? width - vis : 0);
    std::string spaces;
    for (size_t i = 0; i < pad; ++i) spaces += ' ';
    return std::string("█ ") + content + spaces + " █\n";
}

std::string HelpDesk::_serverInfo() const
    std::ostringstream oss;
    const size_t WIDTH = 50;  

    oss << BLUE;
    oss << frameLine(std::string("Server Name: ") + this->_name, WIDTH);
    oss << frameLine(std::string("Online Users: ") + toString(this->_online_c - 1), WIDTH);
    oss << frameLine(std::string("Max Online Users: ") + toString(this->_max_online_c), WIDTH);
    oss << frameLine(std::string("Number of Channels: ") + toString(this->_allChannels.size()), WIDTH);
    oss << RESET;
    return oss.str();
}

std::string HelpDesk::_channelInfo(const std::string& channelName, int fd) const
    std::ostringstream oss;
    const size_t WIDTH = 70;
    oss << BLUE;

    std::map<std::string, Channel*>::const_iterator it =
        this->_allChannels.find(channelName);
    if (it != this->_allChannels.end()) {
        if (this->_clients[fd]->isjoined(channelName)) {
            oss << frameLine(std::string("Channel Name: ") +
                             it->second->getName(), WIDTH);
            oss << frameLine(std::string("Channel Creator: ") +
                             it->second->getCreator()->getFullName(), WIDTH);
            oss << frameLine(std::string("Channel Topic: ") +
                             it->second->getTopic(), WIDTH);
            oss << frameLine(std::string("Online Users: ") +
                             toString(it->second->getOnlineUsers()), WIDTH);
        }
        else {
            oss << frameLine("You Need To Join This Channel First!", WIDTH);
        }
    }
    else {
        oss << frameLine(std::string("There's No Channel Named ") +
                         channelName + " in the Server!", WIDTH);
    }
    oss << RESET;
    return oss.str();
}


std::string HelpDesk::_listAllChannels() const
    std::ostringstream oss;
    const size_t WIDTH = 86;
    oss << BLUE;

    oss << frameLine("Channel Name | Online | Creator | Topic", WIDTH);

    if (this->_allChannels.empty()) {
        oss << frameLine("NO CHANNELS IN SERVER", WIDTH);
    }
    else {
        for (std::map<std::string, Channel*>::const_iterator it =
                 this->_allChannels.begin();
             it != this->_allChannels.end();
             ++it)
        {
            std::string line = it->first
                + " | " + toString(it->second->getOnlineUsers())
                + " | " + it->second->getCreator()->getFullName()
                + " | " + it->second->getTopic();
            oss << frameLine(line, WIDTH);
        }
    }

    oss << RESET;
    return oss.str();
}

std::string HelpDesk::execute(Request request, int fd)
{
  std::string Greeting(_getBotMessage());
  if (request.args.size() == 0)
    return (Greeting);
  if (request.args.size() > 0)
  {
    if (request.args[0] == "0")
      return (this->_clients[fd]->getUserInfo());
    else if (request.args[0] == "1")
      return (this->_clients[fd]->getAllChannels());
    else if (request.args[0] == "2")
      return ("Online Users: " + toString(this->_online_c - 1) + "\n");
    else if (request.args[0] == "3")
      return (_listAllChannels());
    else if (request.args[0] == "4")
    {
      if (request.args.size() == 2)
return (_channelInfo(request.args[1], fd));
      else {
        return ("Usage of this Command: HELPDESK 4 [CHANNEL NAME]\n");
      } 
    }
    else if (request.args[0] == "5")
      return (_serverInfo());
    else
      return ("Invalid Command Number! Please use a valid command number.\n");
  }
  return (Greeting);
};
