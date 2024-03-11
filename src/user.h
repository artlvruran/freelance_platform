//
// Created by kirill on 22.02.24.
//

#ifndef FREELANCEPLATFORM_SRC_USER_H_
#define FREELANCEPLATFORM_SRC_USER_H_

#pragma once
#include <iostream>
#include <string>
#include <utility>
#include <vector>

class User {
 public:
  virtual bool log_in(const std::string& username,
                         const std::string& email,
                         const std::string& password) const = 0;

  virtual void sign_up(const std::string& username,
                       const std::string& email,
                       const std::string& password) const = 0;
};

#endif //FREELANCEPLATFORM_SRC_USER_H_
