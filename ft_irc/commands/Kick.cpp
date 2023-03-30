#include "../Servers.hpp"

int	Servers::Kick(Users &user, Parser &parser)
{
	if (parser.getArgs().size() == 0)
	{
		send(user.getFd(), parser.SendReply("461", parser.getCommand(), "No arguments were given"), parser.getReply().size(), 0);
		return (1);
	}
	std::map<std::string, Channels>::iterator	it = Chans.find(parser.getArgs()[0]);
	std::map<std::string, Users *>::iterator uit = userPointer.find(parser.getArgs()[1]);
	if (parser.getArgs().size() < 2)
	{
		if (parser.getArgs()[0][0] != '#')
			send(user.getFd(), parser.SendReply("461", parser.getCommand(), "No Channel was given"), parser.getReply().size(), 0);
		else if (userPointer.find(parser.getArgs()[0]) == userPointer.end())
			send(user.getFd(), parser.SendReply("461", parser.getCommand(), "No user was given"), parser.getReply().size(), 0);
		return (1);
	}
	if (it == Chans.end())
	{
		send(user.getFd(), parser.SendReply("403", parser.getArgs()[0], "Non-existant channel"), parser.getReply().size(), 0);
		return (1);
	}
	if (uit == userPointer.end() || (uit->second->getChannels().find(it->second.getName()) == uit->second->getChannels().end()))
	{
		send(user.getFd(), parser.SendReply("442", parser.getArgs()[1], "User does not exists or isn't on channel"), parser.getReply().size(), 0);
		return (1);
	}
	if (!it->second.getOp(user.getNickname()))
	{
		send(user.getFd(), parser.SendReply("482", parser.getArgs()[0], "You do not have the privileges to kick somebody out of the channel"), parser.getReply().size(), 0);
		return (1);
	}
	uit->second->removeChannel(it->second.getName());
	it->second.RemoveUser(parser.getArgs()[1]);
	if (it->second.getUsers().size() == 0)
		Chans.erase(it);
	std::string msg = ":" + user.getNickname() + "!" + user.getUsername() + "@" + user.getHostname() + " KICK " + it->second.getName() + " " + uit->second->getNickname() + "\n";
    send(uit->second->getFd(), msg.c_str(), msg.size(), 0);
	return (0);
}