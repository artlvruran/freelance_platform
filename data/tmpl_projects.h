//
// Created by kirill on 30.03.24.
//

#ifndef FREELANCEPLATFORM_DATA_TMPL_PROJECTS_H_
#define FREELANCEPLATFORM_DATA_TMPL_PROJECTS_H_

#include <cppcms/view.h>
#include "../src/project.h"
#include "tmpl_master.h"
namespace Data {

  struct projectsPage {
    std::vector<Project> projects;
    projectsPage(){};
    ~projectsPage(){};
  };

  struct Projects : public Master {
    Data::projectsPage projects_page;
  };
}

#endif //FREELANCEPLATFORM_DATA_TMPL_PROJECTS_H_
