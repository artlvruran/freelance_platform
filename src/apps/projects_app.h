//
// Created by kirill on 06.05.24.
//
#pragma once

#include <cppcms/applications_pool.h>
#include <cppcms/url_dispatcher.h>
#include <cppcms/http_response.h>
#include <cppcms/application.h>
#include <cppcms/url_mapper.h>
#include <cppcms/service.h>
#include <cppcms/application.h>
#include <cppcms/http_file.h>

#include "../../data/tmpl_master.h"
#include "../../data/tmpl_signup.h"
#include "../../data/tmpl_projects.h"
#include "../../data/tmpl_project.h"
#include "../../data/tmpl_add_project.h"
#include "../../data/tmpl_contractors.h"
#include "../../data/tmpl_user.h"
#include "../../data/tmpl_profile_edit.h"
#include "../../data/tmpl_notifications.h"
#include "../../data/tmpl_employees.h"
#include "../../data/tmpl_main.h"
#include "../../data/tmpl_send_message.h"

#include "../../src/models/employee.h"
#include "../../src/models/contractor.h"
#include "../../src/models/constants.h"
#include "../../src/models/message.h"

#include "../utils/utils.h"

class Projects : public cppcms::application {
 public:
  Projects(cppcms::service &srv) : cppcms::application(srv) {
    dispatcher().assign("/bid_on/(.+)", &Projects::bid_on, this, 1);
    mapper().assign("/bid_on/{1}");

    dispatcher().assign("/add_project/task", &Projects::add_task, this);
    mapper().assign("/add_project/task");

    dispatcher().assign("/add_project/long", &Projects::add_long, this);
    mapper().assign("/add_project/long");

    dispatcher().assign("/add_project/contest", &Projects::add_contest, this);
    mapper().assign("/add_project/contest");

    dispatcher().assign("/add_project", &Projects::add_project, this);
    mapper().assign("/add_project");

    dispatcher().assign("/(.+)/advance", &Projects::advance, this, 1);
    mapper().assign("/{1}/advance");

    dispatcher().assign("/(.+)/(.+)", &Projects::consider, this, 1, 2);
    mapper().assign("/{1}/{2}");

    dispatcher().assign("/(.+)", &Projects::project, this, 1);
    mapper().assign("/{1}");

    dispatcher().assign("", &Projects::projects, this);
    mapper().assign("");

    dispatcher().assign("/", &Projects::projects, this);
    mapper().assign("/");
  }

  virtual void projects() {
    std::string paramlist = request().query_string();
    if (!paramlist.empty()) {
      auto param = paramlist.substr(5, paramlist.size() - 5);
      projects_filter(param);
      return;
    }

    Data::Projects mn;

    add_menu(mn, *this);

    DataBase db(db_source);
    soci::rowset<Project> projects = (db.prepare("select * from projects"));
    std::vector<std::pair<Project, Contractor>> all_projects;
    for (auto& project : projects) {
      Contractor contractor;
      db << "select * from users where id = :contractor_id and role='contractor'",
          soci::use(project.contractor_id), soci::into(contractor);
      all_projects.emplace_back(project, contractor);
    }
    mn.projects_page.projects = all_projects;

    render("Projects", mn);
  }

  template<typename ProjectType>
  std::vector<std::pair<ProjectType, Contractor>> get_all_projects(const std::string& tp) {
    DataBase db(db_source);
    soci::rowset<ProjectType> projects = (db.prepare("select * from projects where type=\"" + tp + "\""));
    std::vector<std::pair<ProjectType, Contractor>> all_projects;
    for (auto it = projects.begin(); it != projects.end(); ++it) {
      Contractor contractor;
      db << "select * from users where id = :contractor_id and role='contractor'",
          soci::use(it->contractor_id), soci::into(contractor);
      all_projects.emplace_back(*it, contractor);
    }
    return all_projects;
  }

  template<typename ProjectType, typename ViewType>
  ViewType filter(const std::string& tp) {
    auto all_projects = get_all_projects<ProjectType>(tp);
    ViewType view;
    view.projects_page.projects = all_projects;
    add_menu(view, *this);
    return view;
  }

