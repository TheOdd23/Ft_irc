#include "../Servers.hpp"

int	Servers::List(Users &user, Parser &parser)
{
	send(user.getFd(), parser.SendReply("321", user.getNickname() + " Channel", 
					"List of channels "), parser.getReply().size(), 0);
	if (parser.getArgs().size() > 0)
	{
		ListDefine(user, parser);
		return 0;
	}
	std::map<std::string, Channels>::iterator it = Chans.begin();
	std::map<std::string, Channels>::iterator ite = Chans.end();

	for (; it != ite; it++)
	{
		if (UsersIsSubscribe(it->second.getName(), user) || !it->second.getFlag(S))
		{
			if (it->second.getFlag(P) && !UsersIsSubscribe(it->second.getName(), user))
				send(user.getFd(), parser.SendReply("322", user.getNickname() + " " + it->second.getName() + " " + std::to_string(it->second.getUsers().size()), 
					"You are not subscribed to this private channel "), parser.getReply().size(), 0);
			else
				send(user.getFd(), parser.SendReply("322", user.getNickname() + " " + it->second.getName() + " " + std::to_string(it->second.getUsers().size()), 
					it->second.getTopic()), parser.getReply().size(), 0);
		}
	}
	send(user.getFd(), parser.SendReply("323", user.getNickname(), "End of list"), parser.getReply().size(), 0);
	return (0);
}

void	Servers::ListDefine(Users &user, Parser &parser)
{
	std::vector<std::string> comaChannels = parser.SplitComa(parser.getArgs()[0]);

	for (size_t i = 0; i < comaChannels.size(); i++)
	{
		if (UsersIsSubscribe(comaChannels[i], user) || Chans[comaChannels[i]].getFlag(S))
		{
			if (Chans[comaChannels[i]].getFlag(P) && !UsersIsSubscribe(comaChannels[i], user))
				send(user.getFd(), parser.SendReply("322", user.getNickname() + " " + comaChannels[i] + " " + std::to_string(Chans[comaChannels[i]].getUsers().size()), 
					"You are not subscribed to this private channel "), parser.getReply().size(), 0);
			else
				send(user.getFd(), parser.SendReply("322", user.getNickname() + " " + comaChannels[i] + " " + std::to_string(Chans[comaChannels[i]].getUsers().size()), 
					Chans[comaChannels[i]].getTopic()), parser.getReply().size(), 0);
		}
	}
	send(user.getFd(), parser.SendReply("323", "*","End of list"), parser.getReply().size(), 0);
	return ;
}