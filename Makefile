NAME := ircserv
CXX := c++
CXXFLAGS := -Wall -Wextra -Werror -std=c++98

SRC_DIRS := . channel client file request commands server
SRCS := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.cpp))
OBJS := $(SRCS:.cpp=.o)

INCLUDES := -Iinclude -Icommands/include

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
