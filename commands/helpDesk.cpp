#include "include/HelpDesk.hpp"

HelpDesk::HelpDesk(Server& server)
    : _server(server)
{}

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

std::string HelpDesk::execute(Request& request, int sender_fd) {
    if (request.args.empty())
        return _server.getHelpMessage();

    const std::string& cmd = request.args[0];
    if (cmd == "0")
        return _server.getHelpMessage();
    else if (cmd == "1")
        return _server.listAllChannels();
    else if (cmd == "2")
        return _server.getServerInfo();
    else if (cmd == "3") {
        if (request.args.size() == 2)
            return _server.getChannelInfo(request.args[1], sender_fd);
        else
            return "Usage: HELPDESK 3 [CHANNEL_NAME]\n";
    } else {
        return "Invalid command. Use 0–3.\n";
    }
}
