#include "Users.hpp"
#include "colours.hpp"
#include <iostream>

Users::Users(): Pass(false), Registration(false)
{

}

Users::Users(const std::string nname, const std::string uname, const std::string realname, bool passw, bool reg): Nickname(nname), Username(uname), Realname(realname), Pass(passw), Registration(reg){}

Users::Users(const Users &other)
{
	*this = other;
}

Users::~Users(){Pass = false; Registration = false;}

const Users   &Users::operator=(const Users &other)
{
	// std::cout << BLU "assign operator called" CLEAR<< std::endl;
	fd = other.getFd();
	Nickname = other.getNickname();
	Username = other.getUsername();
	Realname = other.getRealname();
	Pass = other.getPass();
	Registration = other.getRegStat();
	is_Away = other.getAway();
	Channels = other.getChannels();
	active = other.active;
	return *this;
}

int	Users::getFd() const
{
	return fd;
}
const std::string	&Users::getNickname() const
{
	return Nickname;
}
const std::string	&Users::getUsername() const
{
	return Username;
}
const std::string	&Users::getRealname() const
{
	return Realname;
}

const std::string	&Users::getHostname() const
{
	return Hostname;
}

const std::set<std::string>     &Users::getChannels() const
{
	return Channels;
}

bool	Users::getPass() const
{
	return Pass;
}

bool	Users::getRegStat() const
{
	return Registration;
}

const std::string	&Users::getBuffer() const
{
	return (Buffer);
}

bool                Users::getActive() const
{
	return (active);
}

//SETTERS

void	Users::setFd(int fdesc)
{
	fd = fdesc;
}
void	Users::setNickname(const std::string &name)
{
	Nickname = name;
}
void	Users::setHostName(const std::string &name)
{
	Hostname = name;
}
void	Users::setUserName(const std::string &name)
{
	Username = name;
}
void	Users::setRealName(const std::string &name)
{
	Realname = name;
}
void	Users::setPass(bool passw)
{
	Pass = passw;
}
void	Users::setRegistration(bool reg)
{
	Registration = reg;
}

void    Users::setBuffer(std::string str)
{
	Buffer = str;
}

bool	Users::getAway() const
{
	return is_Away;
}

void	Users::setAway(bool stat)
{
	is_Away = stat;
}

const std::string	&Users::getAwayMsg() const
{
	return AwayMsg;
}

void	Users::setAwayMsg(std::string msg)
{
	AwayMsg = msg;
}

void	Users::setActive(bool status)
{
	active = status;
}

void    Users::addChannel(const std::string &chan_name)
{
	Channels.insert(chan_name);
}

void    Users::removeChannel(const std::string &chan_name)
{
	Channels.erase(chan_name);
}