  virtual void projects_filter(std::string number) {
    std::string tp;
    if (number == "1") {
      tp = "task";
      auto view = filter<Task, Data::Tasks>(tp);
      render("Tasks", view);
    } else if (number == "2") {
      tp = "long";
      auto view = filter<LongTermJob, Data::Longs>(tp);
      render("Longs", view);
    } else {
      tp = "contest";
      auto view = filter<Contest, Data::Contests>(tp);
      render("Contests", view);
    }
  }

  virtual void project(std::string id) {
    Data::SingleProject pr;

    add_menu(pr, *this);

    DataBase db(db_source);
    Project project;
    db << "select * from projects where id=:id", soci::use(id), soci::into(project);

    pr.project_page.project = project;
    if (session().is_set("role")) {
      if (session()["role"] == "employee") {
        pr.project_page.is_employee = true;
        soci::indicator ind;
        Bid bid;
        db << "select * from bids where project_id=:project_id and employee_id=("
               "select id from users where role='employee' and username=:username"
               ")",
            soci::use(id), soci::use(session()["username"]), soci::into(bid, ind);
        if (db.got_data() && ind == soci::i_ok) {
          pr.project_page.is_bid_created = true;
          pr.project_page.bids.emplace_back(bid, get_employee(*this));
        }
      } else {
        pr.project_page.is_employee = false;

        int contractor_id;
        db << "select contractor_id from projects where id=:id", soci::use(id), soci::into(contractor_id);

        Contractor contractor;
        db << "select * from users where role='contractor' and username=:username",
            soci::use(session()["username"]), soci::into(contractor);

        if (contractor_id == contractor.id) {
          pr.project_page.has_right = true;
          soci::rowset<Bid> rs = (db.prepare("select * from bids where project_id = :id"), soci::use(id));
          for (auto& bid : rs) {
            pr.project_page.bids.emplace_back(bid, get_employee_by_id(std::to_string(bid.employee_id)));
          }
        } else {
          pr.project_page.has_right = false;
        }
      }
    }
    render("SingleProject", pr);
  }

  virtual void bid_on(std::string id) {
    if (session().is_set("role")) {
      if (session()["role"] == "employee") {
        DataBase db(db_source);
        Project project;
        db << "select * from projects where id=:id", soci::use(id), soci::into(project);

        Employee employee;
        db << "select * from users where role='employee' and username=:username",
            soci::use(session()["username"]), soci::into(employee);

        employee.create_bid(project.id);

        response().set_redirect_header("/projects/" + id);
        return;
      } else {
        response().set_redirect_header("/projects/" + id);
        return;
      }
    }
  }

  virtual void consider(std::string bid_id, std::string status) {
    if (session().is_set("role")) {
      if (session()["role"] == "employee") {
        response().set_redirect_header("/projects/");
        return;
      } else {
        int contractor_id;

        DataBase db(db_source);
        db << "select contractor_id from projects where id=("
               "select project_id from bids where id=:bid_id"
               ")", soci::use(bid_id), soci::into(contractor_id);

        Contractor contractor;
        db << "select * from users where role='contractor' and username=:username",
            soci::use(session()["username"]), soci::into(contractor);
        if (contractor_id == contractor.id) {
          Bid bid;
          db << "select * from bids where id=:bid_id", soci::use(bid_id), soci::into(bid);
          contractor.consider_bid(bid, (status == "approve" ? bid_event::approve : bid_event::reject));
          int project_id;
          db << "select project_id from bids where id=:bid_id",
              soci::use(bid_id), soci::into(project_id);
          response().set_redirect_header("/projects/" + std::to_string(project_id));
          return;
        }
        response().set_redirect_header("/");
        return;
      }
    }
    response().set_redirect_header("/");
  }

  virtual void add_project() {
    Data::AddProject addpr;
    add_menu(addpr, *this);

    render("AddProject", addpr);
  }

  static void get_fields(Task& project, Data::AddTask& view) {
    project.name = view.add_project_form.name.value();
    project.description = view.add_project_form.description.value();
    project.wage = view.add_project_form.wage.value();
    project.location = view.add_project_form.location.value();
    project.state = std::make_unique<NotStarted>();
  }

