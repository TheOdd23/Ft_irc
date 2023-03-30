#include "Servers.hpp"

//Compress the Array tracking our sockets (fds) when a connexion is closed.
void    compress_function(struct pollfd *fds, int nfds)
{
    for (int i = 0; i < nfds; i++)
    {
        if (fds[i].fd == -1)
        {
          for(int j = i; j < nfds; j++)
          {
            fds[j].fd = fds[j+1].fd;
          }
          i--;
          nfds--;
        }
    }
}