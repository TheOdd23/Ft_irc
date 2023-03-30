#include "../Servers.hpp"

bool    ft_isgoodchar(std::string nick)
{
    for(std::string::iterator it = nick.begin(); it != nick.end(); it++)
    {
        if (it == nick.end())
            return true;
        if (!isalnum(*it) && *it != '-' && *it != '[' &&
            *it != ']' && *it != '_' && *it != '{' &&
            *it != '}' && *it != '\\' && *it != '`' && *it != '|')
            return false;
    }
    return true;
}

int     Servers::Nick(Users &user, Parser &parser)
{
    if (user.getPass() == false)
    {
            send(user.getFd(), parser.SendReply("464", "PASS", "No password entered, please enter password"), parser.getReply().size(), 0);
            _close_connection = true;
            return (1);
    }
    if (parser.getArgs().empty())
    {
            if (user.getRegStat() == false)
            {
                send(user.getFd(), parser.SendReply("431", "*", "Nickname empty, please enter valid Nickname"), parser.getReply().size(), 0);
                _close_connection = true;
            }
            else
                send(user.getFd(), parser.SendReply("431", user.getNickname(), "Nickname empty, please enter valid Nickname"), parser.getReply().size(), 0);
            return (1);
    }
    if (parser.getArgs()[0].length() > 9 || !ft_isgoodchar(parser.getArgs()[0]))
    {
            if (user.getRegStat() == false)
            {
                send(user.getFd(), parser.SendReply("432", parser.getArgs()[0], "Nickname invalid, please enter valid Nickname"), parser.getReply().size(), 0);
                _close_connection = true;
            }
            else
                send(user.getFd(), parser.SendReply("432", user.getNickname() + " " + parser.getArgs()[0], "Nickname invalid, please enter valid Nickname"), parser.getReply().size(), 0);
            return (1);
    }
    if (userPointer.find(parser.getArgs()[0]) != userPointer.end())
    {
            if (user.getRegStat() == false)
            {
                send(user.getFd(), parser.SendReply("433", parser.getArgs()[0], "Nickname already in use, please enter another valid Nickname"), parser.getReply().size(), 0);
                _close_connection = true;
            }
            else
                send(user.getFd(), parser.SendReply("433", user.getNickname() + " " + parser.getArgs()[0], "Nickname already in use, please enter valid Nickname"), parser.getReply().size(), 0);
            return (1);
    }
    if (user.getRegStat() == true)
    {
        for (std::set<string>::iterator it = user.getChannels().begin(); it != user.getChannels().end(); it++)
        {
            for (std::vector<Users>::iterator uit = Chans.find(*it)->second.getUsers().begin(); uit != Chans.find(*it)->second.getUsers().end(); uit++)
            {
                if (uit->getNickname() == user.getNickname())
                    uit->setNickname(parser.getArgs()[0]);
            }
        }
        userPointer.erase(user.getNickname());
    }
    user.setNickname(parser.getArgs()[0]);
    userPointer.insert(std::make_pair(parser.getArgs()[0], &user));
    send(user.getFd(), parser.SendReply("001", parser.getArgs()[0], "Nickname valid"), parser.getReply().size(), 0);
    return (0);
}
