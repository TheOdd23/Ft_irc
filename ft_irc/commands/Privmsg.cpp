#include "../Servers.hpp"

bool	ft_is_empty_string(std::string msg)
{
	for (size_t i = 0; i < msg.size(); i++)
	{
		if (msg[i] != ' ' && msg[i] != '\r' && msg[i] != '\n' && msg[i] != ':')
			return false;
	}
	return true;
}

int	Servers::Privmsg(Users &user, Parser &parser)
{
	if (ft_is_empty_string(parser.getMessage()))
	{
		send(user.getFd(), parser.SendReply("412", "*", "No message to send!"), parser.getReply().size(), 0);
		return (1);
	}
	if (parser.getArgs().size() == 0)
	{
		send(user.getFd(), parser.SendReply("411", "*", "No recipient has been given!"), parser.getReply().size(), 0);
		return (1);
	}
	if (parser.getArgs().size() > 1)
	{
		send(user.getFd(), parser.SendReply("407", "*", "Please, seperate your recipients with ',' not ' '"), parser.getReply().size(), 0);
		return (1);
	}
	if (parser.getArgs()[0][0] == '#' || parser.getArgs()[0][0] == '&')
	{
		std::vector<std::string> clist(parser.SplitComa(parser.getArgs()[0]));
		for (size_t i = 0; i < clist.size(); i++)
		{
			std::map<std::string, Channels>::iterator	it = Chans.find(clist[i]);
			if (it == Chans.end())
			{
				send(user.getFd(), parser.SendReply("403", parser.getArgs()[i], "Channel inexistant"), parser.getReply().size(), 0);
				//return (1);
			}
			else if ((it->second.getFlag(N) == true && user.getChannels().find(it->second.getName()) == user.getChannels().end()) || it->second.getBan(user.getNickname()) || (it->second.getFlag(M) == true && !it->second.getMod(user.getNickname())))
			{
				send(user.getFd(), parser.SendReply("404", parser.getArgs()[0], "You don't have the right to send messages to the channel"), parser.getReply().size(), 0);
				//return (1);
			}
			else
			{
				for (size_t j = 0; j < it->second.getUsers().size(); j++)
				{ 
					if (it->second.getUsers().at(j).getNickname() == user.getNickname())
						continue;
					if (it->second.getUsers()[j].getAway() == true){
						send(user.getFd(), parser.SendReply("301", user.getNickname() + " " + it->second.getUsers()[j].getNickname(), it->second.getUsers()[j].getAwayMsg()), parser.getReply().size(), 0);
					}
					else
						send(it->second.getUsers()[j].getFd(), parser.SendReply("PRIVMSG", user.getNickname(), user.getNickname() + " " + parser.getMessage()), parser.getReply().size(), 0);
				}
			}
		}
		return (0);
	}
	else
	{
		std::vector<std::string> ulist(parser.SplitComa(parser.getArgs()[0]));
		std::map<std::string, Users*>::iterator it;
		for (size_t i = 0; i < ulist.size(); i++)
		{
			it = userPointer.find(ulist[i]);
			if (it == userPointer.end())
				send(user.getFd(), parser.SendReply("401", ulist[i], "User you try to communicate with doesn't exists"), parser.getReply().size(), 0);
			else if(it->second->getNickname() == user.getNickname())
				continue;
			else if (it->second->getAway() == true)
				send(user.getFd(), parser.SendReply("301", user.getNickname() + " " + it->second->getNickname(), it->second->getAwayMsg()), parser.getReply().size(), 0);
			else
				send(it->second->getFd(), parser.SendReply("PRIVMSG", user.getNickname(), user.getNickname() + " " + parser.getMessage()), parser.getReply().size(), 0);
		}
	}
	return (0);
}