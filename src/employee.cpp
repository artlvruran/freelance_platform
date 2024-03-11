//
// Created by kirill on 26.02.24.
//
#pragma once
#include "employee.h"
#include "sqlite3.h"
#include "constants.h"
#include <boost/format.hpp>

void Employee::sign_up(const std::string& username,
                       const std::string& email,
                       const std::string& password) const {
  sqlite3 *db;
  int rc;
  rc = sqlite3_open(db_source, &db);
  std::string request =
      (boost::format("INSERT INTO employees ('username', 'email', 'password') VALUES ('%s', '%s', '%s')") % username % email % password).str();
  rc = sqlite3_exec(db, request.c_str(), 0, 0, 0);
  sqlite3_close(db);
}

bool Employee::log_in(const std::string& username,
                       const std::string& email,
                       const std::string& password) const {
  sqlite3 *db;
  int rc;
  rc = sqlite3_open(db_source, &db);
  std::string request =
      (boost::format("SELECT id FROM employees WHERE username == '%s' AND email == '%s' AND password == '%s')") % username % email % password).str();
  rc = sqlite3_exec(db, request.c_str(), nullptr, nullptr, nullptr);
  sqlite3_close(db);
  return rc == SQLITE_OK;
}

Bid Employee::create_bid(int project_id) {
  sqlite3_stmt *stmt;
  sqlite3 *db;
  int rc;
  rc = sqlite3_open(db_source, &db);
  std::string request =
      (boost::format("INSERT INTO bids ('project_id', 'employee_id') VALUES (%d, %d)") % project_id % id).str();
  rc = sqlite3_exec(db, request.c_str(), 0, 0, 0);

  request =
      (boost::format("SELECT id FROM bids WHERE project_id == %d AND employee_id == %d") % project_id % id).str();
  rc = sqlite3_prepare_v2(db, request.c_str(), -1, &stmt, nullptr);
  sqlite3_close(db);

  int bid_id = sqlite3_column_int(stmt, 0);
  Bid bid{bid_id, id, project_id};
  return bid;
}