  static void get_fields(LongTermJob& project, Data::AddLong& view) {
    project.name = view.add_project_form.name.value();
    project.description = view.add_project_form.description.value();
    project.wage = view.add_project_form.wage.value();
    project.location = view.add_project_form.location.value();
    project.state = std::make_unique<NotStarted>();
    project.specialization = view.add_project_form.specialization.value();
    project.format = (view.add_project_form.format.selected_id() == "0" ? "remote" : "office");
  }

  static void get_fields(Contest& project, Data::AddContest& view) {
    project.name = view.add_project_form.name.value();
    project.description = view.add_project_form.description.value();
    project.wage = view.add_project_form.wage.value();
    project.location = view.add_project_form.location.value();
    project.state = std::make_unique<NotStarted>();
    project.start_at = view.add_project_form.start_at.value();
    project.end_at = view.add_project_form.end_at.value();
  }

  virtual void add_task() {
    Data::AddTask addpr;
    add_menu(addpr, *this);

    if (request().request_method() == "POST") {
      addpr.add_project_form.load(context());
      if (addpr.add_project_form.validate()) {
        if (session().is_set("role") && session()["role"] == "contractor") {
          Contractor contractor;
          Task project;
          get_fields(project, addpr);

          DataBase db(db_source);
          db << "select * from users where role='contractor' and username=:username",
              soci::use(session()["username"]), soci::into(contractor);

          contractor.add_project(project);

          response().set_redirect_header("/projects");
          return;
        }
        response().set_redirect_header("/");
        return;
      }
    }

    render("AddTask", addpr);
  }

  virtual void add_long() {
    Data::AddLong addpr;
    add_menu(addpr, *this);

    if (request().request_method() == "POST") {
      addpr.add_project_form.load(context());
      if (addpr.add_project_form.validate()) {
        if (session().is_set("role") && session()["role"] == "contractor") {
          Contractor contractor;
          LongTermJob project;
          get_fields(project, addpr);

          DataBase db(db_source);
          db << "select * from users where role='contractor' and username=:username",
              soci::use(session()["username"]), soci::into(contractor);

          contractor.add_project(project);

          response().set_redirect_header("/projects");
          return;
        }
        response().set_redirect_header("/");
        return;
      }
    }

    render("AddLong", addpr);
  }

  virtual void add_contest() {
    Data::AddContest addpr;
    add_menu(addpr, *this);

    if (request().request_method() == "POST") {
      addpr.add_project_form.load(context());
      if (addpr.add_project_form.validate()) {
        if (session().is_set("role") && session()["role"] == "contractor") {
          Contractor contractor;
          Contest project;
          get_fields(project, addpr);

          DataBase db(db_source);
          db << "select * from users where role='contractor' and username=:username",
              soci::use(session()["username"]), soci::into(contractor);

          contractor.add_project(project);

          response().set_redirect_header("/projects");
          return;
        }
        response().set_redirect_header("/");
        return;
      }
    }

    render("AddContest", addpr);
  }

  virtual void advance(std::string id) {
    if (session().is_set("role")) {
      if (session()["role"] == "contractor") {
        DataBase db(db_source);
        Project project;
        db << "select * from projects where id=:id", soci::use(id), soci::into(project);

        Contractor contractor;
        db << "select * from users where role='contractor' and username=:username",
            soci::use(session()["username"]), soci::into(contractor);

        int contractor_id;
        db << "select contractor_id from projects where id=:id", soci::use(id), soci::into(contractor_id);

        if (contractor_id == contractor.id) {
          if (project.state->integer() == 0) {
            contractor.start_project_hiring(project);
          } else if (project.state->integer() == 1) {
            contractor.end_project_hiring(project);
          } else {
            contractor.end_project(project);
          }
          response().set_redirect_header("/projects/" + id);
          return;
        }
        response().set_redirect_header("/");
      } else {
        response().set_redirect_header("/");
      }
    }
    response().set_redirect_header("/");
  }
};