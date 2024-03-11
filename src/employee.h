//
// Created by kirill on 26.02.24.
//

#ifndef FREELANCEPLATFORM_SRC_EMPLOYEE_H_
#define FREELANCEPLATFORM_SRC_EMPLOYEE_H_

#pragma once
#include "user.h"
#include "bid.h"

class Employee : public User {
 public:
  using User::User;
  void sign_up(const std::string& username,
               const std::string& email,
               const std::string& password) const override;

  bool log_in(const std::string& username,
               const std::string& email,
               const std::string& password) const override;

  Bid create_bid(int project_id);
};

#endif //FREELANCEPLATFORM_SRC_EMPLOYEE_H_
