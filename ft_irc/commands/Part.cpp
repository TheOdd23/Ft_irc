#include "../Servers.hpp"

int	Servers::Part(Users &user, Parser &parser)
{
	if (parser.getArgs().size() == 0)
	{
		send(user.getFd(), parser.SendReply("461", parser.getCommand(), "No Channels were given"), parser.getReply().size(), 0);
		return (1);
	}
	std::vector<std::string> clist(parser.SplitComa(parser.getArgs()[0]));
	for (size_t i = 0; i < clist.size(); i++)
	{
		std::map<std::string, Channels>::iterator	it = Chans.find(clist[i]);
		if (it == Chans.end())
			send(user.getFd(), parser.SendReply("403", parser.getArgs()[i], "non-existant Channel"), parser.getReply().size(), 0);
		else if (user.getChannels().find(it->second.getName()) == user.getChannels().end())
			send(user.getFd(), parser.SendReply("442", parser.getArgs()[i], "You can't part with a channel you are not in"), parser.getReply().size(), 0);
		else
		{
			user.removeChannel(it->second.getName());
			it->second.RemoveUser(user.getNickname());
			if (it->second.getUsers().size() == 0)
				Chans.erase(it);
			std::string msg = ":" + user.getNickname() + "!" + user.getUsername() + "@" + user.getHostname() + " PART " + it->second.getName() + "\n";
          	send(user.getFd(), msg.c_str(), msg.size(), 0);
		}
	}
	return (0);
}