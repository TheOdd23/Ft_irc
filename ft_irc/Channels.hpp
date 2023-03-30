#ifndef CHANNELS_HPP
#define CHANNELS_HPP

#pragma once
#include "Users.hpp"
#include "Parser.hpp"
#include <sys/socket.h>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#define P 0
#define S 1
#define I 2
#define T 3
#define N 4
#define M 5
#define B 6

using std::string;

class Channels {
public:
  Channels();
  Channels(const string &nam);
  ~Channels();

  void setTopic(const string &topic);
  void setPass(const string &pass);
  void setName(const string &name);
  void setFlag(const int &which, const bool &flag);
  void setLimit(const int &limit, const bool &flag);
  void setOp(const string &user, const bool &op);
  void setBan(const string &user, const bool &ban);
  void setMod(const string &user, const bool &ban);

  const string &getTopic() const;
  const string &getPass() const;
  const string &getName() const;
  const bool &getFlag(const int &where) const;
  const int &getLimit() const;
  bool getOp(const string &user) const;
  std::vector<Users>  &getUsers();
  bool getBan(const string &user) const;
  bool getMod(const string &user) const;

  bool is_user(const std::string user);
  int addUser(const Users &user, const std::string &pass, const bool &invited);
  void  RemoveUser(std::string nickname);

private:
  int _limit;
  string _pass;
  string _name;
  string _topic;
  bool _flags[7];
  std::vector<Users>  _users;
  std::map<string, bool> _mod;
  std::map<string, bool> _ban;
  std::map<string, bool> _operators;
};

#endif