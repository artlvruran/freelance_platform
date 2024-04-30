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
  void consider_bid(Bid& bid, bid_event e);
  void fire_worker(const Project& project, const Employee& employee);
  void end_project(Project& project);
  void end_project_hiring(Project& project);
  void start_project_hiring(Project& project);
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

    static void from_base(values const& v, indicator ind, Contractor& p) {
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

    static void to_base(const Contractor& p, values& v, indicator& ind) {
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
