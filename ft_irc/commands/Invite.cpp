#include "../Servers.hpp"

int	Servers::Invite(Users &user, Parser &parser)
{
	if (parser.getArgs().size() < 2)
	{
		send(user.getFd(), parser.SendReply("461", parser.getCommand(), "Not enough arguments, needs a nickname then a user"), parser.getReply().size(), 0);
		return (1);
	}
	std::map<std::string, Users *>::iterator uit = userPointer.find(parser.getArgs()[0]);
	if (uit == userPointer.end())
	{
		send(user.getFd(), parser.SendReply("401", "*", "User is inexistant"), parser.getReply().size(), 0);
		return (1);
	}
	if (user.getChannels().find(parser.getArgs()[1]) == user.getChannels().end())
	{
		send(user.getFd(), parser.SendReply("442", "*", "You can't invite somebody in a channel you are not in"), parser.getReply().size(), 0);
		return (1);
	}
	std::map<std::string, Channels>::iterator	it = Chans.find(parser.getArgs()[1]);
	if (it == Chans.end())
	{
		return (1);
	}
	if (uit->second->getChannels().find(it->second.getName()) != uit->second->getChannels().end())
	{
		send(user.getFd(), parser.SendReply("443", "*", "User already on channel"), parser.getReply().size(), 0);
		return (1);
	}
	if (!it->second.getOp(user.getNickname()) && it->second.getFlag(I))
	{
		send(user.getFd(), parser.SendReply("482", parser.getArgs()[0], "You do not have the privileges to invite somebody out on the channel"), parser.getReply().size(), 0);
		return (1);
	}
	else
	{
		if (uit->second->getAway() == true)
		{
			send(user.getFd(), parser.SendReply("301", parser.getArgs()[0], uit->second->getAwayMsg()), parser.getReply().size(), 0);
			return (1);
		}
		else
		{
			uit->second->addChannel(it->second.getName());
			it->second.addUser(*uit->second, it->second.getPass(), true);
			std::string msg = ":" + uit->second->getNickname() + "!" + uit->second->getUsername() + "@" + uit->second->getHostname() + " INVITE " + it->second.getName();
			send(user.getFd(), msg.c_str(), msg.size(), 0);
			msg = ":" + uit->second->getNickname() + "!" + uit->second->getUsername() + "@" + uit->second->getHostname() + " JOIN " + it->second.getName() + "\n";
          	send(uit->second->getFd(), msg.c_str(), msg.size(), 0);
		}
	}
	return (0);
}