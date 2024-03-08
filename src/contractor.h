//
// Created by kirill on 26.02.24.
//

#ifndef FREELANCEPLATFORM_SRC_CONTRACTOR_H_
#define FREELANCEPLATFORM_SRC_CONTRACTOR_H_

#include "user.h"
#include "project.h"

class Contractor : public User {
 public:
  using User::User;
  void add_project(const Project& project);
  void approve_bid(const Project& project);
  void reject_bid(const Project& project);
  void fire_worker(const Project& project);
  void end_project(const Project& project);
  void sign_up() const override;
};

#endif //FREELANCEPLATFORM_SRC_CONTRACTOR_H_
