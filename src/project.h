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
  std::string location;
  double wage;
  std::string description;

  Project() = default;

  Project(std::string name) :
          name(std::move(name))
          {};

  Project(const Project& other) :
          id(other.id),
          contractor_id(other.contractor_id),
          name(other.name),
          location(other.location),
          wage(other.wage),
          description(other.description)
  {
    if (other.state->integer() == 0) {
      state = std::make_unique<NotStarted>();
    } else if (other.state->integer() == 1) {
      state = std::make_unique<Preparing>();
    } else if (other.state->integer() == 2) {
      state = std::make_unique<Processing>();
    } else {
      state = std::make_unique<Completed>();
    }
  }

  Project& operator=(const Project& other) {
    id = other.id;
    contractor_id = other.contractor_id;
    name = other.name;
    location = other.location;
    wage = other.wage;
    description = other.description;
    if (other.state->integer() == 0) {
      state = std::make_unique<NotStarted>();
    } else if (other.state->integer() == 1) {
      state = std::make_unique<Preparing>();
    } else if (other.state->integer() == 2) {
      state = std::make_unique<Processing>();
    } else {
      state = std::make_unique<Completed>();
    }
    return *this;
  }

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
    static void from_base(values const &v, indicator ind, Project &p) {
      if (ind == i_null) return;
      try {
        p.id = v.get<int>("id", 0);
        p.name = v.get<std::string>("name", {});
        p.contractor_id = v.get<int>("contractor_id", 0);

        int int_status = v.get<int>("state", 0);
        if (int_status == 0) {
          p.state = std::make_unique<NotStarted>();
        } else if (int_status == 1) {
          p.state = std::make_unique<Preparing>();
        } else if (int_status == 2) {
          p.state = std::make_unique<Processing>();
        } else {
          p.state = std::make_unique<Completed>();
        }

        p.location = v.get<std::string>("location", {});
        p.wage = v.get<double>("wage", 0);
        p.description = v.get<std::string>("description", {});

      } catch (std::exception const &e) { std::cerr << e.what() << std::endl; }
    }

    static void to_base(const Project &p, values &v, indicator &ind) {
      try {
        v.set("id", p.id);
        v.set("name", p.name);
        v.set("state", p.state->integer());
        v.set("contractor_id", p.contractor_id);
        v.set("location", p.location);
        v.set("wage", p.wage);
        v.set("description", p.description);

        ind = i_ok;
        return;
      } catch (std::exception const &e) { std::cerr << e.what() << std::endl; }
      ind = i_null;
    }
  };
}
