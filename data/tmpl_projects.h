//
// Created by kirill on 30.03.24.
//


#pragma once

#include <cppcms/view.h>
#include "../src/project.h"
#include "../src/contractor.h"
#include "tmpl_master.h"
namespace Data {

  struct projectsPage {
    std::vector<std::pair<Project, Contractor>> projects;
    projectsPage(){};
    ~projectsPage(){};
  };

  struct Projects : public Master {
    Data::projectsPage projects_page;
  };
}
