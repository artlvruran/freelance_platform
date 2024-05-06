//
// Created by kirill on 22.02.24.
//

#pragma once
#include <string>
#include "employee.h"
#include "state.h"
#include <memory>
#include <utility>
#include "db_pool.h"

class Project {
 public:
  int id;
  int contractor_id;
  std::string name;
  std::unique_ptr<State> state = std::make_unique<NotStarted>();

  Project() = default;

  Project(std::string name) :
          name(std::move(name))
          {};

  void advance(event e);
 private:
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

namespace soci {

template<>
  struct type_conversion<Project> {
    typedef values base_type;
    static void from_base(values const& values, indicator ind, Project& project) {
      if (ind == i_null) return;
      try {
        project.id = values.get<int>("id", 0);
        project.name = values.get<std::string>("name", {});
        project.contractor_id = values.get<int>("contractor_id", 0);

        int int_status = values.get<int>("state", 0);
        if (int_status == 1) {
          project.state = std::make_unique<NotStarted>();
        } else if (int_status == 2) {
          project.state = std::make_unique<Preparing>();
        } else if (int_status == 3) {
          project.state = std::make_unique<Processing>();
        } else {
          project.state = std::make_unique<Completed>();
        }

      } catch (std::exception const& exception) { std::cerr << exception.what() << std::endl; }
    }

    static void to_base(const Project& project, values& values, indicator& ind) {
      try {
        values.set("id", project.id);
        values.set("name", project.name);
        values.set("state", project.state->integer());
        values.set("contractor_id", project.contractor_id);

        ind = i_ok;
        return;
      } catch (std::exception const& exception) {
        std::cerr << exception.what() << std::endl;
      }
      ind = i_null;
    }
  };
}
