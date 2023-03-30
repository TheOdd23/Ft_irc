#include "../Servers.hpp"

int Servers::Join(Users &user, Parser &parser) {
  std::string null;
  switch (parser.getArgs().size()) {
  case 1: // only channel no password
  {
    std::vector<std::string> channels =
        parser.SplitComa(parser.getArgs().at(0));
    for (size_t i = 0; i < channels.size(); i++) {
      std::map<std::string, Channels>::iterator it =
          this->Chans.find(channels.at(i));
      if (it != this->Chans.end()) {
        user.addChannel(it->second.getName());
        switch (it->second.addUser(user, null, false)) {
        case 1:
          send(user.getFd(),
               parser.SendReply("475", channels.at(i),
                                "Cannot join channel (+k)"),
               parser.getReply().size(), 0);
          user.removeChannel(it->second.getName());
          break;
        case 2:
          send(user.getFd(),
               parser.SendReply("471", channels.at(i),
                                "Cannot join channel (+l)"),
               parser.getReply().size(), 0);
               user.removeChannel(it->second.getName());
          break;
        case 3:
          send(user.getFd(),
               parser.SendReply("474", channels.at(i),
                                "Cannot join channel (+b)"),
               parser.getReply().size(), 0);
               user.removeChannel(it->second.getName());
          break;
        case 4:
          send(user.getFd(),
               parser.SendReply("473", channels.at(i),
                                "Cannot join channel (+i)"),
               parser.getReply().size(), 0);
               user.removeChannel(it->second.getName());
          break;
        default:
          it->second.setOp(user.getNickname(), false);
          if (!this->Chans.at(channels.at(i)).getTopic().empty())
            send(user.getFd(),
                 parser.SendReply("332", channels.at(i),
                                  this->Chans.at(channels.at(i)).getTopic()),
                 parser.getReply().size(), 0);
            std::string msg = ":" + user.getNickname() + "!" + user.getUsername() + "@" + user.getHostname() + " JOIN " + it->second.getName() + "\n";
            send(user.getFd(), msg.c_str(), msg.size(), 0);
          break;
        }
      } else {
        if (channels.at(i).size() <= 200 &&
            channels.at(i).find(7, 0) == channels.at(i).npos &&
            (channels.at(0).at(0) == '#' || channels.at(0).at(0) == '&')) {
          Channels hold(channels.at(i));
          this->Chans.insert(
              std::pair<std::string, Channels>(channels.at(i), hold));
          this->Chans.at(channels.at(i)).addUser(user, null, false);
          this->Chans.at(channels.at(i)).setOp(user.getNickname(), true);
          user.addChannel(channels.at(i));
          std::string msg = ":" + user.getNickname() + "!" + user.getUsername() + "@" + user.getHostname() + " JOIN " + channels.at(i) + "\n";
          send(user.getFd(), msg.c_str(), msg.size(), 0);
        }
      }
    }
    break;
  }
  case 2: // check if passwords are used
  {
    std::vector<std::string> channels =
        parser.SplitComa(parser.getArgs().at(0));
    std::vector<std::string> password =
        parser.SplitComa(parser.getArgs().at(1));
    for (size_t i = 0; i < channels.size(); i++) {
      std::map<std::string, Channels>::iterator it =
          this->Chans.find(channels.at(i));
      if (it != this->Chans.end()) {
        user.addChannel(it->second.getName());
        switch (it->second.addUser(user, parser.getArgs().at(1), false)) {
        case 1:
          send(user.getFd(),
               parser.SendReply("475", channels.at(i),
                                "Cannot join channel (+k)"),
               parser.getReply().size(), 0);
               user.removeChannel(it->second.getName());
          break;
        case 2:
          send(user.getFd(),
               parser.SendReply("471", channels.at(i),
                                "Cannot join channel (+l)"),
               parser.getReply().size(), 0);
               user.removeChannel(it->second.getName());
          break;
        case 3:
          send(user.getFd(),
               parser.SendReply("474", channels.at(i),
                                "Cannot join channel (+b)"),
               parser.getReply().size(), 0);
               user.removeChannel(it->second.getName());
          break;
        case 4:
          send(user.getFd(),
               parser.SendReply("473", channels.at(i),
                                "Cannot join channel (+i)"),
               parser.getReply().size(), 0);
               user.removeChannel(it->second.getName());
          break;
        default:
          it->second.setOp(user.getNickname(), false);
          if (!this->Chans.at(channels.at(i)).getTopic().empty())
            send(user.getFd(),
                 parser.SendReply("332", channels.at(i),
                                  this->Chans.at(channels.at(i)).getTopic()),
                 parser.getReply().size(), 0);
          std::string msg = ":" + user.getNickname() + "!" + user.getUsername() + "@" + user.getHostname() + " JOIN " + it->second.getName() + "\n";
          send(user.getFd(), msg.c_str(), msg.size(), 0);
          break;
        }
      } else {
        if (channels.at(i).size() <= 200 &&
            channels.at(i).find(7, 0) == channels.at(i).npos &&
            (channels.at(0).at(0) == '#' || channels.at(0).at(0) == '&')) {
          Channels hold(channels.at(i));
          this->Chans.insert(
              std::pair<std::string, Channels>(channels.at(i), hold));
          user.addChannel(channels.at(i));
          this->Chans.at(channels.at(i)).addUser(user, null, false);
          this->Chans.at(channels.at(i)).setOp(user.getNickname(), true);
          std::string msg = ":" + user.getNickname() + "!" + user.getUsername() + "@" + user.getHostname() + " JOIN " + channels.at(i) + "\n";
          send(user.getFd(), msg.c_str(), msg.size(), 0);
        }
      }
    }
    break;
  }
  }
  return (0);
}
