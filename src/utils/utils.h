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

#include "../models/employee.h"
#include "../models/contractor.h"
#include "../models/constants.h"
#include "../models/message.h"

#include "../../data/tmpl_master.h"


template<typename T>
void add_menu(T& object, cppcms::application& app) {
  if (app.session().is_set("username")) {
    object.current_username = app.session()["username"];
  }
  object.menuList.push_back(std::pair<std::string,std::string>("/","Main"));
  object.menuList.push_back(std::pair<std::string,std::string>("/projects","Projects"));
  object.menuList.push_back(std::pair<std::string,std::string>("/users/contractors","Contractors"));
  object.menuList.push_back(std::pair<std::string,std::string>("/users/employees","Creatives"));
  object.menuList.push_back(std::pair<std::string,std::string>("/signup","Sign up"));
  object.menuList.push_back(std::pair<std::string,std::string>("/login","Log in"));
  object.menuList.push_back(std::pair<std::string,std::string>("/logout","Log out"));

  if (app.session().is_set("username")) {
    object.menuList.push_back(std::pair<std::string,std::string>("/notifications","notifications"));
    std::vector<std::pair<int, std::string>> notifications;

    std::string src = "dbname=";
    src += db_source;
    soci::session sql("sqlite3", src);

    int user_id;
    sql << "select id from users where username=:username", soci::use(app.session()["username"]), soci::into(user_id);

    soci::rowset<soci::row> nots = (sql.prepare << "select * from notifications where user_id=:user_id" , soci::use(user_id));
    for (auto &it : nots) {
      if (!it.get<int>("is_read")) {
        notifications.emplace_back(it.get<int>("id"), it.get<std::string>("description"));
      }
    }

    object.notifications = notifications;
  }

  if (app.session().is_set("role") && app.session()["role"] == "contractor") {
    object.menuList.push_back(std::pair<std::string, std::string>("/projects/add_project", "Add Project"));
  }
}

User get_user(cppcms::application& app) {
  if (app.session().is_set("username")) {
    std::string src = "dbname=";
    src += db_source;
    soci::session sql("sqlite3", src);
    User current_user;
    sql << "select * from users where username=:username",
        soci::use(app.session()["username"]), soci::into(current_user);
    return current_user;
  } else {
    throw std::runtime_error("current user is not set");
  }
}

Employee get_employee(cppcms::application& app) {
  if (app.session().is_set("username")) {
    std::string src = "dbname=";
    src += db_source;
    soci::session sql("sqlite3", src);
    Employee current_user;
    sql << "select * from users where username=:username",
        soci::use(app.session()["username"]), soci::into(current_user);
    return current_user;
  } else {
    throw std::runtime_error("current user is not set");
  }
}

User get_user_by_id(const std::string& id) {
  std::string src = "dbname=";
  src += db_source;
  soci::session sql("sqlite3", src);
  User user;
  sql << "select * from users where id=:id", soci::use(id), soci::into(user);
  return user;
}

Employee get_employee_by_id(const std::string& id) {
  std::string src = "dbname=";
  src += db_source;
  soci::session sql("sqlite3", src);
  Employee user;
  sql << "select * from users where id=:id", soci::use(id), soci::into(user);
  return user;
}

double calculate_rating(const User& user) {
  std::string src = "dbname=";
  src += db_source;
  soci::session sql("sqlite3", src);

  soci::rowset<int> rates = (sql.prepare << "select rate from rates where target_id=:target_id",
      soci::use(user.id));
  long long sum = 0;
  int sz = 0;

  for (int& rate : rates) {
    sum += rate;
    ++sz;
  }
  return double(sum) / sz;
}