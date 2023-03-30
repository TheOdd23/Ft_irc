#include "../Servers.hpp"


int	Servers::Away(Users &user, Parser &parser)
{
	if (!parser.getMessage().empty()){
		if (user.getAway() == false)
				user.setAway(true);
		user.setAwayMsg(parser.getMessage());
	}
	else{
		if (parser.getArgs().size() == 0)
		{
			if (user.getAway() == true)
				user.setAway(false);
		}
		else
		{
			if (user.getAway() == false)
				user.setAway(true);
			user.setAwayMsg(parser.getArgs().at(0));
		}
	}
	return (0);
}