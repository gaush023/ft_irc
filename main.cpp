#include "./include/Sever.hpp"

int main(int ac, char **av)
{
  if(ac != 3)
  {
    std::cerr << "Usage: " << av[0] << " <port> <password>" << std::endl;
    return 1;
  }
  Server srv("DEEZ.NUTS", 10, av[1], av[2]);
  try {
    srv.startServer();
  }
  catch (const std::exception &e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
  }
};
