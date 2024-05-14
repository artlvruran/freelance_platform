//
// Created by kirill on 26.02.24.
//
#pragma once
#include "user.h"
#include "db_pool.h"
#include "project.h"
#include <string>

class Contractor : public User {
 public:
  using User::User;

  Contractor() = default;

  void add_project(Project& project);
  void consider_bid(Bid& bid, bid_event event);
  void fire_worker(const Project& project, const Employee& employee);
  void end_project(Project& project);
  void start_project_hiring(Project& project);
  void end_project_hiring(Project& project);
  void sign_up();

  bool log_in();


  void register_observer(const User& user) const;
  void remove_observer(const User &user) const;
  void notify_observers(std::string description) const;
 private:
};

namespace soci {
  template<> struct type_conversion<Contractor> {
    typedef values base_type;

    static void from_base(values const& values, indicator ind, Contractor& contractor) {
      if (ind == i_null) return;
      try {
        contractor.id = values.get<int>("id", 0);
        contractor.username = values.get<std::string>("username", {});
        contractor.email = values.get<std::string>("email", {});
        contractor.password = values.get<std::string>("password", {});
      } catch (std::exception const & exception) {
        std::cerr << exception.what() << std::endl;
      }
    }

    static void to_base(const Contractor& contractor, values& values, indicator& ind) {
      try {
        values.set("id", contractor.id);
        values.set("username", contractor.username);
        values.set("email", contractor.email);
        values.set("password", contractor.password);
        values.set("role", std::string("contractor"));
        ind = i_ok;
        return;
      } catch (std::exception const& exception) {
        std::cerr << exception.what() << std::endl;
      }
      ind = i_null;
    }
  };
}
