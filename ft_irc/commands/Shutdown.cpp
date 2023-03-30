#include "../Servers.hpp"

int	Servers::Shutdown(Users &user, Parser &parser)
{
	if (parser.getArgs()[0] == "End_me")
	{
		_end_server = true;
		return (0);
	}
	send(user.getFd(), parser.SendReply("464", "*", "Incorrect Password\n"), parser.getReply().size(), 0);
	return (1);
}