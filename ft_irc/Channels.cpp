#include "Channels.hpp"

Channels::Channels() {}

Channels::Channels(const string &name) : _limit(-1), _name(name) {
  for (size_t i = 0; i < 6; i++)
    this->_flags[i] = false;
}

Channels::~Channels() {}

void Channels::setTopic(const string &topic) { this->_topic = topic; }

void Channels::setPass(const string &pass) { this->_pass = pass; }

void Channels::setName(const string &name) { this->_name = name; }

void Channels::setFlag(const int &which, const bool &flag) {
  this->_flags[which] = flag;
}

void Channels::setLimit(const int &limit, const bool &flag) {
  if (flag == false)
    this->_limit = -1;
  else
    this->_limit = limit;
}

void Channels::setOp(const string &user, const bool &op) {
  if (this->_operators.find(user) != this->_operators.end())
    this->_operators.at(user) = op;
}

void Channels::setBan(const string &user, const bool &ban) {
  if (this->_ban.find(user) != this->_ban.end())
    this->_ban.at(user) = ban;
}


void Channels::setMod(const string &user, const bool &ban){
  if (this->_mod.find(user) != this->_mod.end())
    this->_mod.at(user) = ban;
}

const string &Channels::getTopic() const { return this->_topic; }

const string &Channels::getPass() const { return this->_pass; }

const string &Channels::getName() const { return this->_name; }

const bool &Channels::getFlag(const int &where) const {
  return this->_flags[where];
}

const int &Channels::getLimit() const { return this->_limit; }

bool Channels::getOp(const string &user) const {
  if (this->_operators.find(user) == this->_operators.end())
    return (false);
  return this->_operators.at(user);
}

bool Channels::getBan(const string &user) const {
  if (this->_ban.find(user) == this->_ban.end())
    return (false);
  return this->_ban.at(user);
}

bool Channels::getMod(const string &user) const {
  if (this->_mod.find(user) == this->_mod.end())
    return (false);
  return this->_mod.at(user);
}

std::vector<Users> &Channels::getUsers() { return _users; }

bool Channels::is_user(const std::string user) {
  for (size_t i = 0; i < this->_users.size() ; i++){
    if (this->_users.at(i).getNickname() == user)
      return true;
  }
  return false;
}

int Channels::addUser(const Users &user, const std::string &pass, const bool &invited) {
  std::vector<Users>::iterator it = this->_users.begin();
  while (it != this->_users.end()) {
    if (user.getNickname() == (*it).getNickname())
      break;
    it++;
  }
  if (it == this->_users.end()) {
    if (this->getFlag(I) == false || invited == true)
      if (this->getBan(user.getNickname()) == false)
        if (this->getLimit() == -1 ||
            (const int) this->getUsers().size() < this->getLimit())
          if (!this->getPass().empty())
            if (this->getPass() == pass) {
              this->_users.push_back(user);
              this->_operators.insert(
                std::pair<std::string, bool>(user.getNickname(), false));
              this->_ban.insert(
                std::pair<std::string, bool>(user.getNickname(), false));
              this->_mod.insert(
                std::pair<std::string, bool>(user.getNickname(), false));
            } else
              return 1;
          else {
            this->_users.push_back(user);
            this->_operators.insert(
                std::pair<std::string, bool>(user.getNickname(), false));
            this->_ban.insert(
                std::pair<std::string, bool>(user.getNickname(), false));
            this->_mod.insert(
                std::pair<std::string, bool>(user.getNickname(), false));
          }
        else
          return 2;
      else
        return 3;
    else
      return 4;
  }
  return 5;
}

void Channels::RemoveUser(std::string nickname) {
  std::vector<Users>::iterator it = _users.begin();
  std::vector<Users>::iterator ite = _users.end();

  while (it != ite) {
    if (nickname == it->getNickname()) {
      _users.erase(it);
      break;
    }

    it++;
  }
}