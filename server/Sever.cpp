#include "../headers/Server.hpp"

Server::Server() : _name(), _password(), _socketfd(0), _clients(), _pfds(NULL), _online_c(0), _prefix(":"), _allChannels(), _unavailableUserName(), _clientNicknames() {}

Server::Server(std::string Name, int max_online, std::string Port, std::string Password): _clients()
{
    this->_name = Name;
    this->_max_online_c = max_online + 1;
    this->_password = Password;
    this->_online_c = 0;
    this->_pfds = new pollfd[max_online];
    for (int i = 0; i < max_online; i++)
    {
        this->_pfds[i].fd = -1;
        this->_pfds[i].events = 0;
        this->_pfds[i].revents = 0;
    }
    _getSocket(Port); 
    this->_pfds[0].fd = this->_socketfd;
    this->_pfds[0].events = POLLIN;
    this->_pfds[0].revents = 0;
    this->_online_c++;
}

Server::~Server()
{
  if(this->_pfds)
    delete [] this->_pfds;
  std::map<int, Client *>::iterator it = this->_clients.begin(); 
  while(it != this->_clients.end())
  {
    delete it->second;
    it++;
  }
  this->_clients.clear();
  std::map<std::string, Channel *>::iterator it2 = this->_allChannels.begin();
  while(it2 != this->_allChannels.end())
  {
    delete it2->second;
    it2++;
  }
  this->_allChannels.clear();
  for (std::map<std::string, ICommand *>::iterator it = this->_commands.begin(); it != this->_commands.end(); ++it)
  {
    delete it->second;
  }
  _commands.clear();
}

std::string Server::_printMessage(std::string num, std::string nickname, std::string msg)
{
  if(nickname.empty())
    nickname = "unknown";
  return ":" + this->_name + " " + num + " " + nickname + " " + msg + "\r\n";
}

void Server::_registerCommands(void)
{
    this->_commands["PASS"]     = new SetCommand(*this);
    this->_commands["NICK"]     = new SetCommand(*this);
    this->_commands["USER"]     = new SetCommand(*this);
    this->_commands["OPER"]     = new SetCommand(*this);
    this->_commands["MODE"]     = new ModeCommand(*this);
    this->_commands["PRIVMSG"]  = new PrivmsgCommand(*this);
    this->_commands["NOTICE"]   = new NoticeCommand(*this);
    this->_commands["HELP"]     = new HelpCommand(*this);
    this->_commands["JOIN"]     = new JoinCommand(*this);
    this->_commands["TOPIC"]    = new TopicCommand(*this);
    this->_commands["KICK"]     = new KickCommand(*this);
    this->_commands["PART"]     = new PartCommand(*this);
    this->_commands["QUIT"]     = new QuitCommand(*this);
    this->_commands["PING"]     = new PingCommand(*this);
    this->_commands["INVITE"]   = new InviteCommand(*this);
    this->_commands["SENDFILE"] = new SendFileCommand(*this);
    this->_commands["GETFILE"]  = new GetFileCommand(*this);
    this->_commands["HELPDESK"] = new HelpDeskCommand(*this);
}

void Server::_newClient(void)
{
  struct sockaddr_storage remotaddr;
  socklen_t addrlen;
  int newfd;

  addrlen = sizeof remotaddr;
  newfd = accept(this->_socketfd, (struct sockaddr *)&remotaddr, &addrlen);
  if (newfd < -1)
  {
    std::cerr << "accept: " << strerror(errno) << std::endl;
    return;
  }
  else {
    _addToPoll(newfd);    
    std::string welcomeMsg = _welcomeMsg();
    if (send(newfd, welcomeMsg.c_str(), welcomeMsg.length(), 0) == -1)
    {
      std::cerr << "Client on socket " << newfd << " disconnected or error sending message: " << strerror(errno) << std::endl;
      _removeFromPoll(newfd);
    }
    char *ip = inet_ntoa(((struct sockaddr_in *)&remotaddr)->sin_addr);
    std::cout << "[" << currentDateTime() << "]: new connection from " << ip << " on socket " << newfd << std::endl;
  }
}

void Server::startServer(void)
{
  const int TIMEOUT = 1000;
  while(true)
  {
    int pollCount = poll(this->_pfds, this->_online_c, TIMEOUT);
    if (pollCount == -1)
    {
      std::cerr << "poll error: " << strerror(errno) << std::endl;
      exit(-1); 
    }
    if (pollCount == 0)
    {
      std::cout << "No activity detected, waiting for clients..." << std::endl;
      continue;
    }
    for (int i = 0; i < this->_online_c; i++)
    {
      if(this->_pfds[i].revents & POLLIN)
      {
        if(this->_pfds[i].fd == this->_socketfd)
          _newClient();
        else 
          _ClientRequest(i);
      }
    }
  }
}

std::string Server::_getPassword() const
{
  return this->_password;
};

Client *Server::getClientByFd(int fd) const
{
  std::map<int, Client *>::const_iterator it = this->_clients.find(fd);
  if ( it == this->_clients.end())
    return NULL;
  return it->second;
}


Channel *Server::getChannelByName(const std::string &channelName) const
{
  std::map<std::string, Channel *>::const_iterator it = this->_allChannels.find(channelName);
  if (it == this->_allChannels.end())
    return NULL;
  return it->second;
}

Client *Server::getClientByNickName(const std::string &nickname) const
{
  std::map<int, Client *>::const_iterator it;  
  for (it = this->_clients.begin(); it != this->_clients.end(); ++it)
  {
    if (it->second->getNickName() == nickname)
      return it->second;
  }
  return NULL;
}
