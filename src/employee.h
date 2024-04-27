//
// Created by kirill on 26.02.24.
//
#pragma once
#include "user.h"
#include "bid.h"
#include "db_pool.h"

class Employee : public User {
 public:
  using User::User;

  Employee() = default;

  void sign_up();

  bool log_in();

  void create_bid(int project_id);
 private:
};

namespace soci {
template<> struct type_conversion<Employee> {
  typedef values base_type;

  static void from_base(values const& v, indicator ind, Employee& p) {
    if (ind == i_null) return;
    try {
      p.id = v.get<int>("id", 0);
      p.username = v.get<std::string>("username", {});
      p.email = v.get<std::string>("email", {});
      p.password = v.get<std::string>("password", {});
      p.rate = v.get<double>("rate", 0);
      p.cover = v.get<std::string>("cover", {});
      p.avatar = v.get<std::string>("avatar", {});
      p.fullname = v.get<std::string>("fullname", {});
    } catch (std::exception const & e) { std::cerr << e.what() << std::endl; }
  }

  static void to_base(const Employee& p, values& v, indicator& ind) {
    try {
      v.set("id", p.id);
      v.set("username", p.username);
      v.set("email", p.email);
      v.set("password", p.password);
      v.set("role", std::string("contractor"));
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
