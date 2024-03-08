//
// Created by kirill on 26.02.24.
//

#ifndef FREELANCEPLATFORM_SRC_EMPLOYEE_H_
#define FREELANCEPLATFORM_SRC_EMPLOYEE_H_

#pragma once
#include "user.h"

class Employee : public User {
 public:
  using User::User;
  std::vector<std::string> skills;
  std::string bio;
  int age;
  void sign_up() const override;
};

#endif //FREELANCEPLATFORM_SRC_EMPLOYEE_H_
