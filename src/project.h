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
  std::string name;
  std::string description;
  std::string timeline;
  double budget;
  std::unique_ptr<State> status = std::make_unique<Preparing>();

  Project(std::string  name,
          std::string  description,
          std::string  timeline,
          double budget) :
          name(std::move(name)),
          description(std::move(description)),
          timeline(std::move(timeline)),
          budget(budget) {};

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
