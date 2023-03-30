#include <poll.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <errno.h>
#include <string>
#include <vector>
#include <cctype>
#include <exception>
#include "Servers.hpp"

bool    check_arg(char *str)
{
    for(int i = 0; str[i]; i++)
    {
        if (!isdigit(str[i]))
            return (1);
    }
    return (0);
}

int main(int ac, char **av)
{
    if (ac != 3 || check_arg(av[1]))
    {
        std::cout << "Please, follow these instructions: ./IRC PORT(between 1 and 65535) PASSWORD" << std::endl;
        return (1);
    }
    try
    {
        size_t  pt = std::stoul(std::string(av[1]));
        if (pt < 1 || pt > 65535)
            throw(std::out_of_range("Port number out of range, stay between 1 and 65535"));
        std::string pw(av[2]);
        Servers main_server(pt, pw);
        main_server.start();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        std::cout << "Please, enter a valid Port" << std::endl;
    }
    return (0);
}