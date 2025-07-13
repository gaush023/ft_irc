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

int Server::_sendall(int destfd, const std::string& message)
{
    size_t total     = 0;
    size_t to_send   = message.size();
    const char* data = message.c_str();

    while (total < to_send) {
        ssize_t sent = send(destfd, data + total, to_send - total, 0);
        if (sent < 0) {
            if (errno == EINTR)  
                continue;
            return -1;            
        }
        if (sent == 0) {
            return -1;
        }
        total += (size_t)sent;
    }
    return 0;
}
