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
#include "constants.h"
#include "boost/format.hpp"
#include "../database.h"

class Project {
 public:
  int id;
  int contractor_id;
  std::string name;
  std::unique_ptr<State> state = std::make_unique<NotStarted>();
  std::string location;
  double wage;
  std::string description;
  std::string type;

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
          description(other.description),
          type(other.type)
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
    type = other.type;
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

  void advance(event exception);

  virtual void load(int con_id) const {
    DataBase db(db_source);
    db << (boost::format("insert into projects (name, contractor_id, state, location, wage, description)"
                          "values(:name, %d, :state, :location, :wage, :description)") % con_id).str(), soci::use(*this);
  };
 private:
};

class LongTermJob : public Project {
 public:
  using Project::Project;
  std::string specialization;
  std::string format;
  void load(int contractor_id) const override {
    DataBase db(db_source);
    db << (boost::format("insert into projects (name, contractor_id, state, location, wage, description, specialization, format, type)"
                          "values(:name, %d, :state, :location, :wage, :description, :specialization, :format, 'long')") % contractor_id).str(), soci::use(*this);
  }
};

class Task : public Project {
 public:
  using Project::Project;
};

class Contest : public Project {
 public:
  using Project::Project;
  std::string start_at;
  std::string end_at;
  void load(int contractor_id) const override {
    DataBase db(db_source);
    db << (boost::format("insert into projects (name, contractor_id, state, location, wage, description, start_at, end_at, type)"
                          "values(:name, %d, :state, :location, :wage, :description, :start_at, :end_at, 'contest')") % contractor_id).str(), soci::use(*this);
  }
};

namespace soci {

template<>
  struct type_conversion<Project> {
    typedef values base_type;
    static void from_base(values const &values, indicator ind, Project &project) {
      if (ind == i_null) return;
      try {
        project.id = values.get<int>("id", 0);
        project.name = values.get<std::string>("name", {});
        project.contractor_id = values.get<int>("contractor_id", 0);

        int int_status = values.get<int>("state", 0);
        if (int_status == 0) {
          project.state = std::make_unique<NotStarted>();
        } else if (int_status == 1) {
          project.state = std::make_unique<Preparing>();
        } else if (int_status == 2) {
          project.state = std::make_unique<Processing>();
        } else {
          project.state = std::make_unique<Completed>();
        }

        project.location = values.get<std::string>("location", {});
        project.wage = values.get<double>("wage", 0);
        project.description = values.get<std::string>("description", {});
        project.type = values.get<std::string>("type", {});

      } catch (std::exception const &exception) { std::cerr << exception.what() << std::endl; }
    }

    static void to_base(const Project &project, values &values, indicator &ind) {
      try {
        values.set("id", project.id);
        values.set("name", project.name);
        values.set("state", project.state->integer());
        values.set("contractor_id", project.contractor_id);
        values.set("location", project.location);
        values.set("wage", project.wage);
        values.set("description", project.description);
        values.set("type", project.type);

        ind = i_ok;
        return;
      } catch (std::exception const &exception) { std::cerr << exception.what() << std::endl; }
      ind = i_null;
    }
  };

template<>
struct type_conversion<Task> {
  typedef values base_type;
  static void from_base(values const &values, indicator ind, Task &project) {
    type_conversion<Project>::from_base(values, ind, project);
  }

  static void to_base(const Task &project, values &values, indicator &ind) {
    type_conversion<Project>::to_base(project, values, ind);
    values.set("type", std::string("task"));
  }
};

template<>
struct type_conversion<LongTermJob> {
  typedef values base_type;
  static void from_base(values const &values, indicator ind, LongTermJob &project) {
    type_conversion<Project>::from_base(values, ind, project);
    project.specialization = values.get<std::string>("specialization", {});
    project.format = values.get<std::string>("format", {});
  }

  static void to_base(const LongTermJob &project, values &values, indicator &ind) {
    type_conversion<Project>::to_base(project, values, ind);
    values.set("specialization", project.specialization);
    values.set("format", project.format);
    values.set("type", std::string("long"));
  }
};

template<>
struct type_conversion<Contest> {
  typedef values base_type;
  static void from_base(values const &values, indicator ind, Contest &project) {
    type_conversion<Project>::from_base(values, ind, project);
    project.start_at = values.get<std::string>("start_at", {});
    project.end_at = values.get<std::string>("end_at", {});
  }

  static void to_base(const Contest &project, values &values, indicator &ind) {
    type_conversion<Project>::to_base(project, values, ind);
    values.set("start_at", project.start_at);
    values.set("end_at", project.end_at);
    values.set("type", std::string("contest"));
  }
};
}
