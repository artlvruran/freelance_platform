//
// Created by kirill on 30.03.24.
//

#ifndef FREELANCEPLATFORM_DATA_TMPL_MASTER_H_
#define FREELANCEPLATFORM_DATA_TMPL_MASTER_H_

#include <cppcms/view.h>
namespace Data {
  struct infoPage {
    std::string title;
    std::string description;
    std::string keywords;
    std::vector<std::pair<std::string,std::string>> menuList;
    std::string current_username;

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

#endif //FREELANCEPLATFORM_DATA_TMPL_MASTER_H_