//
// Created by kirill on 26.02.24.
//
#include "employee.h"
#include "sqlite3.h"
#include "constants.h"
#include <boost/format.hpp>

void Employee::sign_up() const {
  sqlite3 *db;
  int rc;
  rc = sqlite3_open(constants::db_source, &db);
  std::string request =
      (boost::format("INSERT INTO employees ('username', 'email', 'password') VALUES ('%s', '%s', '%s')") % username % email % password).str();
  rc = sqlite3_exec(db, request.c_str(), 0, 0, 0);
  sqlite3_close(db);
}