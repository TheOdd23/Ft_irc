#include "../Servers.hpp"

int Servers::Topic(Users &user, Parser &parser) {
  switch (parser.getArgs().size()) {
  case 0:
    send(user.getFd(),
         parser.SendReply("461", parser.getCommand(), "Not enough parameters"),
         parser.getReply().size(), 0);
    break;
  case 1:
    if (this->Chans.find(parser.getArgs().at(0)) != this->Chans.end())
      if (this->Chans.at(parser.getArgs().at(0)).getTopic().empty() == false)
        send(user.getFd(),
             parser.SendReply(
                 "332", this->Chans.at(parser.getArgs().at(0)).getName(),
                 this->Chans.at(parser.getArgs().at(0)).getTopic()),
             parser.getReply().size(), 0);
      else
        send(user.getFd(),
             parser.SendReply("331",
                              this->Chans.at(parser.getArgs().at(0)).getName(),
                              "No topic is set"),
             parser.getReply().size(), 0);
    else
      send(user.getFd(),
           parser.SendReply("403", parser.getArgs().at(0), "No such channel"),
           parser.getReply().size(), 0);
    break;
  default:
    if (this->Chans.find(parser.getArgs().at(0)) != this->Chans.end()) {
      if (user.getChannels().find(this->Chans.at(parser.getArgs()[0]).getName()) == user.getChannels().end())
      {
        send(user.getFd(),
            parser.SendReply("442", parser.getArgs().at(0), "You are not on channel, cannot change topic"),
            parser.getReply().size(), 0);
      }
      if ((this->Chans.at(parser.getArgs().at(0)).getFlag(T)) == 0) {
        string topic = parser.getArgs().at(1);
        for (size_t i = 2; i < parser.getArgs().size(); i++)
          topic += parser.getArgs().at(i);
        this->Chans.at(parser.getArgs().at(0)).setTopic(topic);
        send(user.getFd(),
             parser.SendReply("332", parser.getArgs().at(0),
                              parser.getArgs().at(1)),
             parser.getReply().size(), 0);
      } else {
        if (this->Chans.at(parser.getArgs().at(0)).getOp(user.getNickname()) ==
            true) {
          string topic = parser.getArgs().at(1);
          for (size_t i = 2; i < parser.getArgs().size(); i++)
            topic += parser.getArgs().at(i);
          this->Chans.at(parser.getArgs().at(0)).setTopic(topic);
          send(user.getFd(),
             parser.SendReply("332", parser.getArgs().at(0),
                              parser.getArgs().at(1)),
             parser.getReply().size(), 0);
        } else
          send(user.getFd(),
               parser.SendReply(
                   "482", this->Chans.at(parser.getArgs().at(0)).getName(),
                   "You're not channel operator"),
               parser.getReply().size(), 0);
      }
    } else
      send(user.getFd(),
           parser.SendReply("403", parser.getArgs().at(0), "No such channel"),
           parser.getReply().size(), 0);
    break;
  }

  return (0);
}