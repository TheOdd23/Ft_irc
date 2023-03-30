#include "Servers.hpp"

void    compress_function(struct pollfd *fds, int nfds);

Servers::Servers(size_t pt, std::string pw)
{
    _server_data.Port = pt;
    _server_data.Password = pw;
    CommandInit();
    ServerInit();

	std::cout << "Welcome, it is I, your server" << std::endl;
}

Servers::~Servers()
{
	std::cout << "Get the F out! This ain't 4chan" << std::endl;
}

void Servers::CommandInit()
{
    _command_map["PASS"] = &Servers::Pass;
    _command_map["NICK"] = &Servers::Nick;
    _command_map["USER"] = &Servers::User;
    _command_map["AWAY"] = &Servers::Away;
    _command_map["INVITE"] = &Servers::Invite;
    _command_map["JOIN"] = &Servers::Join;
    _command_map["KICK"] = &Servers::Kick;
    _command_map["LIST"] = &Servers::List;
    _command_map["MODE"] = &Servers::Mode;
    _command_map["NAMES"] = &Servers::Names;
    _command_map["NOTICE"] = &Servers::Notice;
    _command_map["PART"] = &Servers::Part;
    _command_map["PING"] = &Servers::Ping;
    _command_map["PONG"] = &Servers::Pong;
    _command_map["PRIVMSG"] = &Servers::Privmsg;
    _command_map["QUIT"] = &Servers::Quit;
    _command_map["TOPIC"] = &Servers::Topic;
    _command_map["SHUTDOWN"] = &Servers::Shutdown;
}

// [SERVER SOCKET CREATION] : Open server socket and preps it 
void Servers::ServerInit()    
{
    int yes = 1;
    int r_bind, r_listen;

    _server_data.address.sin_family = AF_INET; //Address family for internet protocols (for TCP)
    _server_data.address.sin_port = htons(_server_data.Port) ; // Contains the port number
    _server_data.address.sin_addr.s_addr = INADDR_ANY; // Use when we don't want to bind a socket to any specific IP. For example if we don't know the IP. 

    // SOCKET CREATE: 1.Returns fd 2. PF_INET or PF_INET6 protocol internet for IPV4 or IPV6 
        // 3.Sock_stream : default for TCP usage 4. Protocol value for IP = 0 
    _server_data.server_fd = socket(PF_INET, SOCK_STREAM, 0);
    _server_data.nfds = 1; //First socket is now open

    // SETSOCKETOPT : Set options for the specified socket. enable to reuse socket address. Prevent error such as "Address already in use" 
        // 1. SOL_SOCKET is default to manipulate options at the socket level 2. SO_REUSEADDR is an option to be able to reuse socket
        // 3. option_value and option len (NULL, 0), is a buffer of len size that returns the value of the option
    if (setsockopt(_server_data.server_fd, SOL_SOCKET, SO_REUSEADDR,  &yes, sizeof(int)) < 0)
        throw std::runtime_error("Setsockopt failed");

    // FCNTL : Enables the descriptor to be in NON-BLOCKING MODE
    fcntl(_server_data.server_fd, F_SETFL, O_NONBLOCK);
  
    //BIND : binds the socket to the address and port number specified in the sockaddr_in struct
    r_bind = bind(_server_data.server_fd, (struct sockaddr *)&_server_data.address, sizeof( _server_data.address));

    //  [LISTENING] Defines the sockets to listen to, and max amount of pendind requests
    r_listen = listen(_server_data.server_fd, SOMAXCONN);

    // Prepare poll_fd struct, which tracks all open sockets
    memset(_server_data.poll_fd, 0 , sizeof(_server_data.poll_fd)); //Initialize this array to 0
    _server_data.poll_fd[0].fd = _server_data.server_fd; // Assign Server socket as the first fd;
    _server_data.poll_fd[0].events = POLLIN; // Checks if data may be read without blocking
}

