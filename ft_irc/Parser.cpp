#include "Parser.hpp"

Parser::Parser() {}

Parser::Parser(char *buff)
{
    std::string line(buff);
    _server_name = IRC;
    ParseCommand(line);
}

Parser::Parser(std::string line)
{
    _server_name = IRC;
    ParseCommand(line);
}


Parser::Parser(const Parser &other) : _command(other.getCommand()), _my_args(other.getArgs()), _prefix(other.getPrefix()), 
        _message(other.getMessage()), _server_name(other._server_name)
{

}

Parser::~Parser()
{

}

Parser  &Parser::operator=(const Parser &other)
{
    _command = other.getCommand();
    _message = other.getMessage();
    _my_args = other.getArgs();
    _prefix = other.getPrefix();
    return (*this);
}

void    Parser::ParseCommand(std::string line)
{
    unsigned long pos = 0;
    bool first = true;

    while (line[0] == ' ') //Eliminates all spaces
            line.erase(line.begin());
    if (line.size() == 0)
        return ; 
    while (pos != line.npos && line.size() > 0)
    {
        while (line[0] == ' ') //Eliminates all spaces inbetween strings
            line.erase(line.begin());
        if (line[0] == ':' && first == false) //End Message management 
        {
            _message = line;
            _message.erase(_message.begin());
            line.erase(line.end() - 1);
            line.erase(line.end() - 1);
            break ;
        }
        pos = line.find(' '); //Checks for next space
        if (pos == line.npos) 
        {
            line.erase(line.end() - 1);
            line.erase(line.end() - 1);
            _my_args.push_back(line);
            break;
        }
        _my_args.push_back(line.substr(0, pos));
        line = line.substr(pos + 1, line.size());
        first = false;
    }
    if (_my_args.size() > 0 && _my_args[0][0] == ':')
    {
        _prefix = _my_args[0];
        _prefix.erase(_prefix.begin());
        _my_args.erase(_my_args.begin());
    }
    _command = _my_args[0];
    _my_args.erase(_my_args.begin());
    if ( _my_args.size() > 0 && (_my_args[_my_args.size() - 1][0] == ' ' || _my_args[_my_args.size() - 1][0] == '\r' 
            || _my_args[_my_args.size() - 1].size() == 0))
    {
        _my_args.pop_back();
    }
}

const char    *Parser::SendReply(const std::string code, 
                const std::string args, const std::string message) 
{
    _reply.clear();
    _reply = ":" + _server_name + ".42.qc" ;
    // _reply = ":" + _server_name + ".42.qc" + code + " " + args + " :" + message + "\r\n\r\n";
    if (code.size())
        _reply += " " + code;
    if (args.size())
        _reply += " " + args;
    if (message.size())
        _reply += " :" + message;
    _reply += "\r\n\n";

    if (_reply.size() > 512)
        return NULL;
    std::cout << "Reply sent : " << _reply << std::endl;
    return (_reply.c_str());
}

//CODE Pints every elements of the Parser
void    Parser::PrintElements(void)
{
    std::cout << MAG;
    if (_prefix.length() > 0)
        std::cout << "Prefix: " << _prefix << std::endl; 
    std::cout <<  "CMD: " << _command << std::endl;
    // for (size_t i = 0; i < _command.size(); i++)
    // {
    //     std::cout << "-" << static_cast<int>(_command[i]) ;
    // }
    // std::cout << std::endl;
    if (_my_args.size() > 0)
    {
        std::vector<std::string>::iterator it = _my_args.begin();
        std::vector<std::string>::iterator ite = _my_args.end();
        std::cout << "ARGS " ;
        for (;it != ite; it++)
        {
            std::cout << " - " << *it << " ";
            for (size_t i = 0; i < (*it).size(); i++)
            {
                std::cout << "-" << static_cast<int>((*it)[i]) ;
            }
        }   
    }
    std::cout << std::endl << "Size of args is : " << _my_args.size() << std::endl;;
    if (_message.length() > 0)
        std::cout <<  "Message: " << _message << std::endl;
    std::cout << CLEAR << std::endl;
}

std::vector<std::string>    Parser::SplitComa(std::string line)
{
    std::vector<std::string> temp;
    size_t pos = 0;

    if (line.size() == 0)
        return temp;
    while (pos != line.npos && line.size() > 0)
    {
        pos = line.find(',');
        if (pos == line.npos)
        {
            temp.push_back(line);
            break;
        }
        temp.push_back(line.substr(0, pos));
        line = line.substr(pos + 1, line.size());
    }
    return temp;
}

const std::vector<std::string>  &Parser::getArgs() const
{
    return (_my_args);
}

const std::string               &Parser::getCommand() const
{
    return (_command);
}

const std::string               &Parser::getPrefix() const
{
    return (_prefix);
}

const std::string               &Parser::getMessage() const
{
    return (_message);
}

const std::string                 &Parser::getReply() const
{
    return (_reply);
}