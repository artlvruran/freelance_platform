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

    static void from_base(values const& values, indicator ind, Employee& employee) {
      if (ind == i_null) return;
      try {
        employee.id = values.get<int>("id", 0);
        employee.username = values.get<std::string>("username", {});
        employee.email = values.get<std::string>("email", {});
        employee.password = values.get<std::string>("password", {});
      } catch (std::exception const& exception) {
        std::cerr << exception.what() << std::endl;
      }
    }

    static void to_base(const Employee& employee, values& values, indicator& ind) {
      try {
        values.set("id", employee.id);
        values.set("username", employee.username);
        values.set("email", employee.email);
        values.set("password", employee.password);
        values.set("role", std::string("employee"));
        ind = i_ok;
        return;
      } catch (std::exception const& exception) {
        std::cerr << exception.what() << std::endl;
      }
      ind = i_null;
    }
  };
}