void    Servers::start()
{
    _compression = false;
    _end_server = false;

    while (_end_server == false) //Loops and runs the poll function 
    {
         std::cout << "Waiting for poll motherfucker ... ! : " << std::endl;
        if (poll(_server_data.poll_fd, _server_data.nfds, COUNTDOWN) < 0) // Poll checks sockets for incoming messages
        {
            std::cerr << "Poll is not working man ! :( I'm CRASSSSSHHHING ... \n" ;
            break;
        }
        //Checks in socket array for changes 
        TrackingFd(); 
        
        if (_compression) //If a socket has been close, compression is execute
        {
            compress_function(_server_data.poll_fd, _server_data.nfds);
            _compression = false;
        }
        std::cout << "Fds count: " YEL << _server_data.nfds << CLEAR " User count :" MAG << usersMap.size() << CLEAR << std::endl;

    }
    CloseAll(); //Close server file descriptor, need to close the others with a function probable.
    
    return ;
}

//Loops through the sockets for changes
int Servers::TrackingFd()
{
    for (int i = 0; i < _server_data.nfds; i++) //Loops throught all sockets
    {
        if (_server_data.poll_fd[i].revents == 0) //If socket has no changes... go to next one;
        {
            if(i != 0)
                CheckClient(usersMap[_server_data.poll_fd[i].fd], i);
            continue;
        }
        if (_server_data.poll_fd[i].revents != POLLIN) //if error in socket received from socket, close socket
        {
            std::cout << "POll_fd[" << i << "] has revents : " MAG <<  _server_data.poll_fd[i].revents << CLEAR << std::endl;
            // if (_server_data.poll_fd[i].revents == 17)
            // {
            //     CloseSocket(i);
            //     continue;
            // }
            CloseSocket(i);
        }
        else if (_server_data.poll_fd[i].fd == _server_data.server_fd)// If server socket has a change, accept new connection
            AcceptConnection();
        else                    //if any active other socket has a change, then receive data
            ReceiveData(usersMap[_server_data.poll_fd[i].fd]);
        
        if (_close_connection == true) 
            CloseSocket(i);
    }
    return 0;
}

 // [ACCEPT] : Accepts a new connection
void    Servers::AcceptConnection()
{
    std::cout << "New connection is requested" <<  std::endl;
    struct sockaddr_in client;
    socklen_t addre_len = sizeof(client);

    int new_sd = 0;
    while (new_sd != -1)
    {
        new_sd = accept(_server_data.server_fd, (struct sockaddr *)(&client) , &addre_len); //return a socket description of new connection socket
        if (new_sd < 0)
        {
            if (errno != EWOULDBLOCK) //If accept encunters a fatal error, server stops
            {
                perror("Acceptance of new connection has failed !");  
                _end_server = true;
            }
            return;
        }
        std::cout << "CLIENT IP ADDRESS: " << inet_ntoa(client.sin_addr) << std::endl;
        _server_data.poll_fd[_server_data.nfds].fd = new_sd;
        _server_data.poll_fd[_server_data.nfds].events = POLLIN;
        _server_data.nfds++;
        usersMap[new_sd];
        usersMap[new_sd].setHostName(inet_ntoa(client.sin_addr));
        usersMap[new_sd].setFd(new_sd);
        usersMap[new_sd].timer.Start();
        usersMap[new_sd].setActive(true);
        std::cout << " New connection has been added to descriptor: " << new_sd << std::endl;
    }
    return ;
}

// [RECEIVE DATA] : Receives the data, adds it in the buffer and calls for the buffer management
void    Servers::ReceiveData(Users &user)
{
    _close_connection = false;
    char buff[512];
    int result = 666;
    user.setActive(true);

    memset(buff, 0, sizeof(buff));
    while (result > 0)
    {
        result = recv(user.getFd(), buff, sizeof(buff), 0);
        if (result < 0)
        {
            if (errno != EWOULDBLOCK)
            {
                perror("Socket reception has failed, massive crash, connection will be flushed ! Bye loser XD");
                _close_connection = true;
                return ;
            }
            if (result == 0)
            {
                _close_connection = true;
                break;
            }
        }
    }
    std::string str(buff);
    user.setBuffer(user.getBuffer() + str);
    ManageUserBuffer(user);
    user.timer.Start();
}

