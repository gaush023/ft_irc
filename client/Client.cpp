#include "../headers/Client.hpp"

Client::Client(): _clientfd(0), _Auth(false), _Registered(false), _isOperator(false), _NickName(), _UserName(), _RealName(), _Host("deez.nuts"), _ID(), _remoteAddr(), _addrlen(),_modes(), _joinedChannels{};
Client::Client(int fd): _clientfd(fd), _Auth(false), _Registered(false), _isOperator(false), _NickName(), _UserName(), _RealName(), _Host("deez.nuts"), _ID(), _remoteAddr(), _addrlen(),_modes(), _joinedChannels{};
Client::Client(const Client& x): _Host(x._Host) { *this = x; };

Clinet &Client::operator=( const Client& rhs)
{
  if(this == &rhs)
    return *this;
  
  this->_clientfd = rhs._clientfd;
  this->_Registered = rhs._Registered;
  this->_isOperator = rhs._isOperator;
  this->_NickName = rhs._NickName;
  this->_UserName = rhs._UserName;
  this->_FullName = rhs._FullName;
  this->_Auth = rhs._Auth;
  this->_remotaddr = rhs._remoteAddr;
  this->_addrlen = rhs._addrlen;
  this->_joinedChannels.insert(rhs._joinedChannels.begin(), rhs._joinedChannels.end());
  return *this;
}

Client::~Client() {};


