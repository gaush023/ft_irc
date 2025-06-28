#include "../headers/Server.hpp"

std::string Server::_welcomeMsg(void)
{
    std::string welcome = RED;
    welcome.append("██╗    ██╗███████╗██╗      ██████╗ ██████╗ ███╗   ███╗███████╗\n");
    welcome.append("██║    ██║██╔════╝██║     ██╔════╝██╔═══██╗████╗ ████║██╔════╝\n");
    welcome.append("██║ █╗ ██║█████╗  ██║     ██║     ██║   ██║██╔████╔██║█████╗\n");
    welcome.append("██║███╗██║██╔══╝  ██║     ██║     ██║   ██║██║╚██╔╝██║██╔══╝\n");
    welcome.append("╚███╔███╔╝███████╗███████╗╚██████╗╚██████╔╝██║ ╚═╝ ██║███████╗\n");
    welcome.append(" ╚══╝╚══╝ ╚══════╝╚══════╝ ╚═════╝ ╚═════╝ ╚═╝     ╚═╝╚══════╝\n");
    welcome.append(BLUE);
    welcome.append("You need to login so you can start chatting OR you can send HELP to see how :) \n");
    welcome.append(RESET);
    return (welcome);
}

int Server::_sendall(int destfd, std::string msg)
{
    int total = 0;
    int bytesleft = msg.length();
    int b;

    while(total < (int)msg.length())
    {
      b = send(destfd, msg.c_str() + total, bytesleft, 0);
      if (b == -1)
      {
        if (errno == EINTR)
          continue;
        if (errno == EAGAIN || errno == EWOULDBLOCK)
        {
          struct pollfd pfd;
          pfd.fd = destfd;
          pfd.events = POLLOUT;
          int rv = poll(&pfd, 1, 500);
          if (rv > 0 && (pfd.revents & POLLOUT))
          {
            continue; // socket is ready to send
          }
          else if (rv == 0)
          {
            return -1; // timeout occurred
          }
          else {
            return -1; // poll error
          }
        }
        return -1; // error occurred
      }
      total += b;
      bytesleft -= b;
    }
    if (b == -1)
      return -1;
    return 0;
}
