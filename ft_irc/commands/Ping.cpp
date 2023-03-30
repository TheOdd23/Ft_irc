#include "../Servers.hpp"

int	Servers::Ping(Users &user, Parser &parser)
{
	user.setActive(false);
	send(user.getFd(), parser.SendReply("1", "PING", "Resquesting Pong"), parser.getReply().size(), 0);
	return (0);
}