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

  void advance(event e);

  virtual void load(int contractor_id) const {
    std::string src = "dbname=";
    src += db_source;
    soci::session sql("sqlite3", src);
    sql << (boost::format("insert into projects (name, contractor_id, state, location, wage, description)"
                          "values(:name, %d, :state, :location, :wage, :description)") % contractor_id).str(), soci::use(*this);
  };
 private:
};

class LongTermJob : public Project {
 public:
  using Project::Project;
  std::string specialization;
  std::string format;
  void load(int contractor_id) const override {
    std::string src = "dbname=";
    src += db_source;
    soci::session sql("sqlite3", src);
    sql << (boost::format("insert into projects (name, contractor_id, state, location, wage, description, specialization, format, type)"
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
    std::string src = "dbname=";
    src += db_source;
    soci::session sql("sqlite3", src);
    sql << (boost::format("insert into projects (name, contractor_id, state, location, wage, description, start_at, end_at, type)"
                          "values(:name, %d, :state, :location, :wage, :description, :start_at, :end_at, 'contest')") % contractor_id).str(), soci::use(*this);
  }
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
        p.type = v.get<std::string>("type", {});

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
        v.set("type", p.type);

        ind = i_ok;
        return;
      } catch (std::exception const &e) { std::cerr << e.what() << std::endl; }
      ind = i_null;
    }
  };

template<>
struct type_conversion<Task> {
  typedef values base_type;
  static void from_base(values const &v, indicator ind, Task &p) {
    type_conversion<Project>::from_base(v, ind, p);
  }

  static void to_base(const Task &p, values &v, indicator &ind) {
    type_conversion<Project>::to_base(p, v, ind);
    v.set("type", std::string("task"));
  }
};

template<>
struct type_conversion<LongTermJob> {
  typedef values base_type;
  static void from_base(values const &v, indicator ind, LongTermJob &p) {
    type_conversion<Project>::from_base(v, ind, p);
    p.specialization = v.get<std::string>("specialization", {});
    p.format = v.get<std::string>("format", {});
  }

  static void to_base(const LongTermJob &p, values &v, indicator &ind) {
    type_conversion<Project>::to_base(p, v, ind);
    v.set("specialization", p.specialization);
    v.set("format", p.format);
    v.set("type", std::string("long"));
  }
};

template<>
struct type_conversion<Contest> {
  typedef values base_type;
  static void from_base(values const &v, indicator ind, Contest &p) {
    type_conversion<Project>::from_base(v, ind, p);
    p.start_at = v.get<std::string>("start_at", {});
    p.end_at = v.get<std::string>("end_at", {});
  }

  static void to_base(const Contest &p, values &v, indicator &ind) {
    type_conversion<Project>::to_base(p, v, ind);
    v.set("start_at", p.start_at);
    v.set("end_at", p.end_at);
    v.set("type", std::string("contest"));
  }
};
}