//
void    Servers::ManageUserBuffer(Users &user)
{
    size_t pos = user.getBuffer().find("\r\n");
    while (pos != std::string::npos || user.getBuffer().size() >= 512)
    {
        if (user.getBuffer().size() >= 512)
        {
            std::string temp = user.getBuffer().substr(0, 510) + "\r\n";
            user.setBuffer(user.getBuffer().substr(511, user.getBuffer().size() - 510));
            std::cout << "CMD line too big: " << temp << std::endl;
            ExecuteCmd(user, temp);
        }
        else if (pos != std::string::npos)
        {
            std::string temp = user.getBuffer().substr(0, pos + 2);
            std::cout << "CMD line pos found: " << temp << std::endl;
            ExecuteCmd(user, temp);
            user.setBuffer(user.getBuffer().substr(pos + 2, user.getBuffer().size() - (pos + 2)));
        }
        pos = user.getBuffer().find("\r\n");
    }
    if (user.getBuffer().size() > 0)
        std::cout << user.getNickname() << " buffer leftover :" << user.getBuffer() << std::endl;
}

// [EXECUTE COMMAND]
void    Servers::ExecuteCmd(Users &user, std::string &cmd_line)
{
    //PARSING:
    Parser parser(cmd_line);

    parser.PrintElements();
    if (user.getRegStat() == false && (parser.getCommand() != "NICK" && parser.getCommand() != "USER" 
            && parser.getCommand() != "PASS"))
    {
        _close_connection = true;
        std::cout <<RED "NON validated : " CLEAR << parser.getCommand() << std::endl;
        return ;
    }

    //EXECUTE CMD:
    std::map<std::string, fct>::iterator it = _command_map.find(parser.getCommand()); //Looks for iterator pointing to Command function
    if (it != _command_map.end())  //If command exists
        (this->*(it->second))(user, parser); // send the reference of existing user
    else
    {
        send(user.getFd(), parser.SendReply("421", parser.getCommand(), "This command doesn't exist"), parser.getReply().size(), 0);
        std::cout << "Non-existing command taken" << std::endl;
    }

    return ;
}

void    Servers::CloseSocket(int i)
{
    DeleteUsers(usersMap[_server_data.poll_fd[i].fd]);
    close(_server_data.poll_fd[i].fd);
    _server_data.poll_fd[i].fd = -1;
    _server_data.poll_fd[i].revents = 0;
    _server_data.poll_fd[i].events = 0;

    _server_data.nfds--;
    _compression = true;
    if (_server_data.poll_fd[0].revents == 32) //Server Timeout
        _end_server = true;
    _close_connection = false;
    return ;
}

void    Servers::DeleteUsers(Users &user)
{
    userPointer.erase(user.getNickname());
    Username_list.erase(user.getUsername());
    
    //ERASES user from channel 
    std::set<std::string> userChannels = user.getChannels();
    std::set<std::string>::iterator it = userChannels.begin();
    for (; it != userChannels.end(); it++)
    {
        Chans[(*it)].RemoveUser(user.getNickname());
        if (Chans[(*it)].getUsers().size() == 0) // IF channel has no users -> delete channel
            Chans.erase(*it);
    }
    usersMap.erase(user.getFd());
    return ;
}

void    Servers::CheckClient(Users &user, int i)
{
    if (user.getActive() && user.timer.Timing() > IDLE_TIME)
    {
        std::cout << BBLU << user.getNickname() << BLU " Has beeen set to inactive " CLEAR << std::endl;
        Parser empty;
        (this->*(_command_map["PING"]))(user, empty);
        user.timer.Start();
    }
    else if (user.getActive() == false && user.timer.Timing() > KILL_TIME)
    {
        std::cout << BRED << user.getNickname() << RED " Has been kickout due to inactivity " CLEAR << std::endl;
        CloseSocket(i);
    }
}

void    Servers::CloseAll()
{
    int i = 0;
    while (_server_data.poll_fd[i].fd > 0)
    {
        if (i == 0)
            close(_server_data.server_fd);
        else
            close(_server_data.poll_fd[i].fd);
        i++;
    }
}