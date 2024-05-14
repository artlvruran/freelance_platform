//
// Created by kirill on 30.03.24.
//


#pragma once

#include <cppcms/view.h>
#include "../src/user.h"

namespace Data {
  struct infoPage {
    std::string title;
    std::string description;
    std::string keywords;
    std::vector<std::pair<std::string,std::string>> menuList;
    std::string current_username;
    std::vector<std::pair<int, std::string>> notifications;

    infoPage() :
        title      (""),
        description(""),
        keywords   (""),
        menuList   (  )
    {}

    ~infoPage(){}
  };

  struct Master :public cppcms::base_content {
    infoPage    page;

    Master() :
        page()
    {}

    ~Master(){}
  };
}
