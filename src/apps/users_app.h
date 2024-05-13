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


class Users : public cppcms::application {
 public:
  Users(cppcms::service &srv) : cppcms::application(srv) {
    dispatcher().assign("/contractors", &Users::contractors, this);
    mapper().assign("/contractors");

    dispatcher().assign("/employees", &Users::employees, this);
    mapper().assign("/employees");

    dispatcher().assign("/(.+)/rate/(.+)", &Users::rate, this, 1, 2);
    mapper().assign("/{1}/rate/{2}");

    dispatcher().assign("/(.+)/subscribe", &Users::subscribe, this, 1);
    mapper().assign("/{1}");

    dispatcher().assign("/(.+)/unsubscribe", &Users::unsubscribe, this, 1);
    mapper().assign("/{1}");

    dispatcher().assign("/(.+)/edit", &Users::edit, this, 1);
    mapper().assign("/{1}");

    dispatcher().assign("/(.+)", &Users::user, this, 1);
    mapper().assign("/{1}");
  }

  virtual void contractors() {
    Data::Contractors cntr;

    add_menu(cntr, *this);

    DataBase db(db_source);
    soci::rowset<Contractor> contractors = (db.prepare("select * from users where role='contractor'"));
    std::vector<Contractor> all_contractors;
    for (auto& contractor : contractors) {
      all_contractors.push_back(contractor);
    }
    cntr.contractors_page.contractors = all_contractors;
    render("Contractors", cntr);
  }

  virtual void employees() {
    Data::Employees empl;

    add_menu(empl, *this);

    DataBase db(db_source);
    soci::rowset<Employee> employees = (db.prepare("select * from users where role='employee'"));
    std::vector<Employee> all_employees;
    for (auto& employee : employees) {
      all_employees.push_back(employee);
    }
    empl.employees_page.employees = all_employees;
    render("Employees", empl);
  }

  virtual void user(std::string id) {
    Data::SingleUser usr;

    add_menu(usr, *this);

    DataBase db(db_source);

    User user = get_user_by_id(id);

    usr.user_page.user = user;

    if (session().is_set("username") && user.user_role == role::contractor) {
      User current_user = get_user(*this);

      int cnt;
      db << "select count(*) from subscriptions where contractor_id=:contractor_id and user_id=:user_id",
          soci::use(id), soci::use(current_user.id), soci::into(cnt);

      if (cnt != 0) {
        usr.user_page.is_subscribed = true;
      } else {
        usr.user_page.is_subscribed = false;
      }
    }

    if (session().is_set("username")) {
      User current_user = get_user(*this);
      usr.my_rate = 0;
      db << "select rate from rates where rater_id=:rater_id and target_id=:target_id",
          soci::use(current_user.id), soci::use(id), soci::into(usr.my_rate);

      usr.my_id = current_user.id;
    }

    if (session().is_set("username") && session()["username"] == usr.user_page.user.username) {
      usr.is_me = true;
    } else {
      usr.is_me = false;
    }


    render("SingleUser", usr);
  }

  virtual void subscribe(std::string id) {
    if (session().is_set("username")) {
      DataBase db(db_source);
      Contractor contractor;
      db << "select * from users where id=:id and role='contractor'", soci::use(id), soci::into(contractor);

      User user;
      db << "select * from users where username=:username",
          soci::use(session()["username"]), soci::into(user);

      contractor.register_observer(user);
      response().set_redirect_header("/users/" + std::to_string(contractor.id));
      return;
    }
    response().set_redirect_header("/");
  }

  virtual void unsubscribe(std::string id) {
    if (session().is_set("username")) {
      DataBase db(db_source);
      Contractor contractor;
      db << "select * from users where id=:id and role='contractor'", soci::use(id), soci::into(contractor);

      User user;
      db << "select * from users where username=:username",
          soci::use(session()["username"]), soci::into(user);

      contractor.remove_observer(user);
      response().set_redirect_header("/users/" + std::to_string(contractor.id));
      return;
    }
    response().set_redirect_header("/");
  }

  virtual void edit(std::string id) {
    Data::ProfileEdit edt;

    add_menu(edt, *this);

    if (request().request_method() == "POST") {

      edt.edit_form.load(context());
      auto current_user = get_user(*this);
      DataBase db(db_source);
      std::string avatar_name;
      std::string cover_name;

      if (edt.edit_form.avatar_file.validate()) {
        avatar_name = std::to_string(std::hash<std::string>()(session()["username"]));
        if (edt.edit_form.avatar_file.value()->mime() == "image/png") {
          avatar_name += ".png";
        } else {
          avatar_name += ".jpg";
        }
        edt.edit_form.avatar_file.value()->save_to("./media/images/" + avatar_name);
        current_user.avatar = avatar_name;

        db << "update users "
               "set avatar = :avatar "
               "where id = :id", soci::use(avatar_name), soci::use(current_user.id);
      }

      if (edt.edit_form.cover_file.validate()) {
        cover_name = std::to_string(std::hash<std::string>()(session()["username"] + "cover"));
        if (edt.edit_form.cover_file.value()->mime() == "image/png") {
          cover_name += ".png";
        } else {
          cover_name += ".jpg";
        }
        edt.edit_form.cover_file.value()->save_to("./media/images/" + cover_name);
        current_user.cover = cover_name;
        db << "update users "
               "set cover = :cover "
               "where id = :id", soci::use(cover_name), soci::use(current_user.id);

      }
      response().set_redirect_header("/users/" + id);
      return;
    }


    DataBase db(db_source);
    User user;
    db << "select * from users where id=:id", soci::use(id), soci::into(user);

    if (session().is_set("username") && session()["username"] == user.username) {
      edt.is_me = true;
    } else {
      edt.is_me = false;
    }

    render("ProfileEdit", edt);
  }

  virtual void rate(std::string id, std::string grade) {
    auto target_user = get_user_by_id(id);
    if (request().request_method() == "POST" && session().is_set("username")) {
      auto current_user = get_user(*this);

      DataBase db(db_source);

      int cnt;
      db << "select count(*) from rates where rater_id=:rater_id and target_id=:target_id",
          soci::use(current_user.id), soci::use(id), soci::into(cnt);

      if (cnt == 0) {
        db << "insert into rates (rater_id, target_id, rate) values(:rater_id, :target_id, :rate)",
            soci::use(current_user.id), soci::use(id), soci::use(std::stod(grade));
      } else {
        db << "update rates "
               "set rate=:rate "
               "where rater_id=:rater_id and target_id=:target_id", soci::use(grade), soci::use(current_user.id), soci::use(id);
      }

      target_user.rate = calculate_rating(target_user);
      db << "update users "
             "set rate=:rate "
             "where id=:id ", soci::use(target_user);
    }
    response().set_redirect_header("/users/" + id);
  }
};