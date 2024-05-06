//
// Created by kirill on 22.02.24.
//

#pragma once
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <soci/values.h>

enum role {
  employee,
  contractor
};

class User {
 public:
  int id;
  std::string username;
  std::string email;
  std::string password;
  role user_role;
  double rate;
  std::string cover;
  std::string avatar;
  std::string fullname;

  User() = default;

  User(std::string username,
       std::string email,
       std::string password) :
      username(std::move(username)),
      email(std::move(email)),
      password(std::move(password)) {};

  void notify(int contractor_id);
  static void read(int notification_id);
};

namespace soci {
template<> struct type_conversion<User> {
  typedef values base_type;

  static void from_base(values const& values, indicator ind, User& user) {
    if (ind == i_null) return;
    try {
      user.id = values.get<int>("id", 0);
      user.username = values.get<std::string>("username", {});
      user.email = values.get<std::string>("email", {});
      user.password = values.get<std::string>("password", {});
      user.user_role = values.get<std::string>("role", {}) == "employee" ? role::employee : role::contractor;
      user.rate = values.get<double>("rate", 0);
      user.cover = values.get<std::string>("cover", {});
      user.avatar = values.get<std::string>("avatar", {});
      user.fullname = values.get<std::string>("fullname", {});
    } catch (std::exception const& exception) {
      std::cerr << exception.what() << std::endl;
    }
  }

  static void to_base(const User& user, values& values, indicator& ind) {
    try {
      values.set("id", user.id);
      values.set("username", user.username);
      values.set("email", user.email);
      values.set("password", user.password);
      values.set("role", std::string(user.user_role == role::contractor ? "contractor" : "employee"));
      values.set("rate", user.rate);
      values.set("cover", user.cover);
      values.set("avatar", user.avatar);
      values.set("fullname", user.fullname);
      ind = i_ok;
      return;
    } catch (std::exception const& exception) {
      std::cerr << exception.what() << std::endl;
    }
    ind = i_null;
  }
};
}
