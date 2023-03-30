#include "../Servers.hpp"

int     Servers::User(Users &user, Parser &parser)
{
    if (user.getNickname().size() == 0)
    {
        send(user.getFd(), parser.SendReply("431", "*", "Nickname must be validated before entering username"), parser.getReply().size(), 0);
        _close_connection = true;
        return (1);
    }

    if (parser.getArgs().size() != 4)
    {
        //std::cout << BLU "Enters the first wrong stuff " CLEAR << std::endl;
        send(user.getFd(), parser.SendReply("461", parser.getArgs()[0], "Command needs 4 parameters"), parser.getReply().size(), 0);
        _close_connection = true;
        return (1);
    }
    if (Username_list.find(parser.getArgs()[0]) != Username_list.end())
    {
        //std::cout << YEL "Enters the second wrong stuff " CLEAR << std::endl;
        send(user.getFd(), parser.SendReply("462", parser.getArgs()[0], "Username already in use, please enter another valid Nickname"), parser.getReply().size(), 0);
        _close_connection = true;
        return (1);
    }
    std::cout << GRN "User Is accepted " CLEAR << std::endl;
    user.setUserName(parser.getArgs()[0]);
    user.setRealName(parser.getArgs()[3]);
    send(user.getFd(), parser.SendReply("001", parser.getArgs()[0], "Username valid"), parser.getReply().size(), 0);
    user.setRegistration(true);
    return (0);
}