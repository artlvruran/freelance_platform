//
// Created by kirill on 22.02.24.
//

#ifndef FREELANCEPLATFORM_SRC_USER_H_
#define FREELANCEPLATFORM_SRC_USER_H_

#pragma once
#include <iostream>
#include <string>
#include <utility>
#include <vector>

class User {
 public:
  std::string username;
  std::string password;
  std::string email;
  std::string occupation;
  std::string country;
  std::string city;

  int rating = 0;

  User(std::string  username,
       std::string  password,
       std::string  email,
       std::string occupation,
       std::string country,
       std::string city) :
       username(std::move(username)),
       password(std::move(password)),
       email(std::move(email)),
       occupation(std::move(occupation)),
       country(std::move(country)),
       city(std::move(city)) {};

  //virtual void log_in() const = 0;

  virtual void sign_up() const = 0;
};

#endif //FREELANCEPLATFORM_SRC_USER_H_
