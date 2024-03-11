//
// Created by kirill on 22.02.24.
//

#ifndef FREELANCEPLATFORM_SRC_PROJECT_H_
#define FREELANCEPLATFORM_SRC_PROJECT_H_

#pragma once
#include <string>
#include "employee.h"
#include "state.h"
#include <memory>
#include <utility>


class Project {
 public:
  int id;
  std::string name;
  std::unique_ptr<State> status = std::make_unique<NotStarted>();

  Project(std::string  name) :
          name(std::move(name))
          {};

  void advance(event e);
};

class LongTermJob : public Project {
  using Project::Project;
};

class Task : public Project {
  using Project::Project;
};

class Contest : public Project {
  using Project::Project;
};


#endif //FREELANCEPLATFORM_SRC_PROJECT_H_
