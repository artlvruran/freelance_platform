//
// Created by kirill on 22.02.24.
//
#pragma once
#include <iostream>
#include <string>
#include <utility>
#include <vector>

class User {
 public:
  int id;
  std::string username;
  std::string email;
  std::string password;

  User() = default;

  User(std::string username,
       std::string email,
       std::string password) :
      username(std::move(username)),
      email(std::move(email)),
      password(std::move(password)) {};

  virtual bool log_in() = 0;

  virtual void sign_up() = 0;
  void notify(int contractor_id);
};
