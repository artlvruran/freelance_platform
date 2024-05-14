//
// Created by kirill on 30.03.24.
//


#pragma once

#include <cppcms/view.h>
#include "../src/models/project.h"
#include "../src/models/contractor.h"
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

struct tasksPage {
  std::vector<std::pair<Task, Contractor>> projects;
  tasksPage(){};
  ~tasksPage(){};
};
struct Tasks : public Master {
  Data::tasksPage projects_page;
};

struct longsPage {
  std::vector<std::pair<LongTermJob, Contractor>> projects;
  longsPage(){};
  ~longsPage(){};
};

struct Longs : public Master {
  Data::longsPage projects_page;
};

struct contestsPage {
  std::vector<std::pair<Contest, Contractor>> projects;
  contestsPage(){};
  ~contestsPage(){};
};

struct Contests : public Master {
  Data::contestsPage projects_page;
};
}
