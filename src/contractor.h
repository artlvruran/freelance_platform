//
// Created by kirill on 26.02.24.
//

#ifndef FREELANCEPLATFORM_SRC_CONTRACTOR_H_
#define FREELANCEPLATFORM_SRC_CONTRACTOR_H_

#pragma once
#include "user.h"
#include "project.h"
#include "bid.h"

class Contractor : public User {
 public:
  using User::User;
  void add_project(Project& project);
  void consider_bid(Bid& bid, bid_event e);
  void fire_worker(const Project& project);
  void end_project(const Project& project);
  void sign_up(const std::string& username,
               const std::string& email,
               const std::string& password) const override;

  bool log_in(const std::string& username,
               const std::string& email,
               const std::string& password) const override;
};

#endif //FREELANCEPLATFORM_SRC_CONTRACTOR_H_
