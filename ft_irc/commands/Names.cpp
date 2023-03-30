#include "../Servers.hpp"

int	Servers::Names(Users &user, Parser &parser)
{
	if (parser.getArgs().size() > 0)
	{
		NamesDefine(user, parser);
		return (0);
	}
	std::map<std::string, Channels>::iterator it = Chans.begin();
	std::map<std::string, Channels>::iterator ite = Chans.end();
	std::set<std::string> tempList;
	for (; it != ite; it++)
	{
		if ( UsersIsSubscribe(it->second.getName(), user) || (!it->second.getFlag(P) && !it->second.getFlag(S)))
		{
			std::string message;
			std::vector<Users> list_users = it->second.getUsers();
			for (unsigned int i = 0; i < list_users.size(); i++)
			{
				if (i != 0)
					message += " ";
				message += list_users[i].getNickname();
				tempList.insert(list_users[i].getNickname());
			}
			while (message.size())
			{
				if (message.size() > 510)
				{
					send(user.getFd(), parser.SendReply("353", user.getNickname() + " = " + it->second.getName(), message.substr(0, 510)), parser.getReply().size(), 0);
					message = message.substr(0, 510);
				}
				else
				{
					send(user.getFd(), parser.SendReply("353", user.getNickname() + " = " + it->second.getName(), message), parser.getReply().size(), 0);
					break ; 
				}
			}
		}
	}
	std::map<int, Users>::iterator uit = usersMap.begin();
	std::map<int, Users>::iterator uite = usersMap.end();
	std::string other_users;
	bool oneuser = false;
	for (; uit != uite; uit++)
	{
		if (tempList.end() == tempList.find(uit->second.getNickname()))
		{
			if (other_users.size())
				other_users += " ";
			other_users += uit->second.getNickname();
			oneuser = true;
		}
	}
	if (oneuser)
	{
			while (other_users.size())
			{
				if (other_users.size() > 510)
				{
					send(user.getFd(), parser.SendReply("353", user.getNickname() + " = " + "*", other_users.substr(0, 510)), parser.getReply().size(), 0);
					other_users = other_users.substr(0, 510);
				}
				else
				{
					send(user.getFd(), parser.SendReply("353", user.getNickname() + " = " + "*", other_users), parser.getReply().size(), 0);
					break ; 
				}
			}
	}
	send(user.getFd(), parser.SendReply("366", "*", "End of line"), parser.getReply().size(), 0);
	return (0);
}

//IF specific channels are entered as arguemnts
int     Servers::NamesDefine(Users &user, Parser &parser) 
{
	std::vector<std::string> comaChannels = parser.SplitComa(parser.getArgs()[0]);

	for (size_t i = 0; i < comaChannels.size(); i++)
	{
		if (UsersIsSubscribe(comaChannels[i], user) || (!Chans[comaChannels[i]].getFlag(P) && !Chans[comaChannels[i]].getFlag(S)))
		{
			SendChannelUsers(Chans[comaChannels[i]].getUsers(), user, parser, Chans[comaChannels[i]].getName());
		}
	}
	return (0);
}

bool	Servers::UsersIsSubscribe(std::string channel, Users &user)
{
	std::set<string> user_channels = user.getChannels();
	
	if (user_channels.find(channel) == user_channels.end())
	{
		return false;
	}
	return true;	
}

void	Servers::SendChannelUsers(std::vector<Users> list_users, Users &user, Parser &parser, std::string channel_name)
{
	std::string message;
	for (unsigned int i = 0; i < list_users.size(); i++)
	{
		if (i != 0)
			message += " ";
		message += list_users[i].getNickname();
	}
	while (message.size())
	{
		if (message.size() > 510)
		{
			send(user.getFd(), parser.SendReply("353", user.getNickname() + " = " + channel_name, message.substr(0, 510)), parser.getReply().size(), 0);
			message = message.substr(0, 510);
		}
		else
		{
			send(user.getFd(), parser.SendReply("353", user.getNickname() + " = " + channel_name, message), parser.getReply().size(), 0);
			break ; 
		}
	}
	send(user.getFd(), parser.SendReply("366", user.getNickname() + " = " + "*", "End of line"), parser.getReply().size(), 0);
}