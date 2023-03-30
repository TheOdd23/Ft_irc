#ifndef USERS_HPP
#define USERS_HPP

#pragma once
#include <map>
#include <string>
#include <set>
#include "Timer.hpp"

class Users
{
    public:
        Users();
        Users(std::string nname, std::string uname, std::string realname, bool passw, bool reg);
        Users(const Users &other);
        const Users   &operator=(const Users &other);
        ~Users();

        //Getters
        int                             getFd() const;
        const std::string               &getNickname() const;
        const std::string               &getUsername() const;
        const std::string               &getHostname() const;
        const std::string               &getRealname() const;
        const std::set<std::string>     &getChannels() const;
        bool                            getPass() const;
        bool                            getRegStat() const;
        bool                            getAway() const;
        const std::string               &getBuffer() const;
        const std::string               &getAwayMsg() const;
        bool                            getActive() const;
        
        //Setters
        void    setFd(int fdesc);
        void    setNickname(const std::string &name);
        void    setUserName(const std::string &name);
        void    setHostName(const std::string &name);
        void    setRealName(const std::string &name);
        void    setPass(bool passw);
        void    setRegistration(bool reg);
        void    setBuffer(std::string str);
        void    setAway(bool stat);
        void    setAwayMsg(std::string msg);
        void    setActive(bool status);
        void    addChannel(const std::string &chan_name);
        void    removeChannel(const std::string &chan_name);

        //User Timer Class
        Timer                           timer;      //Tracks User interaction last time with server

    private:
        int                             fd;
        std::string                     Nickname;
        std::string                     Username;
        std::string                     Hostname;
        std::string                     Realname;
        std::string                     AwayMsg;
        bool                            Pass;
        bool                            Registration;
        bool                            is_Away;
        std::string                     Buffer;     //Residues of last command
        std::set<std::string>           Channels;
        bool                            active;     //Is active when receives PONG
};

#endif
