#include "../Servers.hpp"

int     Servers::Pass(Users &user, Parser &parser)
{
    if (parser.getArgs()[0] != _server_data.Password)
    {
        send(user.getFd(), parser.SendReply("464", "*", "Incorrect Password\n"), parser.getReply().size(), 0);
        user.setPass(false);
        _close_connection = true;
        return (-1);
    }
    user.setPass(true);
    return (0);
}