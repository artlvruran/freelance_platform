//
// Created by kirill on 26.02.24.
//
#pragma once
#include "employee.h"
#include "sqlite3.h"
#include "constants.h"
#include <boost/format.hpp>

void Employee::sign_up() {
  std::string src = "dbname=";
  src += db_source;
  soci::session sql("sqlite3", src);
  sql << "insert into users (username, email, password, role) values(:username, :email, :password, 'employee')", soci::use(*this);
}

bool Employee::log_in() {
  sqlite3 *db;
  int rc;
  rc = sqlite3_open(db_source, &db);
  std::string request =
      (boost::format("SELECT id FROM users WHERE username == '%s' AND email == '%s' AND password == '%s' AND role == 'employee'") % username % email % password).str();
  rc = sqlite3_exec(db, request.c_str(), nullptr, nullptr, nullptr);
  sqlite3_close(db);
  return rc == SQLITE_OK;
}

Bid Employee::create_bid(int project_id) {
  std::string src = "dbname=";
  src += db_source;
  soci::session sql("sqlite3", src);
  sql << "insert into bids (project_id, employee_id, state) "
         "values(:project_id, :employee_id, 'considering')", soci::use(project_id), soci::use(id);
  int bid_id;
  sql << "select count(*) from bids ", soci::into(bid_id);
  Bid bid{bid_id, id, project_id};
  return bid;
}