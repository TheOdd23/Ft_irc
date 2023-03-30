#include "../Servers.hpp"

int	Servers::Notice(Users &user, Parser &parser)
{
	if (parser.getArgs().size() == 0)
		return (1);
	std::map<std::string, Channels>::iterator it = Chans.find(parser.getArgs()[0]);
	if ((parser.getArgs()[0][0] == '#' || parser.getArgs()[0][0] == '&') && it->second.getBan(user.getNickname()))
	{
		for (size_t j = 0; j < it->second.getUsers().size(); j++)
				send(it->second.getUsers()[j].getFd(), parser.SendReply("*", user.getNickname(), parser.getMessage()), parser.getReply().size(), 0);
	}
	else
	{
		if (userPointer.find(parser.getArgs()[0]) != userPointer.end())
			send(userPointer.find(parser.getArgs()[0])->second->getFd(), parser.SendReply("NOTICE", user.getNickname(), parser.getMessage()), parser.getReply().size(), 0);
	}
	send(user.getFd(), parser.SendReply("NOTICE", user.getNickname(), parser.getMessage()), parser.getReply().size(), 0);
	return (0);
}