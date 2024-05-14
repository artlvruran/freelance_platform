//
// Created by kirill on 30.03.24.
//


#pragma once

#include <cppcms/view.h>
#include "../src/models/user.h"

namespace Data {
  struct Master : public cppcms::base_content {
    std::string title;
    std::vector<std::pair<std::string,std::string>> menuList;
    std::string current_username;
    std::vector<std::pair<int, std::string>> notifications;
  };
}