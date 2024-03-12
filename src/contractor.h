//
// Created by kirill on 26.02.24.
//

#ifndef FREELANCEPLATFORM_SRC_CONTRACTOR_H_
#define FREELANCEPLATFORM_SRC_CONTRACTOR_H_

#pragma once
#include "user.h"
#include "db_pool.h"
#include "project.h"

class Contractor : public User {
 public:
  using User::User;

  std::string username;

  std::string email;

  std::string password;

  Contractor() = default;

  Contractor(std::string username,
             std::string email,
             std::string password) :
             username(username),
             email(email),
             password(password) {};

  void add_project(Project& project);
  void consider_bid(Bid& bid, bid_event e);
  void fire_worker(const Project& project, const Employee& employee);
  void end_project(Project& project);
  void sign_up(const std::string& username,
               const std::string& email,
               const std::string& password) const override;

  bool log_in(const std::string& username,
              const std::string& email,
              const std::string& password) const override;


  void register_observer(const User& user) const;
  void remove_observer(const User &user) const;
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
      } catch (std::exception const & e) { std::cerr << e.what() << std::endl; }
    }

    static void to_base(const Contractor& p, values& v, indicator& ind) {
      try {
        v.set("id", p.id);
        v.set("username", p.username);
        v.set("email", p.email);
        v.set("password", p.password);

        ind = i_ok;
        return;
      } catch (std::exception const & e) { std::cerr << e.what() << std::endl; }
      ind = i_null;
    }
  };
}

#endif //FREELANCEPLATFORM_SRC_CONTRACTOR_H_
