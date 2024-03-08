//
// Created by kirill on 22.02.24.
//

#ifndef FREELANCEPLATFORM_SRC_PROJECT_H_
#define FREELANCEPLATFORM_SRC_PROJECT_H_

#include <string>
#include "employee.h"


class Project {
 public:
  std::string name;
  std::string description;
  std::string timeline;
  double budget;

  Project(std::string& name,
          std::string& description,
          std::string& timeline,
          double budget) :
          name(name),
          description(description),
          timeline(timeline),
          budget(budget) {};
};

class LongTermJob : Project {
  using Project::Project;
};

class Task : Project {
  using Project::Project;

};

class Contest : Project {
  using Project::Project;

};


#endif //FREELANCEPLATFORM_SRC_PROJECT_H_
