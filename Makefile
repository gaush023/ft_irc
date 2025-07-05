NAME = ftirc
:q:q
:q
:wq



CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRCS = \
main.cpp \
channel/Channel.cpp \
client/Client.cpp \
file/File.cpp file/FileTransfer.cpp \
request/Request.cpp \
server/JoinCommand.cpp \
server/KickCommand.cpp \
server/PartCommand.cpp \
server/PrivmsgCommand.cpp \
server/Sever.cpp \
server/commands.cpp \
server/getSocket.cpp \
server/helpDesk.cpp \
server/msgHandling.cpp \
server/pollHandling.cpp \
server/requestHeadling.cpp \
server/sendAll.cpp \
server/utils.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
