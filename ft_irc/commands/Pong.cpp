#include "../Servers.hpp"

int	Servers::Pong(Users &user, Parser &parser)
{
	user.setActive(true);
	std::cout << BBLU "PONG message received" CLEAR << std::endl;
	(void)parser;
	return (0);
}