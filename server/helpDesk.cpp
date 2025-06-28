#include "../headers/Server.hpp"

std::string Server::_getBotMessage()
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

std::string Server::_serverInfo() const
{
  std::string server("Server Name: " + this->_name + "\n");
  server.append("Online Users: " + std::to_string(this->_online_c - 1) + "\n");
  server.append("Max Online Users: " + std::to_string(this->_max_online_c) + "\n");
  server.append("Number of Channels: " + std::to_string(this->_allChannels.size()) + "\n");
  return (server);
}

std::string Server::_channelInfo(std::string channelName, int fd) const
{
  std::map<std::string, Channel *>::const_iterator it = this->_allChannels.find(channelName);
  if ( it != this->_allChannels.end())
  {
    if (this->_clients[fd]->isjoined(channelName))
    {
      std::string Info;
      Info.append("Channel Name: " + it->second->getName() + "\n");
      Info.append("Channel Creator: " + it->second->getCreator()->getFullName() + "\n");
      Info.append("Channel Topic: " + it->second->getTopic() + "\n");
      Info.append("Online Users: " + std::to_string(it->second->getOnlineUsers()) + "\n");
      return (Info);
    } 
    else {
      return ("You Need To Join This Channel First!\n");
    }
  }
  return ("There's No Channel Named " + channelName + " in the Server!\n");
}

std::string Server::_listAllChannels() const
{
  std::string channels(YELLOW "███████████████████████████████████████████████████████████████████████████████████████\n");
  channels.append("█              █              █                    █                                  █\n");
  channels.append("█" RESET " Channel Name " YELLOW "█ " RESET "Online Users " YELLOW "█ " RESET "Creator Of Channel " YELLOW "█ " RESET "          Channel Topic          " YELLOW "█\n");
  channels.append("█              █              █                    █                                  █\n");
  std::string channels(YELLOW "███████████████████████████████████████████████████████████████████████████████████████\n");
  std::map<std::string, Channel *>::const_iterator it = this->_allChannels.begin();
  while (it != this->_allChannels.end())
  {
    channels.append("█              █              █                    █                                  █\n");
    channels.append("█ " RESET + it->first + YELLOW " █ " RESET + std::to_string(it->second->getOnlineUsers()) + YELLOW " █ " RESET + it->second->getCreator()->getFullName() + YELLOW " █ " RESET + it->second->getTopic() + YELLOW " █\n");
    channels.append("█              █              █                    █                                  █\n");
    it++;
  }
  if (this->_allChannels.size() == 0)
  {
    channels.append("█                                                                                     █\n");
    channels.append("█                                " RESET "NO CHANNELS IN SERVER" YELLOW "                                █\n");
    channels.append("█                                                                                     █\n");
     
		channels.append("███████████████████████████████████████████████████████████████████████████████████████\n");

  }
  channels.append(RESET "\n\n");
  return (channels);
};

std::string Server::_helpDesk(Request request, int fd)
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
      return ("Online Users: " + std::to_string(this->_online_c - 1) + "\n");
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
