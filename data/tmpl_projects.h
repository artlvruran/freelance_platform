//
// Created by kirill on 30.03.24.
//


#pragma once

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
