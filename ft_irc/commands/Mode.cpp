#include "../Servers.hpp"

/*	o - give/take channel operator privileges;
    p - private channel flag;
    s - secret channel flag;
    i - invite-only channel flag;
    t - topic settable by channel operator only flag;
    n - no messages to channel from clients on the outside;
    m - moderated channel;
    l - set the user limit to channel;
    b - set a ban mask to keep users out;
    v - give/take the ability to speak on a moderated channel;
    k - set a channel key (password). */

std::string bool_to_string(const bool &flag) { return flag ? "true" : "false"; }

int Servers::Mode(Users &user, Parser &parser) {
  if (parser.getArgs().size() > 0) {
    if (this->Chans.find(parser.getArgs().at(0)) != this->Chans.end()) {
      if (this->Chans.at(parser.getArgs().at(0)).is_user(user.getNickname()) ==
          true) {
        if (this->Chans.at(parser.getArgs().at(0)).getOp(user.getNickname()) ==
            true) {
          bool flag;
          int offset;
          std::string pass;
          std::string us;
          int limit;

          switch (parser.getArgs().size()) {
          case 5:
            pass = parser.getArgs().at(4);
          case 4:
            if (pass.empty())
              pass = parser.getArgs().at(3);
            if (us.empty())
              us = parser.getArgs().at(3);
          case 3:
            if (us.empty())
              us = parser.getArgs().at(2);
            if (pass.empty())
              pass = parser.getArgs().at(2);
            try {
              std::cout << parser.getArgs().at(2);
              limit = stoi(parser.getArgs().at(2));
            } catch (const std::exception &e) {
              limit = -1;
            }
          }
          if (parser.getArgs().at(1).at(0) == '-') {
            flag = false;
            offset = 1;
          } else if (parser.getArgs().at(1).at(0) == '+') {
            flag = true;
            offset = 1;
          } else {
            flag = true;
            offset = 0;
          }
          for (size_t i = 0 + offset; i < parser.getArgs().at(1).size(); i++) {
            switch (parser.getArgs().at(1).at(i)) {
            case 'o':
              if (this->userPointer.find(us) != this->userPointer.end()) {
                if (!us.empty())
                  this->Chans.at(parser.getArgs().at(0)).setOp(us, flag);
                else
                  send(user.getFd(),
                       parser.SendReply("461", parser.getCommand(),
                                        "Not enough parameters"),
                       parser.getReply().size(), 0);
              } else {
                send(user.getFd(),
                     parser.SendReply("401", us, "No such nick/channel"),
                     parser.getReply().size(), 0);
              }
              break;
            case 'p':
              this->Chans.at(parser.getArgs().at(0)).setFlag(P, flag);
              send(user.getFd(),
                   parser.SendReply(
                       "324", this->Chans.at(parser.getArgs().at(0)).getName(),
                       "p mode has been set to " + bool_to_string(flag)),
                   parser.getReply().size(), 0);
              break;
            case 's':
              this->Chans.at(parser.getArgs().at(0)).setFlag(S, flag);
              send(user.getFd(),
                   parser.SendReply(
                       "324", this->Chans.at(parser.getArgs().at(0)).getName(),
                       "s mode has been set to " + bool_to_string(flag)),
                   parser.getReply().size(), 0);
              break;
            case 'i':
              this->Chans.at(parser.getArgs().at(0)).setFlag(I, flag);
              send(user.getFd(),
                   parser.SendReply(
                       "324", this->Chans.at(parser.getArgs().at(0)).getName(),
                       "i mode has been set to " + bool_to_string(flag)),
                   parser.getReply().size(), 0);
              break;
            case 't':
              this->Chans.at(parser.getArgs().at(0)).setFlag(T, flag);
              send(user.getFd(),
                   parser.SendReply(
                       "324", this->Chans.at(parser.getArgs().at(0)).getName(),
                       "t mode has been set to " + bool_to_string(flag)),
                   parser.getReply().size(), 0);
              break;
            case 'n':
              this->Chans.at(parser.getArgs().at(0)).setFlag(N, flag);
              send(user.getFd(),
                   parser.SendReply(
                       "324", this->Chans.at(parser.getArgs().at(0)).getName(),
                       "n mode has been set to " + bool_to_string(flag)),
                   parser.getReply().size(), 0);
              break;
            case 'm':
              this->Chans.at(parser.getArgs().at(0)).setFlag(M, flag);
              send(user.getFd(),
                   parser.SendReply(
                       "324", this->Chans.at(parser.getArgs().at(0)).getName(),
                       "m mode has been set to " + bool_to_string(flag)),
                   parser.getReply().size(), 0);
              break;
            case 'l':
              this->Chans.at(parser.getArgs().at(0)).setLimit(limit, flag);
              send(user.getFd(),
                   parser.SendReply(
                       "324", this->Chans.at(parser.getArgs().at(0)).getName(),
                       "l mode has been set to " + bool_to_string(flag)),
                   parser.getReply().size(), 0);
              break;
            case 'b':
              if (us.empty())
                this->Chans.at(parser.getArgs().at(0)).setFlag(B, flag);
              else if (this->userPointer.find(us) !=
                       this->userPointer.end()) {
                  this->Chans.at(parser.getArgs().at(0)).setBan(us, flag);
                  send(user.getFd(),
                       parser.SendReply(
                           "367",
                           this->Chans.at(parser.getArgs().at(0)).getName(),
                           us),
                       parser.getReply().size(), 0);
              } else
                send(user.getFd(),
                     parser.SendReply("401", us, "No such nick/channel"),
                     parser.getReply().size(), 0);
              break;
            case 'v':
              if (this->userPointer.find(us) != this->userPointer.end()) {
                if (!us.empty()) {
                  this->Chans.at(parser.getArgs().at(0)).setMod(us, flag);
                  send(user.getFd(),
                       parser.SendReply(
                           "324",
                           this->Chans.at(parser.getArgs().at(0)).getName(),
                           "v mode has been set to " + bool_to_string(flag)),
                       parser.getReply().size(), 0);
                } else
                  send(user.getFd(),
                       parser.SendReply("461", parser.getCommand(),
                                        "Not enough parameters"),
                       parser.getReply().size(), 0);
              } else
                send(user.getFd(),
                     parser.SendReply("401", us, "No such nick/channel"),
                     parser.getReply().size(), 0);
              break;
            case 'k':
              if (!pass.empty()) {
                this->Chans.at(parser.getArgs().at(0)).setPass(pass);
                send(user.getFd(),
                     parser.SendReply(
                         "324",
                         this->Chans.at(parser.getArgs().at(0)).getName(),
                         "k mode has been set to " + bool_to_string(flag)),
                     parser.getReply().size(), 0);
              } else
                send(user.getFd(),
                     parser.SendReply("461", parser.getCommand(),
                                      "Not enough parameters"),
                     parser.getReply().size(), 0);
              break;
            default:
              string str(1, parser.getArgs().at(1).at(i));
              send(user.getFd(),
                   parser.SendReply("472", str, "is unknown mode char to me"),
                   parser.getReply().size(), 0);
              break;
            }
          }
        } else
          send(user.getFd(),
               parser.SendReply("482", parser.getArgs().at(0),
                                "You're not channel operator"),
               parser.getReply().size(), 0);
      } else
        send(user.getFd(),
             parser.SendReply("442", parser.getArgs().at(0),
                              "You're not on that channel"),
             parser.getReply().size(), 0);
    } else
      send(user.getFd(),
           parser.SendReply("403", parser.getArgs().at(0), "No such channel"),
           parser.getReply().size(), 0);
  }
  return (0);
}