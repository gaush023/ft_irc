#pragma once


#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <cstring>
#include <algorithm>
#include <utility>
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <time.h>
#include <fcntl.h>

class Client;
class Channel;
class Request;
class File;

#include "ToString.hpp"
#include "Request.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "../commands/file/File.hpp"

class Server
{
  private:
    std::string _name;
    std::string _password;
    int _socketfd;
    std::map<int, Client *> _clients;
    struct pollfd *_pfds;    
    int _online_c;
    int _max_online_c;
    std::string _prefix;
    std::map<std::string, Channel *> _allChannels;
    std::vector<std::string> _unavailableUserName;
    std::vector<std::string> _clientNicknames;  
    std::map<std::string, File> _files;
    std::map<int, std::string> _recvBuf;
    
  private:
    Server();
    std::string _welcomeMsg(void);
    void _getSocket(std::string port);
    void _addToPoll(int newfd);
    void _removeFromPoll(int i);
    void _newClient(void);
		void _ClientRequest(int i);
    int _sendall(int destfd, std::string message);
    void _broadcastmsg(int sender_fd, std::string buf, int nbytes);  
    Request _splitRequest(std::string req);
    std::string _printHelpInfo();
    std::string _printUserModes(std::string ret, int sender_fd);
    std::vector<std::string> _commaSeparator(std::string arg);
		int								_createPrvChannel( std::string ChannelName, std::string ChannelKey, int	 CreatorFd );
    int _createChannel(std::string channelName, int sender_fd);
    int _partChannel(std::string channelName, int sender_fd, std::string message, int isPart);
    std::string _helpDesk(Request request, int sender_fd); 
    std::string _sendFile(Request request, int sender_fd);
    std::string _getFile(Request request, int sender_fd);
    std::string _getBotMessage();
    std::string _listAllChannels() const;
    std::string _channelInfo(std::string channelName, int sender_fd);
    std::string _serverInfo() const;
    std::string _kickedFromChannel(std::string channelName, std::string message, std::vector<std::string> users, int i);
    int _findFdByNickName(std::string nickName);
    std::string _privmsg(Request request, int sender_fd);
    std::string _notice(Request request, int sender_fd);
    std::string _privToUser(std::string user, std::string message, std::string command, int sender_fd);
    std::string _privToChannel(std::string channelName, std::string message, int sender_fd);
    std::string _sendToAllUsers(Channel *channel, int senderFd, std::string message);
    std::string _getPassword() const;


  public:
    Server(std::string Name, int maxClients, std::string port, std::string password);
    Server(const Server &x);
    ~Server();
    Server &operator=(const Server &rhs);

  public:
    void startServer(void);
    Client *getClientByFd(int fd) const;
    Channel *getChannelByName(std::string channelName) const;
    Client *getClientByNickName(std::string nickName) const;

  public:
    std::string getHelpMessage() const;
    std::string getServerInfo() const;
    std::string getChannelInfo(const std::string& channelName, int sender_fd) const;
    std::string listAllChannels() const;
};

