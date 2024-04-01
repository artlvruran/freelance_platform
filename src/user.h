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
    } catch (std::exception const & e) { std::cerr << e.what() << std::endl; }
  }

  static void to_base(const User& p, values& v, indicator& ind) {
    try {
      v.set("id", p.id);
      v.set("username", p.username);
      v.set("email", p.email);
      v.set("password", p.password);
      v.set("role", std::string(p.user_role == role::contractor ? "contractor" : "employee"));
      ind = i_ok;
      return;
    } catch (std::exception const & e) { std::cerr << e.what() << std::endl; }
    ind = i_null;
  }
};
}

#endif //FREELANCEPLATFORM_SRC_USER_H_
