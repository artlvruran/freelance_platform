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

  static void from_base(values const& v, indicator ind, User& p) {
    if (ind == i_null) return;
    try {
      p.id = v.get<int>("id", 0);
      p.username = v.get<std::string>("username", {});
      p.email = v.get<std::string>("email", {});
      p.password = v.get<std::string>("password", {});
      p.user_role = v.get<std::string>("role", {}) == "employee" ? role::employee : role::contractor;
      p.rate = v.get<double>("rate", 0);
      p.cover = v.get<std::string>("cover", {});
      p.avatar = v.get<std::string>("avatar", {});
      p.fullname = v.get<std::string>("fullname", {});
    } catch (std::exception const & e) { std::cerr << e.what() << std::endl; }
  }

  static void to_base(const User& p, values& v, indicator& ind) {
    try {
      v.set("id", p.id);
      v.set("username", p.username);
      v.set("email", p.email);
      v.set("password", p.password);
      v.set("role", std::string(p.user_role == role::contractor ? "contractor" : "employee"));
      v.set("rate", p.rate);
      v.set("cover", p.cover);
      v.set("avatar", p.avatar);
      v.set("fullname", p.fullname);
      ind = i_ok;
      return;
    } catch (std::exception const & e) { std::cerr << e.what() << std::endl; }
    ind = i_null;
  }
};
}
