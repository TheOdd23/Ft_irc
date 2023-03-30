#ifndef PARSER_HPP
#define PARSER_HPP

#pragma once

#include <vector>
#include <string>
#include <iostream>
#include "colours.hpp"

#define IRC "127.0.0.1"

class Parser
{
public:
    Parser();
    Parser(char *buff);
    Parser(std::string str);
    Parser(const Parser &other);

    Parser  &operator=(const Parser &other);
    ~Parser();

    void                        PrintElements();
    std::vector<std::string>    SplitComa(std::string args);

    const std::vector<std::string>    &getArgs() const;
    const std::string                 &getCommand() const;
    const std::string                 &getPrefix() const;
    const std::string                 &getMessage() const;
    const std::string                 &getReply() const;
    const char                              *SendReply(const std::string code, const std::string args, const std::string message) ;
    
private:
    std::string                 _command;
    std::vector<std::string>    _my_args;
    std::string                 _prefix;
    std::string                 _message;
    std::string                 _server_name;
    std::string                 _reply;

    //Private Methods
    void    ParseCommand(std::string line);
};

#endif