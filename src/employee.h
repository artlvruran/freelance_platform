//
// Created by kirill on 26.02.24.
//

#ifndef FREELANCEPLATFORM_SRC_EMPLOYEE_H_
#define FREELANCEPLATFORM_SRC_EMPLOYEE_H_

#pragma once
#include "user.h"
#include "bid.h"
#include "db_pool.h"

class Employee : public User {
 public:
  using User::User;

  std::string username;

  std::string email;

  std::string password;

  Employee() = default;

  void sign_up(const std::string& username,
               const std::string& email,
               const std::string& password) const override;

  bool log_in(const std::string& username,
               const std::string& email,
               const std::string& password) const override;

  Bid create_bid(int project_id);
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
      } catch (std::exception const & e) { std::cerr << e.what() << std::endl; }
    }

    static void to_base(const Employee& p, values& v, indicator& ind) {
      try {
        v.set("id", p.id);
        v.set("username", p.username);
        v.set("email", p.email);
        v.set("password", p.password);
        v.set("role", std::string("employee"));
        ind = i_ok;
        return;
      } catch (std::exception const & e) { std::cerr << e.what() << std::endl; }
      ind = i_null;
    }
  };
}

#endif //FREELANCEPLATFORM_SRC_EMPLOYEE_H_
